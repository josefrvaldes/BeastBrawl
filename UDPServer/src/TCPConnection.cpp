#include "TCPConnection.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../src/Constants.h"

using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace std::chrono;

TCPConnection::TCPConnection(asio::io_context& io_context) : socket_(io_context){

}

void TCPConnection::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred){


}


void TCPConnection::start(){
    message_ = GetTime();

    asio::async_write(
        socket_,
        asio::buffer(message_),
        boost::bind(&TCPConnection::HandleWrite,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

