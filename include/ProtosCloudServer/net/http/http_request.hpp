#pragma once

#include "ProtosCloudServer/net/http/http_req_arg.h"
#include "ProtosCloudServer/net/http/http_parser.h"

namespace ProtosCloudServer{
namespace http {

template<typename KeyT, typename ValueT>
class HeadersMap : public std::unordered_map<KeyT,ValueT>
{};

class HttpRequest {
public:
    using HeadersMapT = HeadersMap<std::string, std::string>;
    explicit HttpRequest(const std::string&);
    HttpRequest(std::string, const HeadersMapT& headersMap);
    unsigned long long GetSize();
    bool IsEmptyRequest();
private:
    HttpParser parser_;
    unsigned long long size_;
    HeadersMapT headers_map_;
    std::vector<HttpReqArg> args_;
};

} //namespace http
} //namespace ProtosCloudServer