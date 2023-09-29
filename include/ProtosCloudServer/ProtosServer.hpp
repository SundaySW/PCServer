#pragma once

#include <utility>
#include "export.h"
#include "exception"

#include "ProtosCloudServer/net/net_server_driver.hpp"

namespace ProtosCloudServer{

/**
 * @brief This object holds other objects specific for this server instance.
 *
 * @ingroup general
 */

class PCS_API ProtosServer {
public:
    explicit ProtosServer(net::NetServer&& netServer)
        :server_(netServer)
    {
    }

    std::string ClientMsgHandler(const std::string& dataString,
                                        const std::unordered_map<std::string, std::string>& headers){
        return {};
    }

    void Start(){
        server_.StartServer();
    }

    void Stop(){
        server_.StopServer();
    }
private:
    NetServer server_;
};

} //namespace ProtosCloudServer