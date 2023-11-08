#pragma once

#include <string>
#include <vector>
#include <optional>

namespace PCServer{
namespace auth{

class UserScope final {
public:
    explicit UserScope(std::string value) : value_(std::move(value)) {}
    const std::string& GetValue() const { return value_; }
private:
    std::string value_;
};

using UserScopes = std::vector<UserScope>;

struct UserAuthInfo {
    std::uint64_t default_id_;
    std::optional<UserScopes> scopes_;
};

struct UserDbInfo {
    Token token;
    std::int64_t user_id;
    std::vector<std::string> scopes;
    std::string name;
};

} //namespace auth
} //namespace PCServer


