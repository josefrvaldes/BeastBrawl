#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <thread>

using boost::asio::ip::udp;
using namespace std;
using namespace std::chrono;

class UDPClient {
   public:
    UDPClient(string host, string port_);

    enum InputType {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        UP_LEFT,
        UP_RIGHT,
        DOWN_LEFT,
        DOWN_RIGHT
    };
    void SendInput(InputType input);
    void Send();

   private:
    void StartReceiving();
    void HandleReceive(const boost::system::error_code& error, size_t bytesTransferred);
    void HandleSendInput(InputType input, const boost::system::error_code& errorCode,
                         std::size_t bytes_transferred);
    void HandleSend(boost::shared_ptr<std::string> message,
                    const boost::system::error_code& errorCode,
                    std::size_t bytes_transferred);
    string GetTime() {
        auto time_point = system_clock::now();
        time_t now_c = system_clock::to_time_t(time_point);
        string salida = ctime(&now_c);
        salida = salida.substr(0, salida.size() - 1);
        return salida;
    }

    boost::asio::io_context context;
    udp::endpoint serverEndpoint;
    udp::socket socket;
    boost::array<char, 128> recvBuff;
    std::thread butler;
};