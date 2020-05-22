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