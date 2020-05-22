#pragma once

#include <memory>
#include "../../include/boost/asio.hpp"
#include "../src/Constants.h"
#include "./TCPServer.h"
#include "./UDPServer.h"

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class Server {
   public:
    Server();
    void Start();
    void Restart();

    // esta variable sirve para controlar que al acabar la partida, recibiremos 
    // un endgame de cada jugador. Con esto controlamos no reiniciar el servidor 
    // 4 veces si hay 4 jugadores. Recibiremos uno, pondremos esto a false
    // y cuando empiece una nueva partida pondremos esto de nuevo a true
    inline static bool ACCEPTING_ENDGAME{true};
    inline static bool GAME_STARTED{false};

   private:
    std::shared_ptr<asio::io_context> context;
    unique_ptr<UDPServer> serverUDP;
    unique_ptr<TCPServer> serverTCP;
};
