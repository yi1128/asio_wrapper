#include "tcp_server.hpp"
#include "assert_def.h"
#include "log.h"

#include <iostream>

using namespace asio_net;

const uint16_t PORT = 6666;

int main() {
    asio::io_context context;
    tcp_server server(context, PORT);
    server.on_session = [](const std::weak_ptr<tcp_session>& ws) {
        //LOG("on_session:");
        std::cout << "on_session: " << std::endl;
        auto session = ws.lock();
        session->on_close = [] {
            //LOG("session on_close:");
            std::cout << "session on_close: " << std::endl;
        };
        session->on_data = [ws](std::string data) {
            ASSERT(!ws.expired());
            //LOG("session on_data: %zu, %s", data.size(), data.c_str());
            std::cout << "session on_data : " << data.size() << ", " << data.c_str() << std::endl;
            ws.lock()->send(std::move(data));
        };
    };
    server.start(true);
    system("pause");
    return 0;
}