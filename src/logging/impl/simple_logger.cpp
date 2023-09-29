#include <utility>

#include "ProtosCloudServer/logging/simple_logger.hpp"

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

    void SimpleLogger::Log(Level level, std::string_view msg){
        impl::Log log{level, std::string{msg}};
        Push(std::move(log));
    }

    void SimpleLogger::Flush() {
        std::future<void> future;
        {
            std::shared_ptr<std::promise<void>> promise = std::make_shared<std::promise<void>>();
            future = promise->get_future();
//            this->m_worker.pushTask([promise]() {
//                promise->set_value();
//            });
        }
        try{
            future.wait();
        }
        catch (std::future_error& e)
        {}
    }

    bool SimpleLogger::ShouldLog(Level level) const noexcept {
        if (!BaseLogger::ShouldLog(level)) return false;
        return true;
    }

    std::string_view SimpleLogger::GetLoggerName() const noexcept {
        return logger_name_;
    }

    void SimpleLogger::Push(impl::Log&& log) {
        msg_queue_.enqueue(log);
        auto log_to_post = impl::Log();
        auto hasElem = msg_queue_.try_dequeue(log_to_post);
        if (hasElem) {
            switch (log_type_) {
                case LogType::kSync:
                    SyncWrite(std::move(log_to_post));
                case LogType::kAsync:
                    AsyncWrite(std::move(log_to_post));
            }
        }
    }

    void SimpleLogger::SyncWrite(impl::Log&& log) {

    }

    void SimpleLogger::AsyncWrite(impl::Log&& log) {

    }
}