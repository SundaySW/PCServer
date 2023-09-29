#pragma once

#include <cstdint>
#include <string>
#include <chrono>
#include <optional>

namespace ProtosCloudServer{

struct ConnectionConfig {
    size_t in_buffer_size = 32 * 1024;
    size_t requests_queue_size_threshold = 100;
    std::chrono::seconds keepalive_timeout{10 * 60};
};

struct ServerConfig {
    ConnectionConfig connection_config;
    std::optional<std::string> protocol;
    std::optional<std::string> address;
    uint16_t port = 15001;
    size_t max_connections = 333;
};

}// namespace ProtosCloudServer