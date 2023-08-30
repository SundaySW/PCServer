//
// Created by user on 17.08.2023.
//

#ifndef PROTOSCLOUDSERVER_HTTP_SERVER_HPP
#define PROTOSCLOUDSERVER_HTTP_SERVER_HPP

#include "ProtosCloudServer/net/asio_server.hpp"

namespace ProtosCloudServer {

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
        if(black_list_.contains(client->GetAddr())){
            client->CloseSession();
            return;
        }

        raw_clients_.insert(client);
        client->Start(
                [this](auto&& _1, auto&& _2) { return HttpSessionMsgHandle(std::forward<decltype(_1)>(_1),
                                                                           std::forward<decltype(_2)>(_2));},
                [&, weak = std::weak_ptr(client)](boost::system::error_code error){
                    if (auto shared = weak.lock(); shared && raw_clients_.erase(shared))
                        HandleClientMsg("We are one less\n\r");
                });
        client->Post(http_parser_.generateResponse("", "text/plain", 200, "OK", true));
    }

    void HandleClientMsg(const std::string &message) {
        for (auto &client: raw_clients_)
            client->Post(message);
    }

    std::unordered_set<std::string> black_list_;
    std::string path_{};
    HttpParser http_parser_;
    ProtosServerMsgHandler protos_server_msgHandler_;
    std::unordered_set<std::shared_ptr<HttpSession>> raw_clients_;
};
}

#endif //PROTOSCLOUDSERVER_HTTP_SERVER_HPP
