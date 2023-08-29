#include <utility>

#include "ProtosCloudServer/logging/impl/simple_logger.hpp"

namespace ProtosCloudServer::logging::impl {

    SimpleLogger::SimpleLogger(LogType logType, std::string logger_name)
        :logger_name_(std::move(logger_name))
        ,log_type_(logType)
    {
        SetLevel(logging::Level::kInfo);
    }

    SimpleLogger::~SimpleLogger()
    {
    }

    void SimpleLogger::Log(Level level, std::string_view msg) {
        impl::Log log{level, std::string{msg}};
        Push(std::move(log));
    }

    void SimpleLogger::Flush() {
        auto promise = std::promise<void>();
        auto future = promise.get_future();
        future.get();
    }

    bool SimpleLogger::ShouldLog(Level level) const noexcept {
        if (!BaseLogger::ShouldLog(level)) return false;
        return true;
    }

    std::string_view SimpleLogger::GetLoggerName() const noexcept {
        return logger_name_;
    }

    void SimpleLogger::Push(impl::Log&& log) {
//        task_queue_.enqueue(25);
    }
}