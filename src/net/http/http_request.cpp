
#include "ProtosCloudServer/net/http/http_request.hpp"
#include "ProtosCloudServer/net/http/http_parser.h"

namespace ProtosCloudServer::http{

HttpRequest::HttpRequest(const std::string& data) {
    headers_map_ = parser_.parseHeader<HeadersMapT>(data, true);

    auto contentLengthIter = headers_map_.find("Content-Length");
    size_ = contentLengthIter != headers_map_.end() ?
                              std::stoull(contentLengthIter->second) : 0;
}

unsigned long long HttpRequest::GetSize() {
    return size_;
}

bool HttpRequest::IsEmptyRequest() {
    return size_ == 0;
}

}