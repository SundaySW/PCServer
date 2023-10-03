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
boost::asio::basic_socket_acceptor<Protocol> CreateEndPoint(const ServerConfig& config){
    using namespace boost::asio::ip;
    if(config.protocol == "udp")
        return udp::endpoint(address::from_string(config.address.value()), config.port);
    else
        return tcp::endpoint(address::from_string(config.address.value()), config.port);
}

/**
 * @brief This class make core TCP ProtosServer using Boost Asio
 *
 * @ingroup net
 */
template<typename Protocol, typename ClientMsgHandler, typename ClientConnectHandler>
class PCS_API BoostAsioHttpServer: public impl::BaseServer{
public:
    BoostAsioHttpServer(const typename boost::asio::basic_socket_acceptor<Protocol>::endpoint_type &endpoint,
                        ClientMsgHandler handler, ClientConnectHandler handler1)
        : io_service_(), acceptor_(io_service_, endpoint), socket_(io_service_),
        http_parser_(),
        client_msg_handler_(handler), client_connect_handler_(handler1)
    {
    }

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
            HandleNewConnection(std::make_shared<BoostAsioHttpSession>(std::move(*socket_)));
            AsyncAccept();
        });
    }

    std::string HandleNewMsg(const std::string& data, const std::unordered_map<std::string, std::string>& headers){
        if (headers.at("_method") == "POST" && headers.at("_path") == path_){
            client_msg_handler_(data, headers);
        }
        return http_parser_.generateResponse("", "text/plain", 200, "OK", false);
    }

    void HandleNewConnection(const std::shared_ptr<BoostAsioHttpSession>& client) {
        LOG_INFO() << "New session: " << client->GetAddr() <<":"<< client->GetPort();
        if(black_list_.contains(client->GetAddr())){
            LOG_WARNING() << "Attempt to connect from black list: " << client->GetAddr() <<":"<< client->GetPort();
            return;
        }

        auto client_addr_as_key = fmt::format("{}:{}", client->GetAddr(), client->GetPort());
        if(!client_connect_handler_(client_addr_as_key)){
            LOG_WARNING() << "Attempt to connect from black list: " << client->GetAddr() <<":"<< client->GetPort();
            return;
        }
        raw_clients_.try_emplace(client_addr_as_key, client);
        client->Start(
                [this](auto&& _1, auto&& _2) { return HandleNewMsg(std::forward<decltype(_1)>(_1),
                                                                   std::forward<decltype(_2)>(_2));},
                [&, weak = std::weak_ptr(client), client_addr_as_key](boost::system::error_code error){
                    if (auto shared = weak.lock(); shared){
                        CloseClientSession(client_addr_as_key);
                    }
                });
        client->Post(http_parser_.generateResponse("", "text/plain", 200, "OK", true));
    }

private:
    boost::asio::io_service io_service_;
    boost::asio::basic_socket_acceptor<Protocol> acceptor_;
    std::optional<boost::asio::basic_stream_socket<Protocol>> socket_;

private:
    void CloseClientSession(auto key){
        RemoveSessionFromStorage(key);
        HandleClientMsg("We are one less\n\r");
    }

    void RemoveSessionFromStorage(const std::string& key){
        raw_clients_.erase(key);
    }

    void HandleClientMsg(const std::string &message){
        for (auto &client: raw_clients_)
            client.second->Post(message);
    }

    std::unordered_set<std::string> black_list_;
    std::string path_{};
    HttpParser http_parser_;
    ClientMsgHandler client_msg_handler_;
    ClientConnectHandler client_connect_handler_;
    std::unordered_map<std::string, std::shared_ptr<BoostAsioHttpSession>> raw_clients_;
};
} //namespace ProtosCloudServer::net