//
// Created by user on 17.08.2023.
//

#ifndef PROTOSCLOUDSERVER_HTTP_SERVER_HPP
#define PROTOSCLOUDSERVER_HTTP_SERVER_HPP

#include <fmt/format.h>

#include "ProtosCloudServer/net/asio_server.hpp"

namespace ProtosCloudServer {

namespace net{

namespace {
    using handleFunc = std::function<void (std::shared_ptr<HttpSession>)>;
}

template<typename Protocol, typename ProtosServerMsgHandler>
class PCS_API HttpServer : public AsioServer<Protocol, handleFunc> {
public:
    using Server = AsioServer<Protocol, handleFunc>;
    HttpServer(
                const typename boost::asio::basic_socket_acceptor<Protocol>::endpoint_type& endpoint,
                ProtosServerMsgHandler protos_server_msgHandler
               )
               :Server(endpoint,
                        [this](auto&& _1)
                            {return HandleNewConnection_(_1);}
                        ),
               http_parser_(),
               protos_server_msgHandler_(protos_server_msgHandler)
    {}
    void StartServer(){
        Server::Start();
    }
    void StopServer(){
        Server::Start();
    }
private:
    std::string HttpSessionMsgHandle(const std::string& data, const std::unordered_map<std::string, std::string>& headers){
        if (headers.at("_method") == "POST" && headers.at("_path") == path_){
            protos_server_msgHandler_(data, headers);
        }
        return http_parser_.generateResponse("", "text/plain", 200, "OK", false);
    }

    void HandleNewConnection_(const std::shared_ptr<HttpSession>& client) {
        LOG(logging::Level::kInfo) << "New session: " << client->GetAddr() <<":"<< client->GetPort();
        if(black_list_.contains(client->GetAddr())){
            LOG(logging::Level::kWarning) << "Attempt to connect from black list: " << client->GetAddr() <<":"<< client->GetPort();
            return;
        }

        auto client_addr_as_key = fmt::format("{}:{}", client->GetAddr(), client->GetPort());
        raw_clients_.try_emplace(client_addr_as_key, client);
        client->Start(
                [this](auto&& _1, auto&& _2) { return HttpSessionMsgHandle(std::forward<decltype(_1)>(_1),
                                                                           std::forward<decltype(_2)>(_2));},
                [&, weak = std::weak_ptr(client), client_addr_as_key](boost::system::error_code error){
                    if (auto shared = weak.lock(); shared){
                        CloseClientSession(client_addr_as_key);
                    }
                });
        client->Post(http_parser_.generateResponse("", "text/plain", 200, "OK", true));
    }

    void CloseClientSession(auto key){
        RemoveSessionFromStorage(key);
        HandleClientMsg("We are one less\n\r");
    }

    void RemoveSessionFromStorage(const std::string& key){
        raw_clients_.erase(key);
    }

    void HandleClientMsg(const std::string &message) {
        for (auto &client: raw_clients_)
            client.second->Post(message);
    }

    std::unordered_set<std::string> black_list_;
    std::string path_{};
    HttpParser http_parser_;
    ProtosServerMsgHandler protos_server_msgHandler_;
    std::unordered_map<std::string, std::shared_ptr<HttpSession>> raw_clients_;
};

} //namespace net
} //namespace ProtosCloudServer
#endif //PROTOSCLOUDSERVER_HTTP_SERVER_HPP
