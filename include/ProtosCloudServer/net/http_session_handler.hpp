#pragma once

#include <string>
#include <memory>

#include "ProtosCloudServer/net/http_client_handler_fwd.hpp"

namespace ProtosCloudServer::net{

class HttpSessionHandler{
public:
    HttpSessionHandler() = default;
    virtual ~HttpSessionHandler();

    virtual void Start(std::shared_ptr<HttpClientHandlerBase> clientHandler) = 0;
    virtual void Post(const std::string &message) = 0;
    virtual std::string GetAddr() = 0;
    virtual unsigned short GetPort() = 0;
    virtual void CloseSession() = 0;
};

}