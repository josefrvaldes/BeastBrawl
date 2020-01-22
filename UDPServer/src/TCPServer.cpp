#include "TCPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../src/Constants.h"

using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace std::chrono;

TCPServer::TCPServer(boost::asio::io_context& context_, uint16_t port_)
    : context(context_), acceptor_(context_, tcp::endpoint(tcp::v4(), port_)) {
    //StartReceiving();
}


void TCPServer::StartReceiving(){
    TCPConnection::pointer new_connection = TCPConnection::Create(context);
    
    acceptor_.async_accept(
        new_connection->socket(),
        boost::bind(&TCPServer::HandleAccept,
            this,
            new_connection,
            boost::asio::placeholders::error));
}



void TCPServer::HandleAccept(TCPConnection::pointer new_connection, const boost::system::error_code& error) {
    if (!error){
        std::cout << "Recibi un mensaje" << std::endl;
        new_connection->start();
    }

    StartReceiving();
}