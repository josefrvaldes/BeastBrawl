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
 */
 
 
 #pragma once

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "../Constants.h"

using boost::asio::ip::tcp;
using namespace std;
using namespace std::chrono;

class TCPClient {
    
   public:
    TCPClient(string host, uint16_t port_);
    ~TCPClient();
    void SendConnectionRequest();
    void SendDisconnectionRequest();
    void SendSelCharacterRequest();
    void SendCancelChar();
    
   private:
    void StartConnect(tcp::resolver::results_type::iterator endpoint_iter);
    void StartConnect(tcp::endpoint serverEndpoint);
    void HandleConnect(const boost::system::error_code& error, tcp::resolver::results_type::iterator endpoint_iter);
    void HandleConnect(const boost::system::error_code& error);
    void StartReceiving();

    void HandleReceived(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytesTransferred);
    void HandleReceivedStartGame(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred);
    void HandleReceivedFullGame();
    void HandleReceivedOpenGame(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred);
    void HandleReceivedCharReq(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred);
    void HandleReceivedCharSel(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred);


    void HandleSentConnectionRequest(std::shared_ptr<unsigned char[]> request, const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentCharacterRequest(std::shared_ptr<unsigned char[]> request, const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void Stop();

    boost::asio::io_context context;
    
    tcp::endpoint serverEndpoint;
    bool stopped;
    tcp::socket socket;
    std::string sendBuff;
    std::thread butler;
};