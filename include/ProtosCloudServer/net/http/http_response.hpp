#pragma once

#include "ProtosCloudServer/net/http/http_status.hpp"

namespace ProtosCloudServer::http{

class HttpResponse {
public:
    explicit HttpResponse(HttpStatus);
    HttpResponse(HttpStatus, std::string_view msg);
    std::string toString();
private:
    std::string payload_;
    HttpStatus status_;
};

} //namespace ProtosCloudServer::http