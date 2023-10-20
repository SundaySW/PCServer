#pragma once

#include <utility>
#include "export.h"
#include "exception"

#include "ProtosCloudServer/net/server.hpp"

namespace ProtosCloudServer{

/**
 * @brief This object holds other objects specific for this server instance.
 *
 * @ingroup general
 */

class PCS_API ProtosServer {
//public:
//    explicit ProtosServer(net::NetServer&& netServer)
//        :server_(netServer)
//    {
//    }
//
//private:
//    NetServer server_;
};

} //namespace ProtosCloudServer