//
// Created by user on 17.08.2023.
//

#ifndef PROTOSCLOUDSERVER_PROTOSSERVER_HPP
#define PROTOSCLOUDSERVER_PROTOSSERVER_HPP

#include <utility>

#include "export.h"
#include "ProtosCloudServer/net/AsioServer.hpp"
#include "ProtosCloudServer/net/HttpServer.hpp"
#include "exception"

namespace ProtosCloudServer{

/**
 * @brief This object holds other objects specific for this server instance.
 *
 * @ingroup general
 */
template<typename ServerBaseProtocol>
class PROTOSCLOUDSERVER_API ProtosServer {
public:
    using EventHandler = std::function<std::string (const std::string&, const std::unordered_map<std::string, std::string>&)>;
    using HttpServer = HttpServer<ServerBaseProtocol, EventHandler>;

    explicit ProtosServer(ServerBaseProtocol&& ver, std::size_t port)
        :server_(typename ServerBaseProtocol::endpoint(ver, port), &handlerFunc)
    {
    }

    static std::string handlerFunc(const std::string& dataString, const std::unordered_map<std::string, std::string>& headers){
        return {};
    }


    void start(){
        server_.StartServer();
    }

private:
    HttpServer server_;
};

}

#endif //PROTOSCLOUDSERVER_PROTOSSERVER_HPP
