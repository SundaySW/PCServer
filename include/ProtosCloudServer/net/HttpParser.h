#ifndef PROTOSCLOUDSERVER_HTTPPARSER_H
#define PROTOSCLOUDSERVER_HTTPPARSER_H

#include "Url.h"
#include "HttpReqArg.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace ProtosCloudServer {

class PROTOSCLOUDSERVER_API HttpParser {

public:
    std::string generateRequest(const Url& url, const std::vector<HttpReqArg>& args, bool isKeepAlive = false) const;
    std::string generateMultipartFormData(const std::vector<HttpReqArg>& args, const std::string& boundary) const;
    std::string generateMultipartBoundary(const std::vector<HttpReqArg>& args) const;
    std::string generateWwwFormUrlencoded(const std::vector<HttpReqArg>& args) const;
    std::string generateResponse(const std::string& data, const std::string& mimeType, unsigned short statusCode, const std::string& statusStr, bool isKeepAlive) const;
    std::unordered_map<std::string, std::string> parseHeader(const std::string& data, bool isRequest) const;
    std::string extractBody(const std::string& data) const;
};

}

#endif //PROTOSCLOUDSERVER_HTTPPARSER_H