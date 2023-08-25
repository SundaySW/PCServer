#ifndef PROTOSCLOUDSERVER_SESSION_HPP
#define PROTOSCLOUDSERVER_SESSION_HPP

#include "export.h"
#include "boost/asio.hpp"
#include "queue"
#include "httpParser.h"

namespace ProtosCloudServer {
/**
 * @brief This class make default session for server client
 *
 * @ingroup net
 */
class PROTOSCLOUDSERVER_API Session : public std::enable_shared_from_this<Session> {
public:
    using message_handler = std::function<void(std::string, std::string)>;
    using error_handler = std::function<void(boost::system::error_code error)>;

    Session(boost::asio::ip::tcp::socket &&socket);
    void Start(message_handler &&on_message, error_handler &&on_error);
    void Post(const std::string &message);
    std::string GetAddr();
    void CloseSession();

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf stream_buf_ptr_;
    std::queue<std::string> outgoingQueue_;
    message_handler on_message_;
    error_handler on_error_;

    void AsyncRead();
    void OnRead(boost::system::error_code error, std::size_t bytes_transferred);
    void AsyncWrite();
    void OnWrite(boost::system::error_code error, std::size_t bytes_transferred);

    void ReadHeader();

    void ReadBody(std::string &headers);
};
}

#endif //ASIOSERVER_SESSION_HPP
