#pragma once

#include <optional>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <utility>

#include "ProtosCloudServer/net/boost_asio_http_session.hpp"
#include "ProtosCloudServer/net/impl/server_base.hpp"
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
class PCS_API BoostAsioHttpServer: public impl::BaseServer{
public:
    using ClientConnectionHandler =
            std::function<void(std::shared_ptr<BoostAsioHttpSession>&& session)>;

    BoostAsioHttpServer(boost::asio::ip::basic_endpoint<Protocol> endpoint,
                        ClientConnectionHandler handler)
        : io_service_(), acceptor_(io_service_, endpoint), socket_(io_service_),
        client_connect_handler_(std::move(handler))
    {}

    ~BoostAsioHttpServer()
    {}

    /**
     * @brief Start receiving new connections.
     */
    void Start() override {
        AsyncAccept();
        io_service_.run();
    }

    /**
    * @brief Stop receiving new connections.
    */
    void Stop() override {
        io_service_.stop();
    }

protected:
    void AsyncAccept() override {
        socket_.emplace(io_service_);
        acceptor_.async_accept(*socket_, [&](boost::system::error_code error) {
            client_connect_handler_(std::make_shared<BoostAsioHttpSession>(std::move(*socket_)));
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