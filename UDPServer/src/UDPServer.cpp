#include "UDPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../../src/Constants.h"
#include "../../src/Systems/Utils.h"

using json = nlohmann::json;
using boost::asio::ip::udp;
using namespace std::chrono;

UDPServer::UDPServer(boost::asio::io_context& context_, uint16_t port_)
    : socket(context_, udp::endpoint(udp::v4(), port_)) {
    // StartReceiving();
}

void UDPServer::StartReceiving() {
    std::shared_ptr<unsigned char[]> recevBuff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
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
        uint8_t petitionType;
        uint16_t idPlayer;
        uint16_t idCall;
        size_t currentIndex = 0;
        Utils::Deserialize(&petitionType, recevBuff.get(), currentIndex);
        Utils::Deserialize(&idCall, recevBuff.get(), currentIndex);
        Utils::Deserialize(&idPlayer, recevBuff.get(), currentIndex);

        // TODO: esto creo que podría evitarse
        unsigned char buffRecieved[Constants::ONLINE_BUFFER_SIZE];
        memcpy(&buffRecieved[0], &recevBuff.get()[0], bytesTransferred);

        SavePlayerIfNotExists(idPlayer, *remoteClient.get());

        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);

        auto player = GetPlayerById(idPlayer);
        if (player != nullptr) {
            Player& p = *player;
            switch (callType) {
                case Constants::PetitionTypes::SEND_INPUTS: {
                    //std::cout << "Recibidos inputs: " << bytesTransferred << std::endl;
                    if (p.lastInputIDReceived < idCall) {
                        // cout << "Se ha recibido y reenviado un paquete de input del player " << idPlayer << endl;
                        p.lastInputIDReceived = idCall;
                        HandleReceivedInputs(buffRecieved, bytesTransferred, *remoteClient.get());
                    } else {
                        cout << "Se ha ignorado un paquete de input porque era antiguo" << endl;
                    }
                } break;
                case Constants::PetitionTypes::SEND_SYNC: {
                    //std::cout << "Recibida sincronizacion: " << bytesTransferred << std::endl;
                    if (p.lastSyncIDReceived < idCall) {
                        // cout << "Se ha recibido y reenviado un paquete de sync del player " << idPlayer << endl;
                        p.lastSyncIDReceived = idCall;
                        HandleReceivedSync(buffRecieved, bytesTransferred, *remoteClient.get());
                    } else {
                        cout << "Se ha ignorado un paquete de sync porque era antiguo" << endl;
                    }
                } break;

                default:
                    cout << "Petición incorrecta" << endl;
                    break;
            }
        } else {
            cout << idPlayer << " - No se ha encontrado el player que corresponde con esta llamada. CATÁSTROFE" << endl;
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
    } else {
        //std::cout << "Se han enviado: " << bytesTransferred << std::endl;
    }
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
    p.id = id;
    p.endpoint = newClient;
    players.push_back(p);
    cout << "Hemos guardado un cliente nuevo y ahora tenemos " << players.size() << " clientes" << endl;
}

Player* UDPServer::GetPlayerById(uint16_t idPlayer) {
    auto it = std::find_if(players.begin(), players.end(),
                           [&idPlayer](Player p) {
                               return p.id == idPlayer;
                           });
    if (it != players.end())
        return it.base();
    return nullptr;
}