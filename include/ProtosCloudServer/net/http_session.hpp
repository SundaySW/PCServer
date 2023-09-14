#ifndef PROTOSCLOUDSERVER_HTTP_SESSION_HPP
#define PROTOSCLOUDSERVER_HTTP_SESSION_HPP

#include <queue>
#include "moodycamel/concurrentqueue.h"

#include "boost/asio.hpp"

#include "export.h"
#include "ProtosCloudServer/net/http_parser.h"

namespace ProtosCloudServer {

namespace net{

/**
 * @brief This class make default session for server client
 *
 * @ingroup net
 */
class PCS_API HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    using StringQueue = moodycamel::ConcurrentQueue<std::string>;
    using message_handler = std::function<void(const std::string&, const std::unordered_map<std::string, std::string>&)>;
    using error_handler = std::function<void(boost::system::error_code error)>;

    explicit HttpSession(boost::asio::ip::tcp::socket &&socket);
    ~HttpSession();
    void Start(message_handler &&on_message, error_handler &&on_error);
    void Post(const std::string &message);
    std::string GetAddr();
    unsigned short  GetPort();
    void CloseSession();

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf stream_buf_ptr_;
    HttpParser parser_;
    StringQueue outgoingQueue_;
    message_handler on_message_;
    error_handler on_error_;

    void AsyncRead();
    void AsyncWrite(std::string msg);
    void OnRead(boost::system::error_code error, std::size_t bytes_transferred);
    void OnWrite(boost::system::error_code error, std::size_t bytes_transferred);
    void ReadHeader();
    void ReadBody(unsigned long long int size, std::shared_ptr<std::unordered_map<std::string, std::string>> &headers);
};
} //namespace net
} //namespace ProtosCloudServer
#endif //ASIOSERVER_SESSION_HPP
