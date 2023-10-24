#pragma once

#include <memory>
#include <atomic>
#include "chrono"

#include "ProtosCloudServer/logging/log_type.hpp"
#include "ProtosCloudServer/logging/level.hpp"

namespace ProtosCloudServer::logging::impl {
/// Base logger class
class BaseLogger{
public:
    BaseLogger(const BaseLogger&) = delete;
    BaseLogger(BaseLogger&&) = delete;
    BaseLogger& operator=(const BaseLogger&) = delete;
    BaseLogger& operator=(BaseLogger&&) = delete;
    virtual void SetLevel(Level level);
    explicit BaseLogger() noexcept;
    virtual ~BaseLogger();

    virtual void Log(Level level, std::string_view msg) = 0;
    virtual void Flush();
    [[nodiscard]] Level GetLevel() const noexcept;
    [[nodiscard]] virtual bool ShouldLog(Level level) const noexcept;
    void SetFlushOn(Level level);
    [[nodiscard]] bool ShouldFlush(Level level) const;

private:
    std::atomic<Level> level_{Level::kNone};
    std::atomic<Level> flush_level_{Level::kWarning};
};

} //namespace ProtosCloudServer::logging::impl