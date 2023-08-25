//
// Created by user on 25.08.2023.
//

#ifndef PROTOSCLOUDSERVER_BASE_LOGGER_HPP
#define PROTOSCLOUDSERVER_BASE_LOGGER_HPP

#include <memory>
#include <atomic>
#include "chrono"

#include "ProtosCloudServer/logging/level.hpp"

namespace ProtosCloudServer::logging {

struct LogMessage final {
    std::string_view payload;
    Level level{logging::Level::kError};
};

class BasicLogger{
public:
    BasicLogger() = delete;
    BasicLogger(const BasicLogger&) = delete;
    BasicLogger(BasicLogger&&) = delete;
    BasicLogger& operator=(const BasicLogger&) = delete;
    BasicLogger& operator=(BasicLogger&&) = delete;

    virtual void SetLevel(Level level);
private:
    std::atomic<Level> level_{Level::kNone};
    std::atomic<Level> flush_level_{Level::kWarning};
};

}

#endif //PROTOSCLOUDSERVER_BASE_LOGGER_HPP
