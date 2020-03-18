#include "TCPServer.h"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../../src/Constants.h"
#include "../src/Systems/Utils.h"
#include "Server.h"
#include "../../src/Systems/Serialization.h"


using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace std::chrono;



TCPServer::TCPServer(boost::asio::io_context& context_, uint16_t port_, UDPServer &udpServer_)
    : context{context_}, acceptor_{context_, tcp::endpoint(tcp::v4(), port_)}, udpServer{udpServer_} {
    //StartReceiving();
}

TCPServer::~TCPServer() {
    cout << "Se ha llamado al destructor de TCPServer" << endl;
}

void TCPServer::Close() {
    for(const auto connection : connections) {
        connection->Close();
    }
    acceptor_.close();
}

void TCPServer::StartReceiving() {
    TCPConnection::pointer new_connection = TCPConnection::Create(context, players, connections);
    acceptor_.async_accept(
        new_connection->socket(),
        boost::bind(&TCPServer::HandleAccept,
                    this,
                    new_connection,
                    boost::asio::placeholders::error));
}

void TCPServer::HandleAccept(TCPConnection::pointer new_connection, const boost::system::error_code& error) {
    if (!error) {
        //std::cout << "Recibi un mensaje" << std::endl;
        if(Server::GAME_STARTED == false){
            new_connection->Start();

            // Comprobaciones para ver si existe el player
            if (PlayerExists(new_connection) == false) {
                connections.push_back(new_connection);
                Player p;
                p.endpointTCP = new_connection->socket().remote_endpoint();
                players.push_back(p);
            }
            if (connections.size() >= Constants::MIN_NUM_PLAYERS) {
                cout << "Ya hemos llegado al núm de conexiones para enviar partida, vamos a visar a los clientes" << endl;
                Server::GAME_STARTED = true;
                SendStartGame();
                // justo despues vaciar el tcp para otra conexion
            }
        }else{
            // no dejar entrar a la sala
            cout << "ENTRA" << "\n";
            new_connection->SendFullGame();
        }
    }

    StartReceiving();
}

bool TCPServer::PlayerExists(TCPConnection::pointer new_connection) {
    string newAddress = new_connection->socket().remote_endpoint().address().to_string();
    uint16_t newPort = new_connection->socket().remote_endpoint().port();
    for (const auto& currentPlayer : connections) {
        string currentAddress = currentPlayer->socket().remote_endpoint().address().to_string();
        uint16_t currentPort = currentPlayer->socket().remote_endpoint().port();
        if (newAddress == currentAddress && newPort == currentPort)
            return true;
    }
    return false;
}

// obtener el string con todos los datos
void TCPServer::SendStartGame() {
    // como ya vamos a empezar una partida nueva, a partir de ahora sí aceptaremos que la partida se pueda acabar
    udpServer.ResetTimerStartReceiving();
    udpServer.StartReceiving();
    udpServer.CheckDisconnectionsAfterSeconds();
    for (const auto& currentPlayer : connections) {
        json j;
        uint8_t posVector = 0;
        uint16_t idPlayer = 0;
        vector<uint16_t> idsEnemies;
        for (auto currentPlayerSub : connections) {
            if (currentPlayer == currentPlayerSub) {
                idPlayer = players[posVector].id + 1;
            } else {
                idsEnemies.push_back(players[posVector].id + 1);
            }
            posVector++;
        }

        // std::shared_ptr<boost::array<unsigned char, Constants::ONLINE_BUFFER_SIZE>> buff = make_shared<boost::array<unsigned char, Constants::ONLINE_BUFFER_SIZE>>();
        std::shared_ptr<unsigned char[]> buff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
        size_t currentBuffSize = 0;
        uint8_t callType = Constants::PetitionTypes::TCP_START_GAME;
        uint8_t enemiesSize = idsEnemies.size();
    
        Serialization::Serialize(buff.get(), &callType, currentBuffSize);
        Serialization::Serialize(buff.get(), &idPlayer, currentBuffSize);
        Serialization::Serialize(buff.get(), &enemiesSize, currentBuffSize);
        Serialization::SerializeVector(buff.get(), idsEnemies, currentBuffSize);

        // j["idPlayer"] = idPlayer;
        // j["idEnemies"] = idsEnemies;
        // string datos = j.dump();

        currentPlayer->SendStartMessage(buff.get(), currentBuffSize);
    }
    Server::ACCEPTING_ENDGAME = true; 
}