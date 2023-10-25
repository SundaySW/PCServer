#pragma once

/// @file PCServer/logging/fwd.hpp
/// @brief Forward declarations for `logging` types

#include <memory>

namespace PCServer {

namespace logging {

namespace impl {

class BaseLogger;

} // namespace impl

using LoggerRef = impl::BaseLogger&;
using LoggerPtr = std::shared_ptr<impl::BaseLogger>;

namespace impl {

void SetDefaultLogger(const LoggerPtr &new_logger) noexcept;

} // namespace impl

} // namespace logging
} // namespace PCServer