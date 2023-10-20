#include <shared_mutex>
#include "ProtosCloudServer/net/server.hpp"
#include "ProtosCloudServer/net/server_config.hpp"
#include "ProtosCloudServer/net/boost_asio_http_server.hpp"

using namespace ProtosCloudServer::net;
using namespace ProtosCloudServer;

namespace PServer {

class ServerImpl {
public:
    using Protocol = boost::asio::ip::tcp;
    explicit ServerImpl(const ServerConfig& config);
    ~ServerImpl() = default;
    void HandleNewConnection(std::shared_ptr<BoostAsioHttpSession>&& session);
    void StartServer();
    void StopServer();
    void SendAll(http::HttpRequest&);
private:
    BoostAsioHttpServer<Protocol> server_;
    ServerConfig config_;
    ProtosCloudServer::auth::AuthCheckerBase auth_checker_;
    std::unordered_map<std::string, std::shared_ptr<ProtosCloudServer::net::HttpClientHandler>> raw_clients_;
    mutable std::shared_mutex on_read_mutex_{};
    ProtosCloudServer::http::HttpParser http_parser_;
};

ServerImpl::ServerImpl(const ServerConfig& config)
    : server_(CreateEndPoint<Protocol>(config),
        [this](std::shared_ptr<BoostAsioHttpSession> &&session){HandleNewConnection(std::move(session));}),
      config_(config),
      http_parser_()
{
}

void ServerImpl::HandleNewConnection(std::shared_ptr<BoostAsioHttpSession> &&session) {
    auto client_addr_as_key = fmt::format("{}:{}", session->GetAddr(), session->GetPort());

//    if(black_list_.contains(session->GetAddr())){
//        LOG_WARNING() << "Attempt to connect from black list: " << client_addr_as_key;
//        return;
//    }
//
//    if(!client_connect_handler_(client_addr_as_key)){
//        LOG_WARNING() << "Attempt to connect from black list: " << client_addr_as_key;
//        return;
//    }

    auto newClient = std::make_shared<HttpClientHandler>(std::move(session));
    newClient->AddHandler(http::HttpHandler("main", [this](http::HttpRequest& request){SendAll(request);}));
    newClient->Post(http_parser_.generateResponse("", "text/plain", 200, "OK", true));
    newClient->RemoveHandler("main");
    auto res = raw_clients_.emplace(std::move(client_addr_as_key), std::move(newClient));
    if (!res.second) {
        throw std::runtime_error("can't handle new client with addr: " + res.first->first);
    }
}

void ServerImpl::SendAll(http::HttpRequest& request){
    for(const auto& client : raw_clients_)
        if(request.GetOwner() != client.first)
            client.second->Post(request);
}

void ServerImpl::StartServer() {
    server_.Start();
}

void ServerImpl::StopServer() {
    server_.Stop();
}

Server::~Server() = default;

Server::Server(const ServerConfig& config)
    : pimpl(std::make_unique<ServerImpl>(config))
{}

void Server::Start() {
    LOG_INFO() << "Starting server";
    pimpl->StartServer();
    LOG_INFO() << "Server is started";
}

void Server::Stop() {
    pimpl->StopServer();
}

} // namespace PServer