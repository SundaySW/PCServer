
#include <future>
#include "ProtosCloudServer/net/http_client_handler.hpp"

namespace ProtosCloudServer::net {

using namespace http;

HttpClientHandler::HttpClientHandler(std::unique_ptr<HttpSessionHandler> session_handler)
    :session_handler_(std::move(session_handler))
{
    session_handler_->Start([this](HttpRequest& request)
                            { SessionRequestHandler(request); }
    );
}

void HttpClientHandler::SessionRequestHandler(HttpRequest& request) {
    if(!handlerMap_.empty())
            for(auto& handler : handlerMap_)
                handler.second.HandleRequest(request);
//                ProcessReqOnHandler([&handler, &request](){ handler.second.HandleRequest(request);});
}

void HttpClientHandler::ProcessReqOnHandler(auto collable){
    auto th = new std::thread(collable);
//    auto fut = std::async( std::launch::async, collable );
//    fut.get();
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

uint16_t HttpClientHandler::GetPort() {
    return session_handler_->GetPort();
}

std::string HttpClientHandler::GetAddr() {
    return session_handler_->GetAddr();
}

HttpClientHandler::~HttpClientHandler() = default;

} //namespace ProtosCloudServer::net