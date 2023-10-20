#pragma once

#include <string>

namespace ProtosCloudServer::http{

class HttpHandler {
public:
    using CallBackT = std::function<void(http::HttpRequest& request)>;

    HttpHandler(std::string requester_name, CallBackT handler)
        :requester_name_(std::move(requester_name)),
        handler_(std::move(handler))
    {}
    std::string_view GetRequesterName(){
        return requester_name_;
    }
    void HandleRequest(http::HttpRequest& request){
        handler_(request);
    }
private:
    std::string requester_name_;
    CallBackT handler_;
};

} //namespace ProtosCloudServer::http