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
 
 
 #include "../../include/boost/array.hpp"
#include "../../include/boost/asio.hpp"
#include <chrono>
#include <iostream>
#include "./UDPServer.h"
#include "./TCPServer.h"
#include "./Server.h"
#include "../../src/Constants.h"

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {

    if(argc == 2) {
        Constants::MIN_NUM_PLAYERS = std::stoi(argv[1]);
    }
    //cout << "El número de jugadores será " << unsigned(Constants::MIN_NUM_PLAYERS) << endl;

    Server s;
    s.Start();
    return 0;
}