#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <chrono>
#include <iostream>

using boost::asio::ip::tcp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
    public:
    typedef boost::shared_ptr<TCPConnection> pointer;
    static pointer Create(boost::asio::io_context& io_context){ return pointer(new TCPConnection(io_context)); }
    tcp::socket& socket(){ return socket_;}
    void Start();
    void SendStartMessage(string datos);
    void SendStartMessage(unsigned char *buff, size_t buffSize);


   private:
    TCPConnection(asio::io_context& io_context);
    void HandleRead(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytes_transferred);
    void HandleWrite(const boost::system::error_code& error, size_t bytes_transferred);

    string GetTime() {
        auto time_point = system_clock::now();
        time_t now_c = system_clock::to_time_t(time_point);
        string salida = ctime(&now_c);
        salida = salida.substr(0, salida.size() - 1);
        return salida;
    }

    tcp::socket socket_;
    std::string message_;

    //uint16_t sendBuff;

};