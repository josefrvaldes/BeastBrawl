#include "../../include/boost/array.hpp"
#include "../../include/boost/asio.hpp"
#include <chrono>
#include <iostream>
#include "./UDPServer.h"
#include "./TCPServer.h"
#include "../../src/Constants.h"

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {

    if(argc == 2) {
        Constants::MIN_NUM_PLAYERS = std::stoi(argv[1]);
    }
    cout << "El número de jugadores será " << unsigned(Constants::MIN_NUM_PLAYERS) << endl;


    try {
        asio::io_context context;
        UDPServer serverUDP(context, SERVER_PORT_UDP);
        TCPServer serverTCP(context, SERVER_PORT_TCP);
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guardUDP(context.get_executor());
        serverUDP.StartReceiving();
        serverTCP.StartReceiving();
        context.run();
        //contextTCP.run();
    } catch (std::exception& e) {
        cout << "Hubo una excepción " << e.what() << endl;
    }
    return 0;
}