#pragma once

#include "url.h"
#include "ProtosCloudServer/net/http/http_req_arg.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace ProtosCloudServer::http {

class PCS_API HttpParser {

public:
    std::string generateRequest(const Url& url, const std::vector<HttpReqArg>& args, bool isKeepAlive = false) const;
    std::string generateMultipartFormData(const std::vector<HttpReqArg>& args, const std::string& boundary) const;
    std::string generateMultipartBoundary(const std::vector<HttpReqArg>& args) const;
    std::string generateWwwFormUrlencoded(const std::vector<HttpReqArg>& args) const;
    std::string generateResponse(const std::string& data, const std::string& mimeType, unsigned short statusCode,
                                 const std::string& statusStr, bool isKeepAlive) const;
    template<typename HeadersMap>
    HeadersMap parseHeader(const std::string& data, bool isRequest) const;
    std::string extractBody(const std::string& data) const;
};

}