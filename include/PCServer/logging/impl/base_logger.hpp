#pragma once

#include <memory>
#include <atomic>
#include "chrono"

#include "PCServer/logging/log_type.hpp"
#include "PCServer/logging/level.hpp"

namespace PCServer::logging::impl {
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

} //namespace PCServer::logging::impl