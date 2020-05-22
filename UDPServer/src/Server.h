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
