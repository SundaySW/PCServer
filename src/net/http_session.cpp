//
// Created by user on 16.08.2023.
//

#include "ProtosCloudServer/net/http_session.hpp"
#include "ProtosCloudServer/logging/log.hpp"

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;
using namespace ProtosCloudServer::logging;

namespace ProtosCloudServer::net {

    HttpSession::HttpSession(tcp::socket &&socket):
        socket_(std::move(socket)),
        parser_()
    {}

    std::string HttpSession::GetAddr(){
        error_code error;
        return socket_.remote_endpoint(error).address().to_string();
        if(error){}
    }

    unsigned short HttpSession::GetPort(){
        error_code error;
        return socket_.remote_endpoint(error).port();
        if(error){}
    }

    void HttpSession::Start(HttpSession::message_handler &&on_message, HttpSession::error_handler &&on_error)
    {
        this->on_message_ = std::move(on_message);
        this->on_error_ = std::move(on_error);
        ReadHeader();
    }

    void HttpSession::Post(const std::string& message){
        outgoingQueue_.enqueue(message);
        std::string msg;
        auto hasElem = outgoingQueue_.try_dequeue(msg);
        if (hasElem)
            AsyncWrite(msg);
    }

//    void HttpSession::AsyncRead() {
//        stream_buf_ptr_.prepare(1024);
//        io::async_read_until(
//            socket_,
//            stream_buf_ptr_,
//            "\r\n",
//            [self = shared_from_this()](auto &&error, auto &&bytes_transferred){
//                self->OnRead(
//                        std::forward<decltype(error)>(error),
//                        std::forward<decltype(bytes_transferred)>(bytes_transferred)
//                );
//            });
//    }

    void HttpSession::ReadHeader() {
        stream_buf_ptr_.prepare(1024);
        io::async_read_until(
            socket_,
            stream_buf_ptr_,
            "\r\n\r\n",
            [this, self = shared_from_this()](auto &&error, auto &&bytes_transferred){
                if(error){
                    if(!socket_.is_open()) return;
                    LOG(Level::kError) << "Error in HttpSession::ReadHeader - async read failure. Remote disconnected";
                    on_error_(error);
                    return;
                }
                io::streambuf::const_buffers_type bufs = stream_buf_ptr_.data();

                std::string dataAsString(boost::asio::buffers_begin(bufs),
                                         boost::asio::buffers_begin(bufs) + bytes_transferred);

                auto headers(std::make_shared<std::unordered_map<std::string,
                             std::string>>(self->parser_.parseHeader(dataAsString, true)));

                auto contentLengthIter = headers->find("Content-Length");
                unsigned long long size = contentLengthIter != headers->end() ?
                        std::stoull(contentLengthIter->second) : 0;

                if(size == 0){
                    auto answer = self->parser_.generateResponse("Bad request", "text/plain", 400,
                                                                 "Bad request", false);
                    Post(answer);
                    stream_buf_ptr_.consume(bytes_transferred);
                    ReadHeader();
                    return;
                }
                stream_buf_ptr_.consume(bytes_transferred);
                self->ReadBody(size, headers);
            });
    }

    void HttpSession::ReadBody(unsigned long long size,
                               std::shared_ptr<std::unordered_map<std::string, std::string>>& headers) {
        stream_buf_ptr_.prepare(size);
        boost::asio::async_read(
                socket_,
                stream_buf_ptr_,
                io::transfer_exactly(size - stream_buf_ptr_.size()),
                [this, self = shared_from_this(), headers](auto &&error, auto &&bytes_transferred){
                    if(error){
                        LOG(Level::kError) << "Error in HttpSession::ReadBody - async read failure" << error;
                        on_error_(error);
                        return;
                    }
                    io::streambuf::const_buffers_type bufs = stream_buf_ptr_.data();
                    std::string dataAsString(boost::asio::buffers_begin(bufs),
                                             boost::asio::buffers_begin(bufs) + bytes_transferred);

                    try {
                        on_message_(dataAsString, *headers);
                    } catch (std::exception& e) {
                        LOG(Level::kError) << "Error in HttpSession::ReadBody - passing message to handler " << error;
                        Post(self->parser_.generateResponse("Internal server error", "text/plain",
                                                            500, "Internal server error", false));
                    }
                    stream_buf_ptr_.consume(bytes_transferred);
                    ReadHeader();
                });
    }

//    void HttpSession::OnRead(error_code error, std::size_t bytes_transferred){
//        if (!error){
//            std::stringstream message;
//            message << socket_.remote_endpoint(error) << ": "
//                    << std::istream(&stream_buf_ptr_).rdbuf();
//            stream_buf_ptr_.consume(bytes_transferred);
//            on_message_(message.str(), {});
//            AsyncRead();
//        }else{
//            socket_.close(error);
//            on_error_(error);
//        }
//    }

    void HttpSession::AsyncWrite(std::string msg) {
        io::async_write(
                socket_,
                io::buffer(msg),
                [self = shared_from_this()](auto &&error, auto &&bytes_transferred){
                    if(error){
                        LOG(Level::kError) << "Error in HttpSession::AsyncWrite - async write failure" << error;
                        return;
                    }
                    self->OnWrite(
                            std::forward<decltype(error)>(error),
                            std::forward<decltype(bytes_transferred)>(bytes_transferred)
                    );
                });
    }

    void HttpSession::OnWrite(error_code error, std::size_t bytes_transferred){
        if (!error) {
            std::string msg;
            auto hasElem = outgoingQueue_.try_dequeue(msg);
            if (hasElem)
                AsyncWrite(msg);
        }else{
            socket_.close(error);
            LOG(Level::kError) << "Error on write to socket: " << error;
            on_error_(error);
        }
    }

    void HttpSession::CloseSession() {
        LOG(Level::kInfo) << "Closing session: " << GetAddr() << ":" << GetPort();
        error_code error;
        socket_.close(error);
        if(error){
            LOG(Level::kError) << "Error on closing socket: " << GetAddr() << ":" << GetPort();
            on_error_(error);
        }
    }

    HttpSession::~HttpSession() {
        CloseSession();
    }
}


