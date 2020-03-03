#include "UDPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../../src/Constants.h"
#include "../../src/Systems/Utils.h"
#include "../../src/Systems/Serialization.h"

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
        size_t currentIndex = 0;
        uint8_t petitionType = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
        int64_t time = Serialization::Deserialize<int64_t>(recevBuff.get(), currentIndex);
        uint16_t idPlayer = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
        cout << Utils::getISOCurrentTimestampMillis() << " Hemos recibido en el server la llamada " << time << " de tipo " << unsigned(petitionType) << " del user " << idPlayer << endl;
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
                    if (p.lastInputTimeReceived < time) {
                        cout << Utils::getISOCurrentTimestampMillis() << "Se ha recibido y reenviado un paquete de input del player " << idPlayer << endl;
                        p.lastInputTimeReceived = time;
                        HandleReceivedInputs(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                    } else {
                        cout << Utils::getISOCurrentTimestampMillis() << "Se ha ignorado un paquete de input porque era antiguo" << endl;
                    }
                } break;
                case Constants::PetitionTypes::SEND_SYNC: {
                    //std::cout << "Recibida sincronizacion: " << bytesTransferred << std::endl;
                    if (p.lastSyncTimeReceived < time) {
                        cout << Utils::getISOCurrentTimestampMillis() << "Se ha recibido y reenviado un paquete de sync del player " << idPlayer << endl;
                        p.lastSyncTimeReceived = time;
                        HandleReceivedSync(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                    } else {
                        cout << Utils::getISOCurrentTimestampMillis() << "Se ha ignorado un paquete de sync porque era antiguo" << endl;
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

void UDPServer::HandleReceivedInputs(const uint16_t id, const unsigned char resendInputs[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    ResendBytesToOthers(id, resendInputs, currentBufferSize, originalClient);
}

void UDPServer::HandleReceivedSync(const uint16_t id, unsigned char recevBuff[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    size_t currentIndex = 0;
    typeCPowerUp typePU;

    Serialization::Deserialize<uint8_t>(recevBuff, currentIndex);  // petitionType
    int64_t time = Serialization::Deserialize<int64_t>(recevBuff, currentIndex);
    uint16_t idCarOnline = Serialization::Deserialize<uint16_t>(recevBuff, currentIndex);
    glm::vec3 posCar = Serialization::DeserializeVec3(recevBuff, currentIndex);
    glm::vec3 rotCar = Serialization::DeserializeVec3(recevBuff, currentIndex);

    glm::vec3 posTotem(0.0, 0.0, 0.0);
    bool haveTotem;
    bool totemInGround;
    Serialization::DeserializePowerUpTotem(recevBuff, typePU, haveTotem, totemInGround, currentIndex);

    /*int64_t totemTime = */ Serialization::Deserialize<int64_t>(recevBuff, currentIndex);  // totemTime

    if (totemInGround) {
        posTotem = Serialization::DeserializeVec3(recevBuff, currentIndex);
    }
    cout << Utils::getISOCurrentTimestampMillis() << "he recibido el sync " << time << " de [" << idCarOnline << "," << id << "] y está en la pos("
         << posCar.x << "," << posCar.y << "," << posCar.z << ") - rot("
         << rotCar.x << "," << rotCar.y << "," << rotCar.z << ")." << endl
         << "Lleva el PU " << (int)typePU << " y lleva el totem(" << haveTotem << ")." << endl
         << "El totem está en el suelo(" << totemInGround << ") y su pos es (" << posTotem.x << "," << posTotem.y << "," << posTotem.z << ") "
         << endl;

    ResendBytesToOthers(id, recevBuff, currentBufferSize, originalClient);
}

void UDPServer::ResendBytesToOthers(const uint16_t id, const unsigned char resendBytes[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    // string originalAddress = originalClient.address().to_string();
    // uint16_t originalPort = originalClient.port();
    // cout << "Vamos a reenviar bytes del cliente [" << id << "] y que tiene la dirección " << originalAddress << ":" << originalPort << " a los demás" << endl;

    for (Player& currentPlayer : players) {
        if (currentPlayer.id != id) {
            // const auto& currentEndpoint = currentPlayer.endpoint;
            // const auto& currentAddress = currentEndpoint.address().to_string();
            // const auto& currentPort = currentEndpoint.port();
            // cout << "Vamos a reenviar bytes del cliente [" << id << "] a [" << currentPlayer.id << "] " << currentAddress << ":" << currentPort << " a los demás" << endl;
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
    }
}

void UDPServer::SavePlayerIfNotExists(const uint16_t id, udp::endpoint& newClient) {
    string newAddress = newClient.address().to_string();
    uint16_t newPort = newClient.port();
    for (Player& currentPlayer : players) {
        // si el cliente ya lo tenemos guardado, nos salimos
        // string currentAddress = currentPlayer.endpoint.address().to_string();
        // uint16_t currentPort = currentPlayer.endpoint.port();
        if (currentPlayer.id == id)
            return;
    }
    Player p;
    p.id = id;
    p.endpoint = newClient;
    players.push_back(p);
    cout << "Hemos guardado al cliente con id " << id << " y que tiene la dirección " << newAddress << ":" << newPort << " y ahora tenemos " << players.size() << " clientes" << endl;
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