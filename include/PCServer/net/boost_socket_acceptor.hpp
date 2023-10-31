#pragma once

#include <optional>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <utility>
#include <any>

#include "PCServer/net/boost_http_session.hpp"
#include "PCServer/net/impl/base_socket_acceptor.hpp"
#include "PCServer/net/server_config.hpp"
#include "PCServer/logging/log.hpp"
#include "PCServer/engine/task_management.hpp"

using namespace PCServer::logging;

namespace PCServer::net{

template<typename Protocol>
auto CreateEndPoint(const PServer::ServerConfig& config){
    return typename Protocol::endpoint(boost::asio::ip::address::from_string(config.address.value()), config.port);
}
/**
 * @brief This class make engine TCP ProtosServer using Boost Asio
 *
 * @ingroup net
 */

template<typename Protocol>
class PCS_API BoostSocketAcceptor: public impl::BaseSocketAcceptor{
public:
    using CallBackT = std::function<void(std::shared_ptr<BoostHttpSession>)>;

    BoostSocketAcceptor(boost::asio::ip::basic_endpoint<Protocol> endpoint, CallBackT callBack)
        : io_service_(), acceptor_(io_service_, endpoint), client_connect_handler_(std::move(callBack))
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
            auto session = std::make_shared<BoostHttpSession>(std::move(*socket_));
            engine::PushTask(engine::TaskContext(
                [this, session]{
                    client_connect_handler_(std::move(session));
                }
            ));
            AsyncAccept();
        });
    }

private:
    boost::asio::io_service io_service_;
    boost::asio::basic_socket_acceptor<Protocol> acceptor_;
    std::optional<boost::asio::basic_stream_socket<Protocol>> socket_;
    CallBackT client_connect_handler_;
};

} //namespace PCServer::net