
#include <utility>
#include <fmt/chrono.h>
#include <filesystem>

#include "PCServer/logging/impl/simple_logger.hpp"

namespace PCServer::logging{

    LoggerRef GetSimpleLogger() noexcept{
        static impl::SimpleLogger simple_logger{LogType::kAsync, "simple_logger"};
        return simple_logger;
    }
    LoggerPtr MakeSimpleLogger(){
        return { std::shared_ptr<void>{}, &logging::GetSimpleLogger() };
    }

}

namespace PCServer::logging::impl {

    SimpleLogger::SimpleLogger(LogType logType, std::string logger_name)
        :logger_name_(std::move(logger_name))
        ,log_type_(logType)
    {
        SetLevel(logging::Level::kInfo);
        std::filesystem::create_directories("logs");
        std::string filePath = fmt::format( "logs/{:%d-%m-%Y_%H.%M.%S}", std::chrono::system_clock::now() );

        fstream_.open(filePath.c_str(), std::fstream::out | std::fstream::app);
    }

    SimpleLogger::~SimpleLogger()
    {
    }

    void SimpleLogger::Log(Level level, std::string_view msg){
        impl::Log log{level, std::string{msg}};
        Push(std::move(log));
    }

    void SimpleLogger::Flush() {
        fstream_.flush();
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
                    break;
                case LogType::kAsync:
                    AsyncWrite(std::move(log_to_post));
            }
        }
    }

    void SimpleLogger::SyncWrite(impl::Log&& log) {
        auto future = std::async( std::launch::deferred,
                                  [this, &log]()
                                  {
                                      fstream_ << fmt::format("{:%d-%m-%Y %H:%M:%S} ", log.time ) << log.payload;
                                      Flush();
                                  } );
        try{
            future.wait();
        }
        catch (std::future_error& e)
        {}
    }

    void SimpleLogger::AsyncWrite(impl::Log&& log) {
        auto future = std::async( std::launch::async,
                                  [this, &log]()
                                  {
                                    fstream_ << fmt::format("{:%d-%m-%Y %H:%M:%S} ", log.time ) << log.payload;
                                    Flush();
                                  } );
        try{
            future.wait();
        }
        catch (std::future_error& e)
        {}
    }
}