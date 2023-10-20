#pragma once

#include <memory>

#include "ProtosCloudServer/net/impl/server_base.hpp"
#include "ProtosCloudServer/net/http_client_handler.hpp"
#include "ProtosCloudServer/auth/auth_checker_base.hpp"

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