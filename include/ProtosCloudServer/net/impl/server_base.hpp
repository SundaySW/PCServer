
#ifndef PROTOSCLOUDSERVER_SERVER_BASE_HPP
#define PROTOSCLOUDSERVER_SERVER_BASE_HPP

namespace ProtosCloudServer::net::impl {
/// Base server class
    class BaseServer{
    public:
        BaseServer(const BaseServer&) = delete;
        BaseServer(BaseServer&&) = delete;
        BaseServer& operator=(const BaseServer&) = delete;
        BaseServer& operator=(BaseServer&&) = delete;
        virtual ~BaseServer();
        virtual void Start() = 0;
        virtual void Stop() = 0;
    protected:
        virtual void AsyncAccept() = 0;
    };

} //namespace ProtosCloudServer::logging::impl

#endif //PROTOSCLOUDSERVER_SERVER_BASE_HPP
