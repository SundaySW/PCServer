//
// Created by user on 17.08.2023.
//

#ifndef PROTOSCLOUDSERVER_HTTPSERVER_HPP
#define PROTOSCLOUDSERVER_HTTPSERVER_HPP

#include "ProtosCloudServer/net/asioServer.hpp"

namespace ProtosCloudServer {

    namespace {
        using handleFunc = std::function<void (std::shared_ptr<Session>)>;
    }

template<typename Protocol, typename EventHandler>
class PROTOSCLOUDSERVER_API HttpServer : public AsioServer<Protocol, handleFunc> {
public:
    using AsioServer = AsioServer<Protocol, handleFunc>;
    HttpServer(
                const typename boost::asio::basic_socket_acceptor<Protocol>::endpoint_type& endpoint,
                EventHandler eventHandler
                )
                : AsioServer(endpoint,
                        [this](auto&& _1)
                            {return HandleNewConnection_(_1);}
                        ),
                  http_parser_(),
                  event_handler_(eventHandler)
    {}
    void StartServer(){
        AsioServer::Start();
    }
    void StopServer(){
        AsioServer::Start();
    }
private:
    std::string Handle_(const std::string& data, const std::unordered_map<std::string, std::string>& headers){
        if (headers.at("_method") == "POST" && headers.at("_path") == path_){
            event_handler_(data, headers);
        }
        return http_parser_.generateResponse("", "text/plain", 200, "OK", false);
    }

    void HandleNewConnection_(const std::shared_ptr<Session>& client) {
        if(black_list_.contains(client->GetAddr())){
            client->CloseSession();
            return;
        }

        raw_clients_.insert(client);
        client->Start(
                [this](auto &&message) { HandleClientMsg(std::forward<decltype(message)>(message)); },
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
    EventHandler event_handler_;
    std::unordered_set<std::shared_ptr<Session>> raw_clients_;
};
}

#endif //PROTOSCLOUDSERVER_HTTPSERVER_HPP
