#include "tcp_client.hpp"
#include "log.h"

using namespace asio_net;

const uint16_t PORT = 6666;

int main() {
    asio::io_context context;
    tcp_client client(context);
    client.on_open = [&] {
        //LOG("client on_open:");
        std::cout << "client on_open:" << std::endl;
        client.send("hello");
    };
    client.on_data = [](const std::string& data) {
        //LOG("client on_data: %s", data.c_str());
        std::cout << "client on_data: " << data.c_str() << std::endl;
    };
    client.on_open_failed = [](std::error_code ec) {
        //LOG("client on_open_failed: %d, %s", ec.value(), ec.message().c_str());
        std::cout << "client on_open_failed: " << ec.value() << ", " << ec.message().c_str() << std::endl;
    };
    client.on_close = [] {
        //LOG("client on_close:");
        std::cout << "client on_close:" << std::endl;
    };
    client.open("localhost", PORT);
    client.run();
    return 0;
}