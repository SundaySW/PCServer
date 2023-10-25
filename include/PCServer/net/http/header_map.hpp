#pragma once

#include <unordered_map>
namespace PCServer::http {

template<typename KeyT, typename ValueT>
class HeaderMap : public std::unordered_map<KeyT, ValueT> {
};

using HeaderMapT = HeaderMap<std::string, std::string>;

} //namespace PCServer::http