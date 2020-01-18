#include "UDPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;
// using namespace boost;
using namespace std::chrono;

UDPServer::UDPServer(boost::asio::io_context& context_, uint16_t port_)
    : socket(context_, udp::endpoint(udp::v4(), port_)) {
    StartReceiving();
}

void UDPServer::StartReceiving() {
    socket.async_receive_from(
        asio::buffer(recvBuff),
        remoteEndpoint,
        boost::bind(
            &UDPServer::HandleReceive,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPServer::HandleReceive(const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    cout << "Hemos recibido algo madafaka" << endl;
    cout << "El endpoint del que hemos recibido es " << remoteEndpoint.address() << ":" << remoteEndpoint.port() << endl;
    if (!errorCode) {
        boost::shared_ptr<string> message(new string(GetTime()));
        socket.async_send_to(
            boost::asio::buffer(*message),
            remoteEndpoint,
            boost::bind(
                &UDPServer::HandleSend,
                this,
                message,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        StartReceiving();
    }
}

void UDPServer::HandleSend(boost::shared_ptr<std::string> message,
                           const boost::system::error_code& errorCode,
                           std::size_t bytes_transferred) {
    if (!errorCode)
        cout << "Ya se ha enviado el mensaje " << *message.get() << ", madafaka" << endl;
    else
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
}