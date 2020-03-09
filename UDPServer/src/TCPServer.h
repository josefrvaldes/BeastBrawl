#pragma once

#include "../../include/boost/array.hpp"
#include "../../include/boost/asio.hpp"
#include "../../include/boost/enable_shared_from_this.hpp"
#include <chrono>
#include <iostream>
#include "TCPConnection.h"
#include "Player.h"

using boost::asio::ip::tcp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class UDPServer;

class TCPServer{
   public:
    TCPServer(boost::asio::io_context& context_, uint16_t port_, UDPServer &udpServer_);
    ~TCPServer();
    void StartReceiving();
    void Close();

   private:
    
    void HandleAccept(TCPConnection::pointer new_connection, const boost::system::error_code& error);
    bool PlayerExists(TCPConnection::pointer new_connection);
    void SendStartGame();

    boost::asio::io_context& context;
    tcp::acceptor acceptor_;
    UDPServer &udpServer;

    vector<TCPConnection::pointer> connections;
    std::vector<Player> players;
};