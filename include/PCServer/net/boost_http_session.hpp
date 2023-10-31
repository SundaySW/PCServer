#pragma once

#include <queue>

#include "moodycamel/concurrentqueue.h"

#include "boost/asio.hpp"

#include "export.h"
#include "PCServer/net/impl/http_session_handler.hpp"
#include "PCServer/net/http/http_request.hpp"
#include "PCServer/net/http/http_response.hpp"
#include "PCServer/net/http_handler.hpp"

namespace PCServer {

namespace net{

/**
 * @brief This class make default session for server client
 *
 * @ingroup net
 */
class PCS_API BoostHttpSession : public HttpSessionHandler,
                                 public std::enable_shared_from_this<BoostHttpSession>
{
public:
    using StringQueue = moodycamel::ConcurrentQueue<std::string>;

    explicit BoostHttpSession(boost::asio::ip::tcp::socket&& socket);
    ~BoostHttpSession() override;
    void Start(http::HttpHandler::CallBackT) override;
    void Post(http::HttpResponse& response) override;
    void Post(std::string& msg) override;
    std::string GetAddr() override;
    unsigned short GetPort() override;
    void CloseSession() override;

private:
    void AsyncWrite(std::string msg);
    void OnWrite(boost::system::error_code error, std::size_t bytes_transferred);
    void ReadHeader();
    void ReadBody(unsigned long long size, std::shared_ptr<http::HttpRequest>&& request);

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf stream_buf_ptr_;
    StringQueue outgoingQueue_;
    http::HttpHandler::CallBackT handler_callback_;
};

} //namespace net
} //namespace PCServer