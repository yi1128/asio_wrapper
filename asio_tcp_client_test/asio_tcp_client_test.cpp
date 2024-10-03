#include "tcp_client.hpp"
#include "log.h"

using namespace asio_net;

const uint16_t PORT = 6666;

int main() {

    asio::io_context context;
    tcp_client client(context);
    bool client_setup_complete = false;
    std::thread([&]() {
        client.on_open = [&] {
            //LOG("client on_open:");
            std::cout << "client on_open:" << std::endl;
            client.send("hello");
            client_setup_complete = true;
        };
        client.on_data = [](const std::string& data) {
            //LOG("client on_data: %s", data.c_str());
            std::cout << "client on_data: " << data.c_str() << std::endl;
        };
        client.on_open_failed = [](std::error_code ec) {
            //LOG("client on_open_failed: %d, %s", ec.value(), ec.message().c_str());
            std::cout << "client on_open_failed: " << ec.value() << ", " << ec.message().c_str() << std::endl;
        };

        client.on_close = [&] {
            //LOG("client on_close:");
            std::cout << "client on_close:" << std::endl;
            client_setup_complete = false;
        };

        client.set_reconnect(1);
        client.open("192.168.0.4", PORT);
        client.run();
        }).detach();


        std::thread([&]() {
            int num = 0;
            std::string test = "check";
            while (true) {
                if (client_setup_complete)
                    client.send(test + std::to_string(num++));
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            }).join();



            /*
            asio::io_context context;
            tcp_config config_;
            config_.auto_pack = true;
            tcp_client client(context, config_);
            std::thread([&] {
                client.on_open = [&] {
                    //LOG("client on_open:");
                    std::cout << "client on_open:" << std::endl;
                    std::string msg("hello");
                    client.send(msg);
                };
                client.on_data = [](const std::string& data) {
                    (void)data;
                    //LOG("client on_data:");
                    std::cout << "client on_data: " << data.c_str() << std::endl;
                };
                client.on_close = [&] {
                    //LOG("client on_close:");
                    std::cout << "client on_close:" << std::endl;
                    static int count = 0;
                    if (++count == 3) {
                        client.stop();
                    }
                };
                client.set_reconnect(1);
                client.open("localhost", PORT);
                client.run();
                }).detach();

                std::thread([&] {
                    client.send("hihihihi");
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }).join();
            */
            return 0;
}