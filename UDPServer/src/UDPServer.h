#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class UDPServer {
   public:
    UDPServer(asio::io_context& context_, uint16_t port_);

   private:
    void SaveClientIfNotExists(udp::endpoint& endpoint);
    void StartReceiving();
    void HandleReceive(udp::endpoint& remoteEndpoint, const boost::system::error_code& error, size_t bytesTransferred);
    void HandleReceiveInput(int8_t inputType, udp::endpoint& remoteClient);
    void HandleReceiveDateTimeRequest(udp::endpoint& remoteClient);
    void HandleSend(boost::shared_ptr<string> message,
                    const boost::system::error_code& errorCode,
                    std::size_t bytes_transferred);
    string GetTime() {
        auto time_point = system_clock::now();
        time_t now_c = system_clock::to_time_t(time_point);
        string salida = ctime(&now_c);
        salida = salida.substr(0, salida.size() - 1);
        return salida;
    }

    udp::socket socket;
    udp::endpoint receiverEndpoint;
    std::vector<udp::endpoint> clients;
    boost::array<uint8_t, 2> recvBuff;
};