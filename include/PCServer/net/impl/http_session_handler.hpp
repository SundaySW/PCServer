#pragma once

#include <string>
#include <memory>
#include <functional>

#include "PCServer/net/http/http_request.hpp"
#include "PCServer/net/http_handler.hpp"
#include "PCServer/net/http/http_response.hpp"


namespace PCServer::net{

class HttpSessionHandler{
public:
    HttpSessionHandler() = default;
    virtual ~HttpSessionHandler();
    virtual void Start(http::HttpHandler::CallBackT) = 0;
    virtual void Post(http::HttpResponse& response) = 0;
    virtual void Post(std::string& msg) = 0;
    virtual std::string GetAddr() = 0;
    virtual unsigned short GetPort() = 0;
    virtual void CloseSession() = 0;
};

} //namespace PCServer::net