/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Antonio Jose Martinez Garcia
 * @author Jose Valdés Sirvent
 * 
 * 
 */

#pragma once

#include "../../include/boost/array.hpp"
#include "../../include/boost/asio.hpp"
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
    void SendStartGame();
    void SendCharsSelected();
    void SendCharsSelectedToOther(uint16_t idConnection);

   private:
    
    void HandleAccept(TCPConnection::pointer new_connection, const boost::system::error_code& error);
    bool PlayerExists(TCPConnection::pointer new_connection);

    boost::asio::io_context& context;
    tcp::acceptor acceptor_;
    UDPServer &udpServer;

    vector<TCPConnection::pointer> connections;
    std::vector<std::shared_ptr<Player>> players;
};