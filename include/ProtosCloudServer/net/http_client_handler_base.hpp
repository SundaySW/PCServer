#pragma once

#include <memory>

#include "ProtosCloudServer/net/http/http_request.hpp"
#include "ProtosCloudServer/net/http_session_handler.hpp"

namespace ProtosCloudServer::net{

class HttpClientHandlerBase : public std::enable_shared_from_this<HttpClientHandlerBase> {
public:
    explicit HttpClientHandlerBase(std::shared_ptr<HttpSessionHandler> session_handler);
    std::shared_ptr<HttpClientHandlerBase> operator()(std::shared_ptr<http::HttpRequest> request);
private:
    bool auth_pass = false;
    std::string addr_;
    uint16_t port_;
    std::shared_ptr<HttpSessionHandler> session_handler_;
};
}