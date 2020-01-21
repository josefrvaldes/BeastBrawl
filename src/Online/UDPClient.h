#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "../Constants.h"

using boost::asio::ip::udp;
using namespace std;
using namespace std::chrono;

class UDPClient {
   public:
    UDPClient(string host, string port_);

    void SendInput(Constants::InputTypes input);
    void SendInputs(vector<Constants::InputTypes>& inputs);
    void SendDateTime();

   private:
    void StartReceiving();
    void HandleReceived(const boost::system::error_code& error, size_t bytesTransferred);

    void HandleSentInput(std::shared_ptr<Constants::InputTypes> input, const boost::system::error_code& errorCode,
                         std::size_t bytes_transferred);
    void HandleSentInputs(const vector<Constants::InputTypes>& inputs, const boost::system::error_code& errorCode,
                          std::size_t bytes_transferred);
    void HandleSentDateTime(const boost::shared_ptr<std::string> message,
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
    // boost::array<char, 128> recvBuff;
    boost::array<string, 1> sendBuff;
    vector<boost::asio::mutable_buffer> recvBuff;
    std::thread butler;
};