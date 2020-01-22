#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <chrono>
#include <iostream>
#include "TCPConnection.h"

using boost::asio::ip::tcp;
using namespace boost;
using namespace std;
using namespace std::chrono;


class TCPServer{
   public:
    TCPServer(boost::asio::io_context& context_, uint16_t port_);
    void StartReceiving();

   private:
    
    void HandleAccept(TCPConnection::pointer new_connection, const boost::system::error_code& error);

    boost::asio::io_context& context;
    tcp::acceptor acceptor_;
};