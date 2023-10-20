
#include "ProtosCloudServer/net/http_client_handler.hpp"

namespace ProtosCloudServer::net {

HttpClientHandler::HttpClientHandler(std::shared_ptr<HttpSessionHandler> session_handler)
        : HttpClientHandlerBase(std::move(session_handler))
{
    session_handler_->Start([this](http::HttpRequest& request)
                            { SessionRequestHandler(request);}
    );
}

void HttpClientHandler::SessionRequestHandler(http::HttpRequest &request) {
    if(!handlerMap_.empty())
        for(auto& handler : handlerMap_)
            handler.second.HandleRequest(request);
}

void HttpClientHandler::AddHandler(http::HttpHandler &&handler) {
    handlerMap_.emplace(handler.GetRequesterName(), std::move(handler));
}

void HttpClientHandler::RemoveHandler(std::string&& requester_name) {
    handlerMap_.erase(requester_name);
}

void HttpClientHandler::AddHandler(http::HttpHandler& handler) {
    handlerMap_.emplace(handler.GetRequesterName(), handler);
}

void HttpClientHandler::Post(http::HttpRequest& request){
    session_handler_->Post(request.GetPayload());
}

void HttpClientHandler::Post(std::string&& msg) {
    session_handler_->Post(msg);
}

} //namespace ProtosCloudServer::net