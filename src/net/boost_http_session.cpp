
#include "PCServer/net/boost_http_session.hpp"
#include "PCServer/logging/log.hpp"

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;
using namespace PCServer::logging;
using namespace PCServer::http;

namespace PCServer::net {

    BoostHttpSession::BoostHttpSession(tcp::socket&& socket):
        socket_(std::move(socket)),
        pool_(4)
    {}

    std::string BoostHttpSession::GetAddr(){
        error_code error;
        return socket_.remote_endpoint(error).address().to_string();
        if(error){}
    }

    unsigned short BoostHttpSession::GetPort(){
        error_code error;
        return socket_.remote_endpoint(error).port();
        if(error){}
    }

    void BoostHttpSession::Start(http::HttpHandler::CallBackT handler){
        handler_callback_ = std::move(handler);
        LOG_INFO() << "New session: " << GetAddr() <<":"<< GetPort();
        ReadHeader();
    }

    void BoostHttpSession::Post(HttpResponse& response){
        outgoingQueue_.enqueue(response.toString());
        std::string msg;
        auto hasElem = outgoingQueue_.try_dequeue(msg);
        if(hasElem)
            AsyncWrite(msg);
    }

    void BoostHttpSession::Post(std::string& msg){
        outgoingQueue_.enqueue(msg);
        std::string outgoingMsg;
        auto hasElem = outgoingQueue_.try_dequeue(outgoingMsg);
        if(hasElem)
            AsyncWrite(outgoingMsg);
    }

    void BoostHttpSession::ReadHeader() {
        stream_buf_ptr_.prepare(http::HttpRequest::kHeaderSize);
        io::async_read_until(
            socket_,
            stream_buf_ptr_,
            "\r\n\r\n",
            [this](auto &&error, auto &&bytes_transferred){
                if(error){
                    LOG_ERROR() << fmt::format("Remote {}:{} disconnected", GetAddr(), GetPort());
                    return;
                }
                io::streambuf::const_buffers_type bufs = stream_buf_ptr_.data();

                std::string dataAsString(boost::asio::buffers_begin(bufs),
                                         boost::asio::buffers_begin(bufs) + bytes_transferred);

                auto request = std::make_shared<HttpRequest>(dataAsString, fmt::format("{}:{}",GetAddr(),GetPort()));
                auto size = request->GetSize();
                if(request->IsEmptyRequest()){
                    auto response = HttpResponse(http::HttpStatus::kBadRequest);
                    Post(response);
                    stream_buf_ptr_.consume(bytes_transferred);
                    ReadHeader();
                    return;
                }
                stream_buf_ptr_.consume(bytes_transferred);
                ReadBody(size, std::move(request));
            });
    }

    void BoostHttpSession::ReadBody(unsigned long long size, std::shared_ptr<HttpRequest>&& request){
        stream_buf_ptr_.prepare(size);
        boost::asio::async_read(
            socket_,
            stream_buf_ptr_,
            io::transfer_exactly(size - stream_buf_ptr_.size()),
            [this, request] (auto &&error, auto &&bytes_transferred){
                if(error){
                    LOG_ERROR() << "Error in BoostHttpSession::ReadBody - async read failure" << error;
                    return;
                }
                io::streambuf::const_buffers_type bufs = stream_buf_ptr_.data();

                boost::asio::post(pool_, [this, request, bufs, bytes_transferred]{
                    std::string dataAsString(boost::asio::buffers_begin(bufs),
                                             boost::asio::buffers_begin(bufs) + bytes_transferred);
                    request->StoreData(dataAsString);
                    try{
                        handler_callback_(*request);
                    }catch (std::exception& e) {
                        LOG_ERROR() << "Error in BoostHttpSession::ReadBody - passing message to handler ";
                        auto response = HttpResponse(http::HttpStatus::kInternalServerError, e.what());
                        Post(response);
                    }
                });

                stream_buf_ptr_.consume(bytes_transferred);
                ReadHeader();
            });
    }

    void BoostHttpSession::AsyncWrite(std::string msg) {
        io::async_write(
                socket_,
                io::buffer(msg),
                [this](auto &&error, auto &&bytes_transferred){
                    if(error){
                        LOG_ERROR() << "Error in BoostHttpSession::AsyncWrite - async write failure" << error;
                        return;
                    }
                    OnWrite(
                            std::forward<decltype(error)>(error),
                            std::forward<decltype(bytes_transferred)>(bytes_transferred)
                    );
                });
    }

    void BoostHttpSession::OnWrite(error_code error, std::size_t bytes_transferred){
        if (!error) {
            std::string msg;
            auto hasElem = outgoingQueue_.try_dequeue(msg);
            if(hasElem)
                AsyncWrite(msg);
        }else{
            socket_.close(error);
            LOG_ERROR() << "Error on write to socket: " << error;
        }
    }

    void BoostHttpSession::CloseSession() {
        LOG_INFO() << "Closing session: " << GetAddr() << ":" << GetPort();
        error_code error;
        socket_.close(error);
        if(error){
            LOG_ERROR() << "Error on closing socket: " << GetAddr() << ":" << GetPort();
        }
    }

    BoostHttpSession::~BoostHttpSession(){
        pool_.join();
        CloseSession();
    }
}


