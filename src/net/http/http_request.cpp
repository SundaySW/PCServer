
#include "ProtosCloudServer/net/http/http_request.hpp"
#include "ProtosCloudServer/net/http/http_parser.h"

namespace ProtosCloudServer::http{

HttpRequest::HttpRequest(const std::string& headerData, const std::string& owner)
    :parser_(),
     args_(),
     owner_(owner)
{
    headers_map_ = parser_.parseHeader(headerData, true);
    auto contentLengthIter = headers_map_.find("Content-Length");
    size_ = contentLengthIter != headers_map_.end() ?
                              std::stoull(contentLengthIter->second) : 0;
}

unsigned long long HttpRequest::GetSize() const {
    return size_;
}

bool HttpRequest::IsEmptyRequest() const {
    return size_ == 0;
}

void HttpRequest::StoreData(std::string data){
    payload_ = std::move(data);
}

std::string& HttpRequest::GetPayload() {
    return *payload_;
}

std::string_view HttpRequest::GetOwner() {
    return owner_;
}

}