#include <ProtosCloudServer/logging/log_driver.hpp>
#include <ProtosCloudServer/logging/impl/base_logger.hpp>

namespace ProtosCloudServer::logging{

void LogDriver::PutMessageEnd(){
    msg_.push_back('\n');
}

void LogDriver::LogMessage() const{
    const std::string_view message(msg_.data(), msg_.size());
    logger_->Log(level_, message);
}

void LogDriver::DoLog() noexcept {
    try {
        PutMessageEnd();
        LogMessage();
    } catch (...) {
        InternalLoggingError("Failed to flush log");
    }
}

void LogDriver::InternalLoggingError(std::string_view message) noexcept {
    try{
        std::cerr << "LogDriver: " << message << ". "
                  << boost::current_exception_diagnostic_information() << '\n';
    } catch (...) {
        // ignore
    }
}

LogDriver::~LogDriver() {
    DoLog();
}

}// namespace ProtosCloudServer::logging