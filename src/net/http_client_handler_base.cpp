#include <utility>

#include "ProtosCloudServer/net/http_client_handler_base.hpp"

namespace ProtosCloudServer::net {

    HttpClientHandlerBase::HttpClientHandlerBase(std::shared_ptr<HttpSessionHandler> session_handler)
            : session_handler_(std::move(session_handler))
    {}

    std::string HttpClientHandlerBase::GetAddr() {
        return session_handler_->GetAddr();
    }

    uint16_t HttpClientHandlerBase::GetPort() {
        return session_handler_->GetPort();
    }
} //namespace ProtosCloudServer::net