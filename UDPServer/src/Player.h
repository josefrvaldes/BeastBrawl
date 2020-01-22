#include <iostream>

#include <boost/asio.hpp>

using boost::asio::ip::udp;

struct Player {
   public:
    Player() = default;
    ~Player() = default;
    uint32_t id{nextId++};
    udp::endpoint endpoint;

   private:
    static uint32_t nextId;
};