#pragma once

#include "PCServer/net/http_client_handler_base.hpp"
#include "PCServer/net/http_handler.hpp"
#include "PCServer/logging/log.hpp"

namespace PCServer::net {

class HttpClientHandler: public HttpClientHandlerBase {
public:
    using HandlerMap = std::unordered_map<std::string, http::HttpHandler>;

    explicit HttpClientHandler(std::shared_ptr<HttpSessionHandler> session_handler);
    void AddHandler(http::HttpHandler&& handler) override;
    void AddHandler(http::HttpHandler& handler) override;
    void RemoveHandler(std::string &&requester_name);
    void Post(http::HttpRequest& request);
    void Post(std::string&& msg);
    std::string GetAddr() override;
    uint16_t GetPort() override;
    ~HttpClientHandler() override;

private:
    std::shared_ptr<HttpSessionHandler> session_handler_;
    HandlerMap handlerMap_;
    void SessionRequestHandler(http::HttpRequest& request) override;
    void ProcessReqOnHandler(auto);
};

} //namespace PCServer::net