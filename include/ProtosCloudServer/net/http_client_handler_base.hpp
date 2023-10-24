#pragma once

#include <memory>

#include "ProtosCloudServer/logging/log.hpp"

#include "ProtosCloudServer/net/http/http_request.hpp"
#include "ProtosCloudServer/net/impl/http_session_handler.hpp"
#include "ProtosCloudServer/auth/auth_checker_base.hpp"

namespace ProtosCloudServer::net{

class HttpClientHandlerBase{
public:
    explicit HttpClientHandlerBase() = default;
    virtual void AddHandler(http::HttpHandler&&) = 0;
    virtual void AddHandler(http::HttpHandler&) = 0;
    virtual std::string GetAddr() = 0;
    virtual uint16_t GetPort() = 0;
    virtual ~HttpClientHandlerBase();
private:
    virtual void SessionRequestHandler(http::HttpRequest& request) = 0;
};

}  //namespace ProtosCloudServer::net