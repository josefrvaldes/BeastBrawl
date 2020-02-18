#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

struct Player {
   public:
    Player() = default;
    ~Player() = default;
    uint16_t id{nextId++};
    udp::endpoint endpoint;
    tcp::endpoint endpointTCP;
    uint16_t lastInputIDReceived{0};
    uint16_t lastSyncIDReceived{0};
    bool hasTotem {false};

   private:
    static uint16_t nextId;

};