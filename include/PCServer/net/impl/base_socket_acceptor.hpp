#pragma once

#include "PCServer/net/server_config.hpp"

namespace PCServer::net::impl {
/// Base server class
    class BaseSocketAcceptor{
    public:
        BaseSocketAcceptor(const BaseSocketAcceptor&) = delete;
        BaseSocketAcceptor(BaseSocketAcceptor&&) = delete;
        BaseSocketAcceptor& operator=(const BaseSocketAcceptor&) = delete;
        BaseSocketAcceptor& operator=(BaseSocketAcceptor&&) = delete;
        virtual ~BaseSocketAcceptor() = default;
        explicit BaseSocketAcceptor() noexcept = default;
        virtual void StartAccept() = 0;
        virtual void StopAccept() = 0;
    protected:
        virtual void AsyncAccept() = 0;
    };
} //namespace PCServer::logging::impl