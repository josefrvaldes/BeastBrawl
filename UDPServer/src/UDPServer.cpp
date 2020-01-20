#include "UDPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../src/Constants.h"

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
        // uint16_t callType = recvBuff[0];
        // boost::array<uint16_t,1> callType = recvBuff[0];
        boost::asio::mutable_buffer buff1 = recvBuff[0];
        uint16_t* ptrCallType = static_cast<uint16_t*>(buff1.data());
        uint16_t callType = *ptrCallType;
        // uint16_t callType = *static_cast<uint16_t*>(recvBuff[0].data());

        // boost::array<Constants::PetitionTypes, 1> petitionTypes = *static_cast<boost::array<Constants::PetitionTypes, 1>*>(recvBuff[0].data());

        // boost::array<Constants::PetitionTypes, 1> petitionTypes = recvBuff[0].data();
        // boost::int8_t ojete = *static_cast<boost::int8_t *>(recvBuff[0].data());
        // Constants::PetitionTypes petitionType = ojete;
        // Constants::PetitionTypes petitionType = recvBuff[0].data();
        // Constants::PetitionTypes petitionType = recvBuff[0];
        switch (callType) {
            case Constants::PetitionTypes::SEND_INPUTS:  //Input
            case Constants::PetitionTypes::SEND_INPUT:  //Input
                HandleReceiveInput(remoteClient);
                break;

            default:
                HandleReceiveDateTimeRequest(remoteClient);
                break;
        }
    }
    StartReceiving();  // antes estaba dentro del if, pero entonces si hay un error ya se rompe tó ¿?
}

void UDPServer::HandleReceiveDateTimeRequest(const udp::endpoint& remoteClient) {
    boost::shared_ptr<string> message(new string(GetTime()));
    socket.async_send_to(
        boost::asio::buffer(*message),
        remoteClient,
        boost::bind(
            &UDPServer::HandleSentDateTimeRequest,
            this,
            message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPServer::HandleReceiveInput(const udp::endpoint& remoteClient) {
    const Constants::InputTypes inputType = *static_cast<Constants::InputTypes*>(recvBuff[1].data());
    // const Constants::InputTypes inputType = static_cast<Constants::InputTypes>(recvBuff[1]);
    cout << "El usuario " << remoteClient.address() << ":" << remoteClient.port()
         << " nos ha enviado un input " << (inputType) << endl;
    ResendInputToOthers(inputType, remoteClient);
}

void UDPServer::ResendInputToOthers(const int8_t inputType, const udp::endpoint& originalClient) {
    string originalAddress = originalClient.address().to_string();
    uint16_t originalPort = originalClient.port();
    for (udp::endpoint& currentClient : clients) {
        string currentAddress = currentClient.address().to_string();
        uint16_t currentPort = currentClient.port();

        // si el cliente NO es el jugador original que mandó este input, le reenviamos el input al resto
        if (originalAddress != currentAddress && originalPort != currentPort) {
        }
    }    
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

void UDPServer::HandleSentDateTimeRequest(const boost::shared_ptr<string> message,
                                          const boost::system::error_code& errorCode,
                                          size_t bytesTransferred) {
    HandleSentDefaultMessage(message, errorCode, bytesTransferred);
}

void UDPServer::HandleSentDefaultMessage(const boost::shared_ptr<string> message,
                                         const boost::system::error_code& errorCode,
                                         size_t bytesTransferred) {
    if (!errorCode)
        cout << "Ya se ha enviado el mensaje " << *message << ", madafaka" << endl;
    else
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
}