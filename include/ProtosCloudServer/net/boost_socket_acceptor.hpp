#pragma once

#include <optional>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <utility>

#include "ProtosCloudServer/net/boost_http_session.hpp"
#include "ProtosCloudServer/net/impl/base_socket_acceptor.hpp"
#include "ProtosCloudServer/net/server_config.hpp"
#include "ProtosCloudServer/logging/log.hpp"

using namespace ProtosCloudServer::logging;

namespace ProtosCloudServer::net{

template<typename Protocol>
auto CreateEndPoint(const PServer::ServerConfig& config){
    return typename Protocol::endpoint(boost::asio::ip::address::from_string(config.address.value()), config.port);
}
/**
 * @brief This class make core TCP ProtosServer using Boost Asio
 *
 * @ingroup net
 */

template<typename Protocol>
class PCS_API BoostSocketAcceptor: public impl::BaseSocketAcceptor{
public:
    using ClientConnectionHandler =
            std::function< void (std::unique_ptr<BoostHttpSession> session) >;

    BoostSocketAcceptor(boost::asio::ip::basic_endpoint<Protocol> endpoint,
                        ClientConnectionHandler handler)
        : io_service_(), acceptor_(io_service_, endpoint),
        client_connect_handler_(std::move(handler))
    {}

    ~BoostSocketAcceptor() override = default;

    /**
     * @brief StartAccept receiving new connections.
     */
    void StartAccept() override {
        AsyncAccept();
        io_service_.run();
    }

    /**
    * @brief StopAccept receiving new connections.
    */
    void StopAccept() override {
        io_service_.stop();
    }

protected:
    void AsyncAccept() override {
        socket_.emplace(io_service_);
        acceptor_.async_accept(*socket_, [this](boost::system::error_code error) {
            auto session = std::make_unique<BoostHttpSession>(std::move(*socket_));
            client_connect_handler_(std::move(session));
            AsyncAccept();
        });
    }

private:
    boost::asio::io_service io_service_;
    boost::asio::basic_socket_acceptor<Protocol> acceptor_;
    std::optional<boost::asio::basic_stream_socket<Protocol>> socket_;
    ClientConnectionHandler client_connect_handler_;
};

} //namespace ProtosCloudServer::net