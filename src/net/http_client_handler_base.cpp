#include <utility>

#include "ProtosCloudServer/net/http_client_handler_base.hpp"

namespace ProtosCloudServer::net {

    HttpClientHandlerBase::HttpClientHandlerBase(std::shared_ptr<HttpSessionHandler> session_handler)
            : session_handler_(std::move(session_handler))
    {
        session_handler_->Start(shared_from_this());
    }

    std::shared_ptr<HttpClientHandlerBase> HttpClientHandlerBase::operator()(std::shared_ptr<http::HttpRequest> request) {

    }

} //namespace ProtosCloudServer::net