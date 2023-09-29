#pragma once

#include "ProtosCloudServer/net/server_config.hpp"

namespace ProtosCloudServer::net::impl {
/// Base server class
    class BaseServer{
    public:
        BaseServer(const BaseServer&) = delete;
        BaseServer(BaseServer&&) = delete;
        BaseServer& operator=(const BaseServer&) = delete;
        BaseServer& operator=(BaseServer&&) = delete;
        virtual ~BaseServer() = default;
        virtual void Start() = 0;
        virtual void Stop() = 0;
    protected:
        virtual void AsyncAccept() = 0;
    };
} //namespace ProtosCloudServer::logging::impl