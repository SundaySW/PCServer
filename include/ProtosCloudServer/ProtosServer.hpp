#ifndef PROTOSCLOUDSERVER_PROTOSSERVER_HPP
#define PROTOSCLOUDSERVER_PROTOSSERVER_HPP

#include <utility>

#include "export.h"
#include "ProtosCloudServer/net/asio_server.hpp"
#include "ProtosCloudServer/net/http_server.hpp"
#include "exception"

namespace ProtosCloudServer{

/**
 * @brief This object holds other objects specific for this server instance.
 *
 * @ingroup general
 */
template<typename ServerBaseProtocol>
class PCS_API ProtosServer {
public:
    using HttpMsgHandler = std::function<std::string(const std::string&,
                                                     const std::unordered_map<std::string, std::string>&)>;
    using Server = net::HttpServer<ServerBaseProtocol, HttpMsgHandler>;

    explicit ProtosServer(ServerBaseProtocol&& ver, std::size_t port)
        :server_(typename ServerBaseProtocol::endpoint(ver, port),
                 [this](auto&& _1, auto&& _2) { return ClientMsgHandler(std::forward<decltype(_1)>(_1),
                                                                        std::forward<decltype(_2)>(_2));})
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
        server_.StartServer();
    }
private:
    Server server_;
};

} //namespace ProtosCloudServer

#endif //PROTOSCLOUDSERVER_PROTOSSERVER_HPP