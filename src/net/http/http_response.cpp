#include "ProtosCloudServer/net/http/http_response.hpp"

namespace ProtosCloudServer::http {

ProtosCloudServer::http::HttpResponse::HttpResponse(HttpStatus status)
    :status_(status)
{
//    generateResponse("Internal server error", "text/plain",
//                                        500, "Internal server error", false));
}

std::string HttpResponse::toString() {
    return std::string();
}

} //namespace ProtosCloudServer::http