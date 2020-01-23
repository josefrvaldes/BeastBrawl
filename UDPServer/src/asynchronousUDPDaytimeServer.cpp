#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include "./UDPServer.h"
#include "./TCPServer.h"

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

#define PORT_UDP 1234
#define PORT_TCP 1235

int main(int argc, char* argv[]) {
    try {
        asio::io_context context;
        //asio::io_context context;
        UDPServer serverUDP(context, PORT_UDP);
        TCPServer serverTCP(context, PORT_TCP);
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guardUDP(context.get_executor());
        //boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guardTCP(contextTCP.get_executor());
        serverUDP.StartReceiving();
        serverTCP.StartReceiving();
        context.run();
        //contextTCP.run();
    } catch (std::exception& e) {
        cout << "Hubo una excepción " << e.what() << endl;
    }
    return 0;
}