#include <atomic>
#include "ProtosCloudServer/logging/log.hpp"
#include "ProtosCloudServer/logging/impl/base_logger.hpp"
#include "ProtosCloudServer/logging/null_logger.hpp"

namespace ProtosCloudServer {

namespace logging {

namespace {

auto& DefaultLoggerRef() noexcept{
    static std::atomic<impl::BaseLogger*> default_logger_ptr{&GetNullLogger()};
    return default_logger_ptr;
}

auto DefaultLoggerPtr() noexcept{
    static LoggerPtr default_logger_holder{};
    return default_logger_holder;
}

}// namespace

namespace impl {

void SetDefaultLogger(const LoggerPtr& logger) noexcept {
    DefaultLoggerRef() = logger.get();
    DefaultLoggerPtr() = logger;
}

}// namespace impl

LoggerRef GetDefaultLogger() noexcept {
    return *DefaultLoggerRef().load();
}

void SetLoggerLevel(LoggerRef logger, Level level) {
    logger.SetLevel(level);
}

void LogFlush(){
    GetDefaultLogger().Flush();
}

} //namespace logging
} //namespace ProtosCloudServer