#include "ProtosCloudServer/logging/log.hpp"
namespace ProtosCloudServer {

namespace logging {

    namespace {
        auto &DefaultLoggerInternal() noexcept {
            static std::atomic<ProtosCloudServer::logging::BasicLogger *> default_logger_ptr{};
            return default_logger_ptr;
        }
    }// namespace

    namespace impl {
        void SetDefaultLoggerRef(LoggerRef logger) noexcept {
            DefaultLoggerInternal() = &logger;
        }
    }// namespace impl

    LoggerRef GetDefaultLogger() noexcept {
        return *DefaultLoggerInternal().load();
    }

    void SetLoggerLevel(LoggerRef logger, Level level) { logger.SetLevel(level); }

}
}