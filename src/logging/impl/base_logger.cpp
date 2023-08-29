//
// Created by user on 25.08.2023.
//
#include "ProtosCloudServer/logging/impl/base_logger.hpp"

namespace ProtosCloudServer::logging::impl{

    void BaseLogger::SetLevel(Level level) {
        level_ = level;
    }

    BaseLogger::BaseLogger() noexcept = default;

    void BaseLogger::Flush() {}

    Level BaseLogger::GetLevel() const noexcept {
        return level_;
    }

    bool BaseLogger::ShouldLog(Level level) const noexcept {
        return this->GetLevel() <= level && level != Level::kNone;
    }

    void BaseLogger::SetFlushOn(Level level) {
        flush_level_ = level;
    }

    bool BaseLogger::ShouldFlush(Level level) const {
        return flush_level_ <= level;
    }

    BaseLogger::~BaseLogger() = default;
}