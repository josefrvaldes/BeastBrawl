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

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_map>
#include "../../include/glm/vec3.hpp"
#include "../Components/CPowerUp.h"
#include "../Constants.h"

using boost::asio::ip::udp;
using namespace std;
using namespace std::chrono;

class PowerUp;

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

    void SendInputs(const int64_t gameTime, const vector<Constants::InputTypes>& inputs, uint16_t id, float speed, float wheelRotation, float skidDeg, float skidRotation);
    void SendSync(const int64_t gameTime, uint16_t idOnline, const glm::vec3& posCar, const glm::vec3& rotCar, float speed, float wheelRotation, float skidDeg, float skidRotation, typeCPowerUp typePU, bool haveTotem, int64_t totemTime,
                  bool totemInGround, const glm::vec3& posTotem);
    void SendCatchPU(uint16_t idOnline, typeCPowerUp typePU);
    void SendClockSync(uint16_t idOnline1, uint16_t idOnline2, int64_t time, float turnOut, uint8_t numMeasurements);
    void SendFinalClockSync(uint16_t idOnlineSender, uint16_t idOnlineReceiver, float turnout, int64_t timeToWaitForSyncing);
    void SendCatchTotem(uint16_t idOnline, uint16_t idPlayerCatched);
    void SendCrashPUCar(const uint16_t idOnline, const uint16_t idPowerUp, const uint16_t idCar);
    void SendCrashPUWall(const uint16_t idOnline, const uint16_t idPowerUp);
    void SendLostTotem(uint16_t idOnline, uint16_t idPlayerLosted, const glm::vec3& pos, float speed, uint16_t rotationTotemY, int numNavMesh);
    void SendRoboJorobo(uint16_t idOnline);
    void SendCollideNitro(uint16_t idOnline, uint16_t idWithTotem, uint16_t idWithNitro);
    void SendThrowMelonOPudin(const uint16_t idOnline, const int64_t time, const uint16_t idPUOnline, const glm::vec3& position, const glm::vec3& rotation, const int8_t typePU);
    void SendThrowTelebanana(const uint16_t idOnline, const int64_t time, const uint16_t idPUOnline, const glm::vec3& position, const glm::vec3& rotation, const int8_t typePU, const uint16_t idToPursue);
    void SendEndgame(uint16_t idPlayer);
    void SendDateTime();
    void SendLaunchAnimationEnd(const uint16_t idOnline, const uint16_t idOnlineWinner);
    void SendWaitingForCountdown(const uint16_t idOnline);
    uint32_t idMainCar;

   private:
    void StartReceiving();
    void HandleReceived(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytesTransferred);
    void HandleReceivedInputs(int64_t time, const vector<Constants::InputTypes> inputs, const uint16_t idRival, const float speed, const float wheelRotation, const float skidDeg, const float skidRotation) const;
    void HandleReceivedSync(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedCatchPU(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedCatchTotem(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedLostTotem(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedUsedRoboJorobo(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedCollideNitro(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedDisconnection(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedEndgame(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedThrowMelonOPudin(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedThrowTelebanana(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedCrashPUCar(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedCrashPUWall(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedLaunchEndAnimation(uint16_t idPlayer, uint16_t idWinner) const;
    void HandleReceivedLaunchCountdownAnimation() const;
    void HandleReceivedClockSync(unsigned char* recevBuff, size_t bytesTransferred);
    void HandleReceivedFinalClockSync(unsigned char* recevBuff, size_t bytesTransferred);

    void HandleSentInputs(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentSync(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentPU(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentCatchTotem(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentLostTotem(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentRoboJorobo(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentCollideNitro(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentEndgame(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentThrowPU(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentCrashPUCar(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentCrashPUWall(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentLaunchAnimationEnd(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentWaitingForCountdown(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentClockSync(const boost::system::error_code& errorCode, std::size_t bytes_transferred);
    void HandleSentFinalClockSync(const boost::system::error_code& errorCode, std::size_t bytes_transferred);

    void HandleSentDateTime(const std::shared_ptr<std::string> message,
                            const boost::system::error_code& errorCode,
                            std::size_t bytes_transferred);

    boost::asio::io_context context;
    udp::endpoint serverEndpoint;
    udp::socket socket;
    std::thread butler;
    // boost::asio::io_context::strand strand;

    unordered_map<uint16_t, int64_t> lastTimeInputReceived;
    unordered_map<uint16_t, int64_t> lastTimeSyncReceived;
    unordered_map<uint16_t, int64_t> lastTimeCatchPUReceived;
    unordered_map<uint16_t, int64_t> lastTimeClockSyncReceived;
    unordered_map<uint16_t, int64_t> lastTimeFinalClockSyncReceived;
    unordered_map<uint16_t, int64_t> lastTimeCatchTotemReceived;
    unordered_map<uint16_t, int64_t> lastTimeLostTotemReceived;
    unordered_map<uint16_t, int64_t> lastTimeUsedRoboJoroboReceived;
    unordered_map<uint16_t, int64_t> lastTimeCollideNitroReceived;
    unordered_map<uint16_t, int64_t> lastTimeThrowMelonOPudinReceived;
    unordered_map<uint16_t, int64_t> lastTimeThrowTelebananaReceived;
    unordered_map<uint16_t, int64_t> lastTimeCrashPUCarReceived;
    unordered_map<uint16_t, int64_t> lastTimeCrashPUWallReceived;
    unordered_map<uint16_t, int64_t> lastTimeLaunchEndGameReceived;
    unordered_map<uint16_t, int64_t> lastTimeLaunchCountdownReceived;
    bool stateAnimationEnd {false};
    int64_t timeGameStarted;
};