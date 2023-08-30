#ifndef PROTOSCLOUDSERVER_ASIO_SERVER_HPP
#define PROTOSCLOUDSERVER_ASIO_SERVER_HPP

#include <optional>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <utility>

#include "ProtosCloudServer/types/runtime_error.hpp"
#include "http_session.hpp"

namespace ProtosCloudServer {

/**
 * @brief This class make core TCP ProtosServer using Boost Asio
 *
 * @ingroup net
 */

template<typename Protocol, typename EventHandler>
class PCS_API AsioServer{
public:
    AsioServer(const typename boost::asio::basic_socket_acceptor<Protocol>::endpoint_type& endpoint,
               EventHandler handler)
        :io_service_()
        ,acceptor_(io_service_, endpoint)
        ,socket_(io_service_)
        ,handler_(handler)
    {}
    /**
     * @brief Start receiving new connections.
     */
    void Start(){
        AsyncAccept();
        io_service_.run();
    }
    /**
    * @brief Stop receiving new connections.
    */
    void Stop(){
        io_service_.stop();
    }
protected:
    void AsyncAccept(){
        socket_.emplace(io_service_);
        acceptor_.async_accept(*socket_, [&](boost::system::error_code error){
            handler_(std::make_shared<HttpSession>(std::move(*socket_)));
            AsyncAccept();
        });
    }
private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::optional<boost::asio::ip::tcp::socket> socket_;
    EventHandler handler_;
};
}

#endif //PROTOSCLOUDSERVER_ASIO_SERVER_HPP