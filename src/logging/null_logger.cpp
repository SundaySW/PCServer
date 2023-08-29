
#include "ProtosCloudServer/logging/null_logger.hpp"
#include "ProtosCloudServer/logging/impl/base_logger.hpp"

namespace ProtosCloudServer {

namespace logging {

namespace {

class NullLogger final : public impl::BaseLogger {
public:
    NullLogger() noexcept{
        BaseLogger::SetLevel(Level::kNone);
    }

    void SetLevel(Level) override {}  // do nothing
    void Log(Level, std::string_view) override {}

    void Flush() override {}
};

}// namespace

LoggerRef GetNullLogger() noexcept {
    static NullLogger null_logger{};
    return null_logger;
}

LoggerPtr MakeNullLogger() {
    return LoggerPtr(std::shared_ptr<void>{}, &logging::GetNullLogger());
}

}// namespace logging
}// namespace ProtosCloudServer