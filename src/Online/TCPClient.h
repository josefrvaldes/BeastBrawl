#pragma once

#include <boost/array.hpp>
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
    // --- TCP --- (sala de espera)
    // .- enviamos al servidor un mensaje de que queremos conectar
    // .- esperamos hasta recibir respuesta de que empieza la partida.
    //      Este mensaje traerá información de el estado inicial de la partida
    //      con el numero de spawn del todos los coches, de esta forma ya sabemos
    //      dónde debemos posicionar todos los coches de inicio

    // --- UDP --- (juego)
    // .- cambiamos de estado a StateInGameMulti
    // .- inicializamos la partida con la info que nos ha llegado del server
    // .- en versión futura se puede recibir una hora de inicio y
    //       ese es el momento en el que empieza
    // .- Empieza la partida, y le enviaremos paquetes de input al servidor y
    //      él se encargará de registrar nuestro endpoint udp

   public:
    TCPClient(string host, uint16_t port_);
    ~TCPClient();
    void SendConnectionRequest();
    void SendDisconnectionRequest();
    
   private:
    void StartConnect(tcp::resolver::results_type::iterator endpoint_iter);
    void StartConnect(tcp::endpoint serverEndpoint);
    void HandleConnect(const boost::system::error_code& error, tcp::resolver::results_type::iterator endpoint_iter);
    void HandleConnect(const boost::system::error_code& error);
    void StartReceiving();

    void HandleReceived(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytesTransferred);
    void HandleReceivedStartGame(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred);
    void HandleReceivedFullGame();

    void HandleSentConnectionRequest(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void Stop();

    boost::asio::io_context context;
    
    // tcp::resolver::results_type endpoints;
    tcp::endpoint serverEndpoint;
    bool stopped;
    tcp::socket socket;
    // boost::array<char, 128> recvBuff;
    std::string sendBuff;
    //vector<boost::asio::mutable_buffer> recvBuff;
    std::thread butler;
    //boost::array<boost::asio::const_buffer,1> sendBuff2;
    
};