#include "ProtosCloudServer/net/http/http_response.hpp"

namespace ProtosCloudServer::http {

ProtosCloudServer::http::HttpResponse::HttpResponse(HttpStatus status)
        :status_(status)
{
//    generateResponse("Internal server error", "text/plain",
//                                        500, "Internal server error", false));
    payload_ = ToString(status);
}

ProtosCloudServer::http::HttpResponse::HttpResponse(HttpStatus status, std::string_view error)
    :status_(status)
{
//    generateResponse("Internal server error", "text/plain",
//                                        500, "Internal server error", false));
    payload_ = fmt::format("{} {} Error: {}", status, ToString(status), error);
}

std::string HttpResponse::toString() {
    return payload_;
}

} //namespace ProtosCloudServer::http