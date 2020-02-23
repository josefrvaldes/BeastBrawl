#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unordered_map>
#include <boost/asio/buffer.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "../../include/glm/vec3.hpp"
#include "../Components/CPowerUp.h"
#include "../Constants.h"

using boost::asio::ip::udp;
using namespace std;
using namespace std::chrono;

class UDPClient {
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
    UDPClient(string host, uint16_t port_);
    ~UDPClient();

    void SendInputs(const vector<Constants::InputTypes>& inputs, uint16_t id);
    void SendSync(uint16_t idOnline, const glm::vec3& posCar, const glm::vec3& rotCar, typeCPowerUp tipoPU, bool haveTotem, int64_t totemTime,
                  bool totemInGround, const glm::vec3& posTotem);
    void SendDateTime();
    uint32_t idMainCar;

   private:
    void StartReceiving();
    void HandleReceived(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytesTransferred);
    void HandleReceivedInputs(const vector<Constants::InputTypes> inputs, const uint16_t idRival) const;
    void HandleReceivedSync(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleSentInputs(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentSync(const boost::system::error_code& errorCode, std::size_t bytes_transferred);

    void HandleSentDateTime(const boost::shared_ptr<std::string> message,
                            const boost::system::error_code& errorCode,
                            std::size_t bytes_transferred);

    boost::asio::io_context context;
    udp::endpoint serverEndpoint;
    udp::socket socket;
    std::thread butler;
    // boost::asio::io_context::strand strand;

    unordered_map<uint16_t, int64_t> lastTimeInputReceived;
    unordered_map<uint16_t, int64_t> lastTimeSyncReceived;

};