#include "TCPServer.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../src/Constants.h"

using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace std::chrono;

#define MIN_NUM_PLAYERS 2

TCPServer::TCPServer(boost::asio::io_context& context_, uint16_t port_)
    : context(context_), acceptor_(context_, tcp::endpoint(tcp::v4(), port_)) {
    //StartReceiving();
}

void TCPServer::StartReceiving() {
    TCPConnection::pointer new_connection = TCPConnection::Create(context);

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
        new_connection->Start();

        // Comprobaciones para ver si existe el player
        if (PlayerExists(new_connection) == false) {
            connections.push_back(new_connection);
            Player p;
            p.endpointTCP = new_connection->socket().remote_endpoint();
            players.push_back(p);
        }
        std::cout << "Num conexiones: " << connections.size() << std::endl;
        if (connections.size() == MIN_NUM_PLAYERS) {
            cout << "Ya hemos llegado al núm de conexiones para enviar partida, vamos a visar a los clientes" << endl;
            SendStartGame();
            // justo despues vaciar el tcp para otra conexion
        }
    }

    StartReceiving();
}

bool TCPServer::PlayerExists(TCPConnection::pointer new_connection) {
    string newAddress = new_connection->socket().remote_endpoint().address().to_string();
    uint16_t newPort = new_connection->socket().remote_endpoint().port();
    for (auto currentPlayer : connections) {
        string currentAddress = currentPlayer->socket().remote_endpoint().address().to_string();
        uint16_t currentPort = currentPlayer->socket().remote_endpoint().port();
        if (newAddress == currentAddress && newPort == currentPort)
            return true;
    }
    return false;
}

// obtener el string con todos los datos
void TCPServer::SendStartGame() {
    for (auto currentPlayer : connections) {
        json j;
        uint8_t posVector = 0;
        uint32_t numero = 0;
        vector<uint32_t> dataEnemies;
        for (auto currentPlayerSub : connections) {
            if (currentPlayer == currentPlayerSub) {
                numero = players[posVector].id + 1;  // cuidado al igualar uint16 con uint32
            } else {
                dataEnemies.push_back(players[posVector].id + 1);
            }
            posVector++;
        }

        j["idPlayer"] = numero;
        j["idEnemies"] = dataEnemies;
        string datos = j.dump();

        currentPlayer->SendStartMessage(datos);
    }
}