#pragma once

#include <memory>

#include "PCServer/net/impl/base_socket_acceptor.hpp"
#include "PCServer/net/http_client_handler.hpp"
#include "PCServer/auth/auth_checker_base.hpp"

namespace PServer{

class ServerImpl;
struct ServerConfig;

class Server final{
public:
    explicit Server(const ServerConfig& config);
    ~Server();
    void Start();
    void Stop();
private:
    std::unique_ptr<ServerImpl> pimpl;
};

} //namespace ProtosServer