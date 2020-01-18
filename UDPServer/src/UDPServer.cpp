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
    // cout << "Hemos recibido una petición del endpoint " << remoteClient.address() << ":" << remoteClient.port() << endl;
    if (!errorCode) {
        SaveClientIfNotExists(remoteClient);
        int8_t callType = recvBuff[0];
        switch (callType) {
            case -1: {  //Input
                int8_t inputType = recvBuff[1];
                HandleReceiveInput(inputType, remoteClient);
            } break;

            default:
                HandleReceiveDateTimeRequest(remoteClient);
                break;
        }
    }
    StartReceiving();  // antes estaba dentro del if, pero entonces si hay un error ya se rompe tó ¿?
}

void UDPServer::HandleReceiveDateTimeRequest(udp::endpoint& remoteClient) {
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
}

void UDPServer::HandleReceiveInput(int8_t inputType, udp::endpoint& remoteClient) {
    cout << "El usuario " << remoteClient.address() << ":" << remoteClient.port()
         << " nos ha enviado un input " << signed(inputType) << endl;
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

void UDPServer::HandleSend(boost::shared_ptr<string> message,
                           const boost::system::error_code& errorCode,
                           size_t bytes_transferred) {
    if (!errorCode)
        cout << "Ya se ha enviado el mensaje " << *message << ", madafaka" << endl;
    else
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
}