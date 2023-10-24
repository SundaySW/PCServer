#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "ProtosCloudServer/net/server.hpp"
#include "ProtosCloudServer/logging/impl/simple_logger.hpp"

using namespace ProtosCloudServer;
using namespace ProtosCloudServer::logging;

int main() {
    PServer::Server server = PServer::Server({});
    logging::impl::SetDefaultLogger(logging::MakeSimpleLogger());

    signal(SIGINT, [](int s) {
        LOG_INFO() << "SIGINT got" << "Server shutting down";
        logging::LogFlush();
        printf("SIGINT got\n");
        exit(0);
    });

    signal(SIGTERM, [](int s) {
        LOG_INFO() << "SIGTERM got" << "Server shutting down";
        logging::LogFlush();
        printf("SIGTERM got\n");
        exit(0);
    });

    try{
        server.Start();
        printf("Server started\n");
    }catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}