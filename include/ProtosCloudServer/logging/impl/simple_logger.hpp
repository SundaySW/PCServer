#ifndef PROTOSCLOUDSERVER_SIMPLE_LOGGER_HPP
#define PROTOSCLOUDSERVER_SIMPLE_LOGGER_HPP

#include <atomic>
#include <future>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <queue>

#include "moodycamel/concurrentqueue.h"

#include "ProtosCloudServer/logging/impl/base_logger.hpp"

namespace ProtosCloudServer{
namespace logging{
namespace impl{

struct Log {
    Level level{};
    std::string payload{};
    std::chrono::system_clock::time_point time{std::chrono::system_clock::now()};
};

using Task = std::function<void()>;
using TaskQueue = moodycamel::ConcurrentQueue<Task>;

class SimpleLogger final: public BaseLogger{
    SimpleLogger(LogType logType, std::string logger_name);
    ~SimpleLogger() override;
    void Log(Level level, std::string_view msg) override;
    void Flush() override;
    bool ShouldLog(Level level) const noexcept override;
    std::string_view GetLoggerName() const noexcept;
private:
    const std::string logger_name_;
    LogType log_type_;
    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic_bool m_stopped;
    TaskQueue task_queue_;

    void Push(impl::Log&& log);
};

} //namespace impl
} //namespace logging
} //namespace ProtosCloudServer

#endif //PROTOSCLOUDSERVER_SIMPLE_LOGGER_HPP
