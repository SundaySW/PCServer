#include <shared_mutex>
#include "ProtosCloudServer/net/server.hpp"
#include "ProtosCloudServer/net/server_config.hpp"
#include "ProtosCloudServer/net/boost_socket_acceptor.hpp"

using namespace ProtosCloudServer::net;
using namespace ProtosCloudServer;

namespace PServer {

class ServerImpl {
public:
    using Protocol = boost::asio::ip::tcp;
    explicit ServerImpl(const ServerConfig& config);
    ~ServerImpl() = default;
    void HandleNewConnection(std::unique_ptr<BoostHttpSession> session);
    void StartServer();
    void StopServer();
    void SendAll(http::HttpRequest&);
private:
    BoostSocketAcceptor<Protocol> acceptor_;
    ServerConfig config_;
    ProtosCloudServer::auth::AuthCheckerBase auth_checker_;
    std::unordered_map<std::string, std::shared_ptr<ProtosCloudServer::net::HttpClientHandler>> raw_clients_;
    mutable std::shared_mutex on_read_mutex_{};
    ProtosCloudServer::http::HttpParser http_parser_;
    boost::asio::thread_pool pool_;

    void NewConnectionHandler(std::unique_ptr<BoostHttpSession> session);
};

ServerImpl::ServerImpl(const ServerConfig& config)
    : acceptor_(CreateEndPoint<Protocol>(config),
                [this](std::unique_ptr<BoostHttpSession> session){ HandleNewConnection(std::move(session)); }),
      config_(config),
      http_parser_(),
      pool_(4)
{
}

void ServerImpl::HandleNewConnection(std::unique_ptr<BoostHttpSession> session) {
    boost::asio::post(pool_, [this, session = std::move(session)] () mutable {
        NewConnectionHandler(std::move(session));
    });
}

void ServerImpl::NewConnectionHandler(std::unique_ptr<BoostHttpSession> session) {
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