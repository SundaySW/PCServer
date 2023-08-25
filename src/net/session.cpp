//
// Created by user on 16.08.2023.
//

#include "ProtosCloudServer/net/session.hpp"

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

namespace ProtosCloudServer {
    Session::Session(tcp::socket &&socket):
            socket_(std::move(socket))
    {}

    std::string Session::GetAddr(){
        error_code error;
        return socket_.remote_endpoint(error).address().to_string();
        if(error){}
    }

    void Session::Start(Session::message_handler &&on_message, Session::error_handler &&on_error)
    {
        this->on_message_ = std::move(on_message);
        this->on_error_ = std::move(on_error);
        AsyncRead();
    }

    void Session::Post(const std::string &message){
        outgoingQueue_.push(message);
        if (!outgoingQueue_.empty())
            AsyncWrite();
    }

    void Session::AsyncRead() {
        stream_buf_ptr_.prepare(1024);
        io::async_read_until(
            socket_,
            stream_buf_ptr_,
            "\r\n",
            [self = shared_from_this()](auto &&error, auto &&bytes_transferred){
                self->OnRead(
                        std::forward<decltype(error)>(error),
                        std::forward<decltype(bytes_transferred)>(bytes_transferred)
                );
            });
    }

    void Session::ReadHeader() {
        stream_buf_ptr_.prepare(1024);
        io::async_read_until(
                socket_,
                stream_buf_ptr_,
                "\r\n\r\n",
                [this, self = shared_from_this()](auto &&error, auto &&bytes_transferred){
                    if(error)
                        on_error_(error);

                    io::streambuf::const_buffers_type bufs = stream_buf_ptr_.data();
                    std::string dataAsString(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + bytes_transferred);
                    stream_buf_ptr_.consume(bytes_transferred);
                    self->ReadBody(dataAsString);
                });
    }

    void Session::ReadBody(std::string& headers) {
        stream_buf_ptr_.prepare(stream_buf_ptr_.max_size());
        boost::asio::async_read(
                socket_,
                stream_buf_ptr_,
                boost::asio::transfer_at_least(1),
                    [this, self = shared_from_this(), headers](auto &&error, auto &&bytes_transferred){
                        if(error)
                            on_error_(error);
                            io::streambuf::const_buffers_type bufs = stream_buf_ptr_.data();
                            std::string dataAsString(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + bytes_transferred);
                            stream_buf_ptr_.consume(bytes_transferred);
                            on_message_(headers, std::move(dataAsString));
                        });
    }

    void Session::OnRead(error_code error, std::size_t bytes_transferred) {
        if (!error) {
            std::stringstream message;
            message << socket_.remote_endpoint(error) << ": "
                    << std::istream(&stream_buf_ptr_).rdbuf();
            stream_buf_ptr_.consume(bytes_transferred);
            on_message_(message.str(), {});
            AsyncRead();
        } else {
            socket_.close(error);
            on_error_(error);
        }
    }

    void Session::AsyncWrite() {
        io::async_write(
                socket_,
                io::buffer(outgoingQueue_.front()),
                [self = shared_from_this()](auto &&error, auto &&bytes_transferred){
                    self->OnWrite(
                            std::forward<decltype(error)>(error),
                            std::forward<decltype(bytes_transferred)>(bytes_transferred)
                    );
                });
    }

    void Session::OnWrite(error_code error, std::size_t bytes_transferred){
        if (!error) {
            outgoingQueue_.pop();
            if (!outgoingQueue_.empty())
                AsyncWrite();
        } else {
            socket_.close(error);
            on_error_(error);
        }
    }

    void Session::CloseSession() {
        error_code error;
        socket_.close(error);
        if(error){}
    }
}


