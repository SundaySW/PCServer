#pragma once

#include <string>
#include <optional>

struct AuthCheckResult {
    enum class Status {
        kTokenNotFound,
        kInternalCheckFailure,
        kInvalidToken,
        kForbidden,
        kOk
    };

    Status status{Status::kOk};
    std::optional<std::string> reason{};
};
