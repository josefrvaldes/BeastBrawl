#include "UDPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../../src/Systems/Utils.h"
//#include "../src/Systems/Serialization.h"
#include "../../src/Constants.h"

using json = nlohmann::json;
using boost::asio::ip::udp;
using namespace std::chrono;

UDPServer::UDPServer(boost::asio::io_context& context_, uint16_t port_)
    : socket(context_, udp::endpoint(udp::v4(), port_)) {
    // StartReceiving();
}

void UDPServer::StartReceiving() {
    std::shared_ptr<unsigned char[]> recevBuff ( new unsigned char[Constants::ONLINE_BUFFER_SIZE] );
    std::shared_ptr<udp::endpoint> receiverEndpoint = make_shared<udp::endpoint>();
    socket.async_receive_from(
        asio::buffer(recevBuff.get(), Constants::ONLINE_BUFFER_SIZE),
        *receiverEndpoint,
        boost::bind(
            &UDPServer::HandleReceive,
            this,
            recevBuff,
            receiverEndpoint,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPServer::HandleReceive(std::shared_ptr<unsigned char[]> recevBuff, std::shared_ptr<udp::endpoint> remoteClient, const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    // cout << "Hemos recibido una petición del endpoint " << remoteClient.address() << ":" << remoteClient.port() << endl;
    if (!errorCode) {
        //string receivedString;
        //std::copy(recevBuff->begin(), recevBuff->begin() + bytesTransferred, std::back_inserter(receivedString));
        //json receivedJSON = json::parse(receivedString);
        //uint16_t auxCallType = receivedJSON["petitionType"];
        //const uint32_t id = receivedJSON["id"];
        uint8_t petitionType;
        uint16_t idPlayer;
        size_t currentIndex = 0;
        Utils::Deserialize(&petitionType, recevBuff.get(), currentIndex);
        Utils::Deserialize(&idPlayer, recevBuff.get(), currentIndex);
        unsigned char buffRecieved[Constants::ONLINE_BUFFER_SIZE];
        memcpy(&buffRecieved[0], &recevBuff.get()[0], bytesTransferred);

        //cout << " -- " << endl;
        //for (size_t i = 0; i < bytesTransferred; i++) {
        //     cout << "item " << i << " = " << unsigned(resendInputs[i]) << endl;
        //}
        //std::copy(&recevBuff, &recevBuff + bytesTransferred, std::back_inserter(resendInputs));

        SavePlayerIfNotExists(idPlayer, *remoteClient.get());

        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);
        switch (callType) {
            case Constants::PetitionTypes::SEND_INPUT: {
                //const uint16_t auxInput = receivedJSON["input"];
                //Constants::InputTypes input = static_cast<Constants::InputTypes>(auxInput);
                //HandleReceivedInput(input, *remoteClient.get());
            } break;

            case Constants::PetitionTypes::SEND_INPUTS: {
                std::cout << "Recibidos inputs: " << bytesTransferred << std::endl;
                HandleReceivedInputs(buffRecieved, bytesTransferred, *remoteClient.get());
            } break;
            case Constants::PetitionTypes::SEND_SYNC:  //Sync
                std::cout << "Recibida sincronizacion: " << bytesTransferred << std::endl;
                HandleReceivedSync(buffRecieved, bytesTransferred, *remoteClient.get());
                break;

            default:
                HandleReceiveDateTimeRequest(*remoteClient.get());
                break;
        }
    } else {
        cout << "¡¡HUBO UN ERROR AL RECIBIR DATOS EN EL SERVER!! errorcode:" << errorCode << endl;
    }
    StartReceiving();  // antes estaba dentro del if, pero entonces si hay un error ya se rompe tó ¿?
}


void UDPServer::HandleReceivedInputs(const unsigned char resendInputs[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    ResendBytesToOthers(resendInputs, currentBufferSize, originalClient);
}

void UDPServer::HandleReceivedSync(const unsigned char resendSync[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    ResendBytesToOthers(resendSync, currentBufferSize, originalClient);
}


void UDPServer::ResendBytesToOthers(const unsigned char resendBytes[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    string originalAddress = originalClient.address().to_string();
    uint16_t originalPort = originalClient.port();
    for (Player& currentPlayer : players) {
        string currentAddress = currentPlayer.endpoint.address().to_string();
        uint16_t currentPort = currentPlayer.endpoint.port();
        // uint32_t currentID = currentPlayer.id;

        // si el cliente NO es el jugador original que mandó este mensaje, le reenviamos el mensaje al resto
        // TODO: falta comparar aquí con id
        if (originalAddress != currentAddress || originalPort != currentPort) {
            SendBytes(resendBytes, currentBufferSize, currentPlayer);
        }
    }
}

void UDPServer::SendBytes(const unsigned char resendBytes[], const size_t currentBufferSize, const Player& player) {
    //std::shared_ptr<string> message = make_shared<string>(s);
    socket.async_send_to(
        boost::asio::buffer(resendBytes, currentBufferSize),
        player.endpoint,
        boost::bind(
            &UDPServer::HandleSentBytes,
            this,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}

void UDPServer::HandleSentBytes(const boost::system::error_code& errorCode, std::size_t bytesTransferred) const {
    if (errorCode) {
        cout << "Hubo un error enviando Bytes. errorcode: " << errorCode << endl;
        // Resend();
    }else{
        std::cout << "Se han enviado: " << bytesTransferred << std::endl;
    }
}


void UDPServer::HandleReceiveDateTimeRequest(const udp::endpoint& remoteClient) {
    boost::shared_ptr<string> message(new string(Utils::GetTime()));
    socket.async_send_to(
        boost::asio::buffer(*message),
        remoteClient,
        boost::bind(
            &UDPServer::HandleSentDateTimeRequest,
            this,
            message,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}


void UDPServer::SavePlayerIfNotExists(const uint16_t id, udp::endpoint& newClient) {
    string newAddress = newClient.address().to_string();
    uint16_t newPort = newClient.port();
    for (Player& currentPlayer : players) {
        // si el cliente ya lo tenemos guardado, nos salimos
        string currentAddress = currentPlayer.endpoint.address().to_string();
        uint16_t currentPort = currentPlayer.endpoint.port();
        // TODO: Falta comprobar por id
        if (newAddress == currentAddress && newPort == currentPort)
            return;
    }
    Player p;
    p.endpoint = newClient;
    players.push_back(p);
    cout << "Hemos guardado un cliente nuevo y ahora tenemos " << players.size() << " clientes" << endl;
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