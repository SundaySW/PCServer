#pragma once

#include "PCServer/net/http/http_status.hpp"

namespace PCServer::http{

class HttpResponse {
public:
    explicit HttpResponse(HttpStatus);
    HttpResponse(HttpStatus, std::string_view msg);
    std::string toString();
private:
    std::string payload_;
    HttpStatus status_;
};

} //namespace PCServer::http