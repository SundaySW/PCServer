#pragma once

/// @file PCServer/logging/Level.hpp
/// @brief Log levels

#include "string"
#include "exception"

namespace PCServer {

namespace logging {
/// Log levels
    enum class Level {
        kDebug = 0,
        kInfo = 1,
        kWarning = 2,
        kError = 3,
        kNone = 4
    };
    inline constexpr auto kLevelMax = static_cast<int>(Level::kNone);
} //namespace logging
} //namespace PCServer
