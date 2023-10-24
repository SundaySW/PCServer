#pragma once

#include <string>
#include <memory>
#include <functional>

#include "ProtosCloudServer/net/http/http_request.hpp"
#include "ProtosCloudServer/net/http_handler.hpp"
#include "ProtosCloudServer/net/http/http_response.hpp"


namespace ProtosCloudServer::net{

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

} //namespace ProtosCloudServer::net