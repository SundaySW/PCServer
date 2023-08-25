#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include "ProtosCloudServer/ProtosServer.hpp"

using namespace ProtosCloudServer;

int main() {
    ProtosServer server(boost::asio::ip::tcp::v4(), 15001);
    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });
    try{
        printf("Server started\n");
        server.Start();
    }catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}