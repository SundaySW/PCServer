#pragma once

#include "ProtosCloudServer/net/http/http_status.hpp"

namespace ProtosCloudServer::http{

class HttpResponse {
public:
    explicit HttpResponse(HttpStatus);
    std::string toString();
private:
    HttpStatus status_;
};

} //namespace ProtosCloudServer::http