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

#include "../../include/boost/asio.hpp"
#include <memory>

#include <chrono>
#include <memory>
#include <iostream>
#include "Player.h"

using boost::asio::ip::tcp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class TCPServer;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
    public:
    ~TCPConnection();
    typedef std::shared_ptr<TCPConnection> pointer;
    static pointer Create(TCPServer *tcpServer_, boost::asio::io_context& io_context, std::vector<std::shared_ptr<Player>> &p, std::vector<TCPConnection::pointer>& connect){ return pointer(new TCPConnection(tcpServer_, io_context, p, connect)); }
    tcp::socket& socket(){ return socket_;}
    void Start();
    void Close();
    void SendStartMessage(string datos);
    void SendStartMessage(std::shared_ptr<unsigned char[]> buff, size_t buffSize);
    void SendFullGame();
    void SendOpenGame();
    void SendCharsSel(std::shared_ptr<unsigned char[]> buff, size_t buffSize);
    // Player *currentPlayer;

    uint16_t ID {nextID++};
    std::shared_ptr<Player> player;


   private:
    TCPConnection(TCPServer *tcpServer_, asio::io_context& io_context, std::vector<std::shared_ptr<Player>> &p, std::vector<TCPConnection::pointer>& connect);
    void HandleRead(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytes_transferred);
    void HandleWrite(const boost::system::error_code& error, size_t bytes_transferred);
    void DeleteMe();

    void HandleReceivedCatchChar(std::shared_ptr<unsigned char[]> recevBuff, const size_t currentBufferSize);
    void SendRequestSelChar(bool selected);
    void CancelChar();
    void HandleSendOpenGame(const boost::system::error_code& error, size_t bytes_transferred);

    inline static uint16_t nextID{0};
    
    string GetTime() {
        auto time_point = system_clock::now();
        time_t now_c = system_clock::to_time_t(time_point);
        string salida = ctime(&now_c);
        salida = salida.substr(0, salida.size() - 1);
        return salida;
    }


    TCPServer *tcpServer;
    tcp::socket socket_;

    std::vector<std::shared_ptr<Player>> &players;
    std::vector<TCPConnection::pointer>& connections;

    //uint16_t sendBuff;

};