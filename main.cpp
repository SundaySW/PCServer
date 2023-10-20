#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "ProtosCloudServer/net/server.hpp"

using namespace ProtosCloudServer;

int main() {
//    ProtosServer server(boost::asio::ip::tcp::v4(), 15001);
    PServer::Server server = PServer::Server({});
    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
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