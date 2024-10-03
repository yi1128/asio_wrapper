



#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <thread>

#include "../asio_net/tcp_client.hpp"
#include "../asio_net/tcp_server.hpp"
#include "../asio_net/assert_def.h"
#include "../asio_net/log.h"

using namespace asio_net;

const uint16_t PORT = 6666;

int main(int argc, char** argv) {
    static uint32_t test_count_max = 100;
    static uint32_t test_count_expect = 0;
    if (argc >= 2) {
        test_count_max = std::strtol(argv[1], nullptr, 10);
    }

    // server
    static std::atomic_bool pass_flag_session_close{ false };
    std::thread([] {
        asio::io_context context;
        tcp_config server_config_;
        server_config_.auto_pack = true;

        tcp_server server(context, PORT, server_config_);

        server.on_session = [](const std::weak_ptr<tcp_session>& ws) {
            std::cout << "[server] on_session:" << std::endl;
            auto session = ws.lock();
            session->on_close = [] {
                std::cout << "[server] session on_close:" << std::endl;
                pass_flag_session_close = true;
            };
            session->on_data = [ws](std::string data) {
                ASSERT(!ws.expired());
#ifndef ASIO_NET_DISABLE_ON_DATA_PRINT
                LOG("[server] session on_data: %s", data.c_str());
#endif
                ws.lock()->send(std::move(data));
            };
        };
        server.start(true);
        }).detach();

        int num = 0;
        // client
        static std::atomic_bool pass_flag_client_close{ false };
        std::thread([&] {
            asio::io_context context;
            tcp_config client_config_;
            client_config_.auto_pack = true;
            tcp_client client(context, client_config_);
            client.on_open = [&] {
                std::cout << "[client] client on_open:" << std::endl;

                for (uint32_t i = 0; i < test_count_max; ++i) {
                    client.send(std::to_string(i));
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            };
            client.on_data = [&](const std::string& data) {
#ifndef ASIO_NET_DISABLE_ON_DATA_PRINT
                LOG("[client] client on_data: %s", data.c_str());
#endif


                ASSERT(std::to_string(test_count_expect++) == data);

                if (test_count_expect == test_count_max - 1) {
                    client.close();
                }

            };
            client.on_close = [&] {
                pass_flag_client_close = true;
                ASSERT(test_count_expect == test_count_max - 1);

                std::cout << "[client] client on_close:" << std::endl;
                client.stop();
            };

            client.open("localhost", PORT);
            client.run();
            }).join();

            ASSERT(pass_flag_session_close);
            ASSERT(pass_flag_client_close);
            return EXIT_SUCCESS;
}

/*
#include "stream_file.hpp"

#include "server_discovery.hpp"

#include <cstdio>
#include <iostream>

#include "log.h"

using namespace asio_net;

int main() {

    std::thread([] {
        asio::io_context context;
        server_discovery::receiver receiver(context, [](const std::string& name, const std::string& message) {
            LOG("receive: name: %s, message: %s", name.c_str(), message.c_str());
            });
        context.run();
        }).detach();

        std::thread([] {
            asio::io_context context;
            auto getFirstIp = [](asio::io_context& context) {
                using namespace asio::ip;
                tcp::resolver resolver(context);
                tcp::resolver::query query(host_name(), "");
                tcp::resolver::iterator iter = resolver.resolve(query);
                tcp::resolver::iterator end;
                if (iter != end) {
                    tcp::endpoint ep = *iter;
                    //std::cout << ep.address().to_v4().to_string() << std::endl;

                    return ep.address().to_string();
                }
                return std::string();
            };
            //asio::ip::host_name()
            server_discovery::sender sender_name(context, "host_name", "127.0.0.1");
            server_discovery::sender sender_ip(context, "ip", getFirstIp(context));
            context.run();
            }).detach();

            getchar();
            return 0;
}

*/

/*
struct test {
    int nt[32];
    std::string name;
    double db[32];
    float f_[32];
};

#include "data_utility_supporter.h"
int main()
{
    test *data = new test;
    data->name = "test";
    for (int i = 0; i < 32; i++) {
        data->nt[i] = i;
        data->db[i] = i * 0.00001;
        data->f_[i] = i * 0.01;
    }
    std::cout << "입력용" << std::endl;
    std::cout << "name : " << data->name << std::endl;
    for (int i = 0; i < 32; i++) {
        std::cout << "int : " << data->nt[i] << ", " << "double : " << data->db[i] << ", " << "float : " << data->f_[i] << std::endl;
    }


    char* check = new char[sizeof(test)];
    struct_to_char<test>(data, check);

    std::string str;
    char_to_string(check, sizeof(test), str);

    char* check2 = new char[sizeof(test)];
    string_to_char(str, sizeof(test), check2);

    test* output = new test;
    char_to_struct<test>(check2, output);


    std::string str2;
    conv_to_string<test>(data, sizeof(test), str2);

    //char* check2 = new char[sizeof(test)];
    //string_to_char(str2, sizeof(test), check2);

    test* output = new test;
    conv_to_struct(std::move(str2), sizeof(test), output);
    //char_to_struct<test>(check2, output);

    std::cout << "확인용" << std::endl;
    std::cout << "name : " << output->name << std::endl;
    for (int i = 0; i < 32; i++) {
        std::cout << "int : " << output->nt[i] << ", " << "double : " << output->db[i] << ", " << "float : " << output->f_[i] << std::endl;
    }

    return 0;
}

*/