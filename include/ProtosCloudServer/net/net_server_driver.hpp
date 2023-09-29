#pragma once

#include <memory>

#include "ProtosCloudServer/net/impl/server_base.hpp"
#include "ProtosCloudServer/net/server_config.hpp"

namespace ProtosCloudServer::net{

class NetServer final {
public:
    explicit NetServer(const ServerConfig& config)
        :pimpl(std::make_unique<ServerImpl>(std::move(config)))
    {};

    void AddMsgHandler();

private:
    std::unique_ptr<ServerImpl> pimpl;
};

} //namespace ProtosCloudServer::net