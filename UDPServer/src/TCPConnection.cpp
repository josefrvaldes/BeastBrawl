#include "TCPConnection.h"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#include "../../src/Systems/Serialization.h"
#include "../src/Constants.h"
#include "../src/Systems/Utils.h"
#include "TCPServer.h"
#include "Server.h"

using boost::asio::ip::tcp;
using namespace std::chrono;

TCPConnection::TCPConnection(TCPServer *tcpServer_, asio::io_context& io_context, std::vector<Player>& p, std::vector<uint8_t> &c, std::vector<TCPConnection::pointer>& connect) : tcpServer{tcpServer_}, socket_(io_context), players(p), characters(c), connections(connect) {
}

TCPConnection::~TCPConnection() {
    cout << "Se ha llamado al destructor de TCPConnection" << endl;
}

void TCPConnection::Close() {
    socket_.close();
}

void TCPConnection::Start() {
    std::shared_ptr<unsigned char[]> recevBuff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    socket_.async_receive(
        asio::buffer(recevBuff.get(), Constants::ONLINE_BUFFER_SIZE),
        boost::bind(&TCPConnection::HandleRead,
                    shared_from_this(),
                    recevBuff,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::HandleRead(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error && bytes_transferred != 0) {
        size_t currentIndex = 0;
        uint8_t petitionType = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);    // numero
        cout << "Hemos leido " << bytes_transferred << " bytes y el petitionType es " << unsigned(petitionType) << endl;

        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);
        switch (callType) {
            case Constants::PetitionTypes::CONNECTION_REQUEST: {
                uint8_t characterRecvd = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);  // personaje
                characters.push_back(characterRecvd);

                if (players.size() >= Constants::MIN_NUM_PLAYERS) {
                    // cout << "Ya hemos llegado al núm de conexiones para enviar partida, vamos a visar a los clientes" << endl;
                    Server::GAME_STARTED = true;
                    tcpServer->SendStartGame();
                    // justo despues vaciar el tcp para otra conexion
                }
                cout << "Se ha conectado un usuario con personaje " << unsigned(characterRecvd) << "\n";
            } break;
            default:
                break;
        }
    } else if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error)) {
        std::cout << "Se desconecta un usuario: " << error.message() << std::endl;
        DeleteMe();
    } else if (error) {
        std::cout << "Error al leer: " << error.message() << std::endl;
    }
    Start();
}

void TCPConnection::DeleteMe() {
    // eliminar del array de jugadores
    players.erase(
        std::remove_if(
            players.begin(),
            players.end(),
            [&](Player& p) { return (p.endpointTCP == socket_.remote_endpoint()); }),
        players.end());

    // actualizamos las IDs
    uint16_t idPlayer = 0;
    for (auto& actualPlayer : players) {
        actualPlayer.id = idPlayer;
        idPlayer++;
    }
    Player::nextId = idPlayer;

    // eliminar del array de conexiones
    connections.erase(
        std::remove_if(
            connections.begin(),
            connections.end(),
            [&](TCPConnection::pointer& c) { return (c->socket().remote_endpoint() == socket_.remote_endpoint()); }),
        connections.end());
}

// void TCPConnection::SendStartMessage(string datos){
//     socket_.async_send(
//         boost::asio::buffer(datos, datos.size()),
//         boost::bind(
//             &TCPConnection::HandleWrite,
//             this,
//             boost::asio::placeholders::error,
//             boost::asio::placeholders::bytes_transferred));
// }

void TCPConnection::SendStartMessage(unsigned char* buff, size_t buffSize) {
    socket_.async_send(
        boost::asio::buffer(buff, buffSize),
        boost::bind(
            &TCPConnection::HandleWrite,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::SendFullGame() {
    unsigned char request[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::TCP_FULL_GAME;
    Serialization::Serialize(request, &petitionType, currentBuffSize);

    socket_.async_send(
        boost::asio::buffer(request, currentBuffSize),
        boost::bind(
            &TCPConnection::HandleWrite,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        std::cout << "Mensaje enviado del servidor TCP"
                  << "\n";
    } else {
        std::cout << "Error al escribir: " << error.message() << "\n";
    }
}
