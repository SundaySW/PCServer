#pragma once

#include <memory>

#include "ProtosCloudServer/logging/log.hpp"

#include "ProtosCloudServer/net/http/http_request.hpp"
#include "ProtosCloudServer/net/http_session_handler.hpp"
#include "ProtosCloudServer/auth/auth_checker_base.hpp"

namespace ProtosCloudServer::net{

class HttpClientHandlerBase{
public:
    explicit HttpClientHandlerBase(std::shared_ptr<HttpSessionHandler> session_handler);
    virtual void AddHandler(http::HttpHandler&&) = 0;
    virtual void AddHandler(http::HttpHandler&) = 0;
    std::string GetAddr();
    uint16_t GetPort();
protected:
    std::shared_ptr<HttpSessionHandler> session_handler_;
private:
    virtual void SessionRequestHandler(http::HttpRequest& request) = 0;
};

}  //namespace ProtosCloudServer::net