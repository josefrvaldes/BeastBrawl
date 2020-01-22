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

#define PORT 1234

int main(int argc, char* argv[]) {
    try {
        asio::io_context context;
        UDPServer serverUDP(context, PORT);
        //TCPServer serverTCP(context, PORT);
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(context.get_executor());
        serverUDP.StartReceiving();
        //serverTCP.StartReceiving();
        context.run();
    } catch (std::exception& e) {
        cout << "Hubo una excepción " << e.what() << endl;
    }
    return 0;
}