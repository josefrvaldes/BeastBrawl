#include "UDPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../../src/Systems/Utils.h"
#include "../../src/Constants.h"

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
        string receivedString;
        std::copy(recevBuff->begin(), recevBuff->begin() + bytesTransferred, std::back_inserter(receivedString));
        json receivedJSON = json::parse(receivedString);
        uint16_t auxCallType = receivedJSON["petitionType"];
        const uint32_t id = receivedJSON["id"];

        SavePlayerIfNotExists(id, *remoteClient.get());

        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(auxCallType);
        switch (callType) {
            case Constants::PetitionTypes::SEND_INPUT: {
                const uint16_t auxInput = receivedJSON["input"];
                Constants::InputTypes input = static_cast<Constants::InputTypes>(auxInput);
                HandleReceivedInput(input, *remoteClient.get());
            } break;

            case Constants::PetitionTypes::SEND_INPUTS: {
                HandleReceivedInputs(receivedString, *remoteClient.get());
            } break;
            case Constants::PetitionTypes::SEND_SYNC:  //Input
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

void UDPServer::HandleReceivedInputs(const string& stringToBeReSent, const udp::endpoint& originalClient) {
    ResendInputsToOthers(stringToBeReSent, originalClient);
}

void UDPServer::ResendInputsToOthers(const string& stringToBeReSent, const udp::endpoint& originalClient) {
    string originalAddress = originalClient.address().to_string();
    uint16_t originalPort = originalClient.port();
    for (Player& currentPlayer : players) {
        string currentAddress = currentPlayer.endpoint.address().to_string();
        uint16_t currentPort = currentPlayer.endpoint.port();
        // uint32_t currentID = currentPlayer.id;

        // si el cliente NO es el jugador original que mandó este input, le reenviamos el input al resto
        // TODO: falta comparar aquí con id
        if (originalAddress != currentAddress || originalPort != currentPort) {
            SendInputs(stringToBeReSent, currentPlayer);
        }
    }
}

void UDPServer::SendInputs(const string s, const Player& player) {
    std::shared_ptr<string> message = make_shared<string>(s);
    socket.async_send_to(
        boost::asio::buffer(*message),
        player.endpoint,
        boost::bind(
            &UDPServer::HandleSentInputs,
            this,
            message,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}

void UDPServer::HandleSentInputs(const std::shared_ptr<string> json, const boost::system::error_code& errorCode, std::size_t bytesTransferred) const {
    if (errorCode) {
        cout << "Hubo un error enviando inputs. errorcode: " << errorCode << endl;
        // Resend();
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

void UDPServer::HandleReceivedInput(const Constants::InputTypes input, const udp::endpoint& remoteClient) {
    cout << "Hemos entrado en HandleReceiveInput" << endl;
    ResendInputToOthers(input, remoteClient);
}

void UDPServer::ResendInputToOthers(const Constants::InputTypes input, const udp::endpoint& originalClient) {
    string originalAddress = originalClient.address().to_string();
    uint16_t originalPort = originalClient.port();
    for (Player& currentPlayer : players) {
        string currentAddress = currentPlayer.endpoint.address().to_string();
        uint16_t currentPort = currentPlayer.endpoint.port();
        // uint32_t currentID = currentPlayer.id;

        // si el cliente NO es el jugador original que mandó este input, le reenviamos el input al resto
        // TODO: falta comparar aquí con id
        if (originalAddress != currentAddress || originalPort != currentPort) {
            SendInput(input, currentPlayer);
        }
    }
}

void UDPServer::SendInput(const Constants::InputTypes input, const Player& p) {
    json j;
    j["petitionType"] = Constants::PetitionTypes::SEND_INPUT;
    j["id"] = p.id;
    j["input"] = input;
    std::shared_ptr<string> s = make_shared<string>(j.dump());
    socket.async_send_to(
        asio::buffer(*s),
        p.endpoint,
        boost::bind(
            &UDPServer::HandleSentInput,
            this,
            *s,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}

void UDPServer::HandleSentInput(const string& json, const boost::system::error_code& errorCode, std::size_t bytesTransferred) const {
    if (!errorCode) {
        cout << "Se ha enviado el input reenviado" << endl;
    } else {
        cout << "No se ha enviado el input reenviado" << endl;
    }
}

void UDPServer::SavePlayerIfNotExists(const uint32_t id, udp::endpoint& newClient) {
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