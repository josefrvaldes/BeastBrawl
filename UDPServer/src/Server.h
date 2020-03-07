#pragma once

#include "../src/Constants.h"
#include "../../include/boost/asio.hpp"
#include "./UDPServer.h"
#include "./TCPServer.h"
#include <memory>

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class Server {
   public:
    Server();
    void Start();
    void Restart();

   private:
    std::shared_ptr<asio::io_context> context;
    unique_ptr<UDPServer> serverUDP;
    unique_ptr<TCPServer> serverTCP;
};
