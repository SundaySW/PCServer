#pragma once

#include "ProtosCloudServer/net/http_client_handler_base.hpp"
#include "ProtosCloudServer/net/http_handler.hpp"

namespace ProtosCloudServer::net {

class HttpClientHandler: public HttpClientHandlerBase {
public:
    using HandlerMap = std::unordered_map<std::string, http::HttpHandler>;
    explicit HttpClientHandler(std::shared_ptr<HttpSessionHandler> session_handler);
    void AddHandler(http::HttpHandler&& handler);
    void AddHandler(http::HttpHandler& handler);
    void RemoveHandler(std::string &&requester_name);
    void Post(http::HttpRequest& request);
    void Post(std::string&& msg);
private:
    HandlerMap handlerMap_;
    void SessionRequestHandler(http::HttpRequest& request) override;
};

} //namespace ProtosCloudServer::net