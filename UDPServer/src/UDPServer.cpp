#include "UDPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../src/Constants.h"

using json = nlohmann::json;
using boost::asio::ip::udp;
using namespace std::chrono;

UDPServer::UDPServer(boost::asio::io_context& context_, uint16_t port_)
    : socket(context_, udp::endpoint(udp::v4(), port_)) {
    // StartReceiving();
}

void UDPServer::StartReceiving() {
    std::shared_ptr<boost::array<char, 1024>> recevBuff = make_shared<boost::array<char, 1024>>();
    std::shared_ptr<udp::endpoint> receiverEndpoint = make_shared<udp::endpoint>();
    socket.async_receive_from(
        asio::buffer(*recevBuff),
        *receiverEndpoint,
        boost::bind(
            &UDPServer::HandleReceive,
            this,
            recevBuff,
            receiverEndpoint,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPServer::HandleReceive(std::shared_ptr<boost::array<char, 1024>> recevBuff, std::shared_ptr<udp::endpoint> remoteClient, const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    // cout << "Hemos recibido una petición del endpoint " << remoteClient.address() << ":" << remoteClient.port() << endl;
    if (!errorCode) {
        SaveClientIfNotExists(id, *remoteClient.get());
        string s;
        std::copy(recevBuff->begin(), recevBuff->begin() + bytesTransferred, std::back_inserter(s));

        json j = json::parse(s);
        uint16_t auxCallType = j["petitionType"];

        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(auxCallType);
        switch (callType) {
            case Constants::PetitionTypes::SEND_INPUTS:  //Input
            case Constants::PetitionTypes::SEND_INPUT: {
                const uint16_t input = j["input"];
                HandleReceiveInput(input, *remoteClient.get());
            } break;

            case Constants::PetitionTypes::SEND_SYNC:  //Input
                break;

            default:
                HandleReceiveDateTimeRequest(*remoteClient.get());
                break;
        }
    }
    StartReceiving();  // antes estaba dentro del if, pero entonces si hay un error ya se rompe tó ¿?
}

// void UDPServer::HandleReceive(const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
//     // cout << "Hemos recibido una petición del endpoint " << remoteClient.address() << ":" << remoteClient.port() << endl;
//     if (!errorCode) {
//         SaveClientIfNotExists(receiverEndpoint);

//         string s;
//         std::copy(recvBuff.begin(), recvBuff.begin() + bytesTransferred, std::back_inserter(s));
//         Constants::PetitionTypes callType = Constants::PetitionTypes::SEND_INPUT;
//         switch (callType) {
//             case Constants::PetitionTypes::SEND_INPUTS:  //Input
//             case Constants::PetitionTypes::SEND_INPUT:   //Input
//                 HandleReceiveInput(receiverEndpoint);
//                 break;

//             default:
//                 HandleReceiveDateTimeRequest(receiverEndpoint);
//                 break;
//         }
//     }
//     StartReceiving();  // antes estaba dentro del if, pero entonces si hay un error ya se rompe tó ¿?
// }

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

void UDPServer::HandleReceiveInput(const uint16_t input, const udp::endpoint& remoteClient) {
    cout << "Hemos entrado en HandleReceiveInput" << endl;
    ResendInputToOthers(input, remoteClient);
    // const Constants::InputTypes inputType = *static_cast<Constants::InputTypes*>(recvBuff[1].data());
    // const Constants::InputTypes inputType = static_cast<Constants::InputTypes>(recvBuff[1]);
    // cout << "El usuario " << remoteClient.address() << ":" << remoteClient.port()
    //      << " nos ha enviado un input " << (inputType) << endl;
    // ResendInputToOthers(inputType, remoteClient);
}

void UDPServer::ResendInputToOthers(const uint16_t inputType, const udp::endpoint& originalClient) {
    string originalAddress = originalClient.address().to_string();
    uint16_t originalPort = originalClient.port();
    for (udp::endpoint& currentClient : clients) {
        string currentAddress = currentClient.address().to_string();
        uint16_t currentPort = currentClient.port();

        // si el cliente NO es el jugador original que mandó este input, le reenviamos el input al resto
        if (originalAddress != currentAddress || originalPort != currentPort) {
            cout << "Hemos llegado al if" << endl;
            // SendInput(input, currentClient);
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