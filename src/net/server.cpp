
#include <shared_mutex>

#include "PCServer/net/server.hpp"
#include "PCServer/net/server_config.hpp"
#include "PCServer/net/boost_socket_acceptor.hpp"
#include "PCServer/engine/task_management.hpp"
#include "PCServer/utils/meta.hpp"


using namespace PCServer::net;
using namespace PCServer;

namespace PServer {

class ServerImpl {
public:
    using Protocol = boost::asio::ip::tcp;
    using SocketAcceptor = BoostSocketAcceptor<Protocol>;
    using CallBackArgT = function_traits<SocketAcceptor::CallBackT>::arg_type;

    explicit ServerImpl(const ServerConfig& config);
    ~ServerImpl() = default;
    void HandleNewConnection(CallBackArgT);
    void StartServer();
    void StopServer();
    void SendAll(http::HttpRequest&);
private:
    SocketAcceptor acceptor_;
    ServerConfig config_;
    PCServer::auth::AuthCheckerBase auth_checker_;
    std::unordered_map<std::string, std::shared_ptr<PCServer::net::HttpClientHandler>> raw_clients_;
    mutable std::shared_mutex on_read_mutex_{};
    PCServer::http::HttpParser http_parser_;

    void NewConnectionHandler(CallBackArgT);
};

ServerImpl::ServerImpl(const ServerConfig& config)
    : acceptor_(CreateEndPoint<Protocol>(config),
                [this](CallBackArgT session){ HandleNewConnection(std::move(session)); }),
      config_(config),
      http_parser_()
{}

void ServerImpl::HandleNewConnection(CallBackArgT session) {
    NewConnectionHandler(std::move(session));
}

void ServerImpl::NewConnectionHandler(CallBackArgT session) {
    auto client_addr_as_key = fmt::format("{}:{}", session->GetAddr(), session->GetPort());
    auto newClient = std::make_shared<HttpClientHandler>(std::move(session));
    newClient->AddHandler(http::HttpHandler("main", [this](http::HttpRequest& request){SendAll(request);}));
    newClient->Post(http_parser_.generateResponse("", "text/plain", 200, "OK", true));
    auto res = raw_clients_.emplace(std::move(client_addr_as_key), std::move(newClient));
    if (!res.second){
        throw std::runtime_error("can't handle new client with addr: " + res.first->first);
    }
}

void ServerImpl::SendAll(http::HttpRequest& request){
    for(const auto& client : raw_clients_)
        if(request.GetOwner() != client.first)
            client.second->Post(request);
}

void ServerImpl::StartServer() {
    acceptor_.StartAccept();
}

void ServerImpl::StopServer() {
    acceptor_.StopAccept();
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