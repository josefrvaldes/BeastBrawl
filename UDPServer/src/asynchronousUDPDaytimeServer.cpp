#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include "./UDPServer.h"

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

#define PORT 1234

int main(int argc, char* argv[]) {
    try {
        asio::io_context context;
        UDPServer server(context, PORT);
        context.run();
    } catch (std::exception& e) {
        cout << "Hubo una excepción " << e.what() << endl;
    }
    return 0;
}