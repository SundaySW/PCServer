#pragma once

#include <queue>

#include "moodycamel/concurrentqueue.h"

#include "boost/asio.hpp"

#include "export.h"
#include "ProtosCloudServer/net/http_session_handler.hpp"
#include "ProtosCloudServer/net/http/http_request.hpp"
#include "ProtosCloudServer/net/http/http_response.hpp"
#include "ProtosCloudServer/net/http_client_handler_base.hpp"

namespace ProtosCloudServer {

namespace net{

/**
 * @brief This class make default session for server client
 *
 * @ingroup net
 */
class PCS_API BoostAsioHttpSession : public HttpSessionHandler,
                                     public std::enable_shared_from_this<BoostAsioHttpSession>
{
public:
    using StringQueue = moodycamel::ConcurrentQueue<std::string>;

    explicit BoostAsioHttpSession(boost::asio::ip::tcp::socket &&socket);
    ~BoostAsioHttpSession() override;
    void Start(std::shared_ptr<HttpClientHandlerBase> clientHandler) override;
    void Post(const std::string &message) override;
    std::string GetAddr() override;
    unsigned short GetPort() override;
    void CloseSession() override;

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf stream_buf_ptr_;
    StringQueue outgoingQueue_;
    std::shared_ptr<HttpClientHandlerBase> clientHandler_;

    void AsyncWrite(std::string msg);
    void OnWrite(boost::system::error_code error, std::size_t bytes_transferred);
    void ReadHeader();
    void ReadBody(unsigned long long size, std::shared_ptr<http::HttpRequest>& request);
};
} //namespace net
} //namespace ProtosCloudServer