#pragma once

#include <memory>

#include "PCServer/logging/log.hpp"

#include "PCServer/net/http/http_request.hpp"
#include "PCServer/net/impl/http_session_handler.hpp"
#include "PCServer/auth/auth_checker_base.hpp"

namespace PCServer::net{

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

}  //namespace PCServer::net