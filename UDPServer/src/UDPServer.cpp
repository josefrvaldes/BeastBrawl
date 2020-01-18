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
        receiverEndpoint,
        [&](const boost::system::error_code& error, std::size_t bytesTransferred) {
            HandleReceive(receiverEndpoint, error, bytesTransferred);
        });
}

void UDPServer::HandleReceive(udp::endpoint& remoteClient, const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    cout << "Hemos recibido una petición del endpoint " << remoteClient.address() << ":" << remoteClient.port() << endl;
    if (!errorCode) {
        SaveClientIfNotExists(remoteClient);
        boost::shared_ptr<string> message(new string(GetTime()));
        socket.async_send_to(
            boost::asio::buffer(*message),
            remoteClient,
            boost::bind(
                &UDPServer::HandleSend,
                this,
                message,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        // StartReceiving();  // probablemente esto debería estar fuera del if!!
    }
    StartReceiving();
}

void UDPServer::SaveClientIfNotExists(udp::endpoint& newClient) {
    string newAddress = newClient.address().to_string();
    uint16_t newPort = newClient.port();
    for (udp::endpoint& currentClient : clients) {
        // si el cliente ya lo tenemos guardado, nos salimos
        string currentAddress = currentClient.address().to_string();
        uint16_t currentPort = currentClient.port();

        if (newAddress == currentAddress && newPort == currentPort)
            return;
    }
    clients.push_back(newClient);
    cout << "Hemos guardado un cliente nuevo y ahora tenemos " << clients.size() << " clientes" << endl;
}

void UDPServer::HandleSend(boost::shared_ptr<std::string> message,
                           const boost::system::error_code& errorCode,
                           std::size_t bytes_transferred) {
    if (!errorCode)
        cout << "Ya se ha enviado el mensaje " << *message.get() << ", madafaka" << endl;
    else
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
}