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
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once

#include <iostream>
#include "../../include/boost/asio.hpp"
//#include "TCPConnection.h"

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

struct Player {
   public:
    Player() = default;
    ~Player() = default;
    

    uint16_t id{nextId++};
    uint8_t character{0};
    udp::endpoint endpoint;
    tcp::endpoint endpointTCP;
    //TCPConnection::pointer connectionPointer;
    int64_t lastInputTimeReceived{0};
    int64_t lastSyncTimeReceived{0};
    int64_t lastCatchPUTimeReceived{0};
    int64_t lastCatchTotemTimeReceived{0};
    int64_t lastLostTotemTimeReceived{0};
    int64_t lastUsedRoboJoroboTimeReceived{0};
    int64_t lastCollideNitroTimeReceived{0};
    int64_t lastThrowPUReceived{0};
    int64_t lastCrashPUCarReceived{0};
    int64_t lastCrashPUWallReceived{0};
    int64_t lastLaunchAnimationEndReceived{0};
    int64_t lastWaitingForCountdownReceived{0};
    int64_t lastClockSyncReceived{0};
    int64_t lastFinalClockSyncReceived{0};

    bool hasTotem {false};
    bool disconnected {false};
    bool readyToDelete {false};
    bool waitingForCountdown {false};


    inline static uint16_t nextId{0};
   private:
};