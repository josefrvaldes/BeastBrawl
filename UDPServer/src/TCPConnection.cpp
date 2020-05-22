/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Antonio Jose Martinez Garcia
 * @author Jose Valdés Sirvent
 * 
 * 
 */

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

TCPConnection::TCPConnection(TCPServer *tcpServer_, asio::io_context& io_context, std::vector<std::shared_ptr<Player>>& p, std::vector<TCPConnection::pointer>& connect) : tcpServer{tcpServer_}, socket_(io_context), players(p), connections(connect) {
}

TCPConnection::~TCPConnection() {
    //cout << "Se ha llamado al destructor de TCPConnection" << endl;
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
        //cout << "Hemos leido " << bytes_transferred << " bytes y el petitionType es " << unsigned(petitionType) << endl;


        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);
        switch (callType) {
            case Constants::PetitionTypes::CONNECTION_REQUEST: {
                SendOpenGame(); // reenviar que se ha conectado correctamente
            } break;
            case Constants::PetitionTypes::TCP_CHARACTER_REQUEST: {
                HandleReceivedCatchChar(recevBuff, bytes_transferred);
            } break;
            case Constants::PetitionTypes::TCP_CANCEL_CHARACTER: {
                CancelChar();
            } break;
            default:
                break;
        }
    } else if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error)) {
        //std::cout << "Se desconecta un usuario: " << error.message() << std::endl;
        for(auto& p : players){
            if(p->id == player->id)
                p->character = Constants::ANY_CHARACTER;
        }
        tcpServer->SendCharsSelectedToOther(ID);
        DeleteMe();
        //tcpServer->SendCharsSelected(); // Testear
        //cout << "Se ha desconectado un nuevo jugador, ahora son " << players.size() << endl;
    } //else if (error) {
        //std::cout << "Error al leer: " << error.message() << std::endl;
    //}
    Start();
}

void TCPConnection::DeleteMe() {
    // eliminar del array de jugadores
    players.erase(
        std::remove_if(
            players.begin(),
            players.end(),
            [&](std::shared_ptr<Player>& p) { return p->id == player->id; }),
        players.end());

    // actualizamos las IDs
    uint16_t idPlayer_ = 0;
    for (auto& actualPlayer : players) {
        actualPlayer->id = idPlayer_;
        idPlayer_++;
    }
    Player::nextId = idPlayer_;

    // eliminar del array de conexiones
    connections.erase(
        std::remove_if(
            connections.begin(),
            connections.end(),
            // [&](TCPConnection::pointer& c) { return (c->socket().remote_endpoint() == socket_.remote_endpoint()); }),
            [&](TCPConnection::pointer& c) { return (c->ID == ID); }),
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

void TCPConnection::SendStartMessage(std::shared_ptr<unsigned char[]> buff, size_t buffSize) {
    socket_.async_send(
        boost::asio::buffer(buff.get(), buffSize),
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



void TCPConnection::SendOpenGame() {
    std::shared_ptr<unsigned char[]> request(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    vector<uint8_t> charsSelected;
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::TCP_OPEN_GAME;

    for (const auto& currentPlayer : players) {
        if(currentPlayer->character != Constants::ANY_CHARACTER)
            charsSelected.emplace_back(currentPlayer->character);
    }

    uint8_t charSelSize = charsSelected.size();
    Serialization::Serialize(request.get(), &petitionType, currentBuffSize);
    Serialization::Serialize(request.get(), &charSelSize, currentBuffSize);
    Serialization::SerializeVector(request.get(), charsSelected, currentBuffSize);

    socket_.async_send(
        boost::asio::buffer(request.get(), currentBuffSize),
        boost::bind(
            &TCPConnection::HandleSendOpenGame,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}



void TCPConnection::SendCharsSel(std::shared_ptr<unsigned char[]> buff, size_t buffSize) {
    socket_.async_send(
        boost::asio::buffer(buff.get(), buffSize),
        boost::bind(
            &TCPConnection::HandleWrite,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}



void TCPConnection::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
    //if (!error) {
    //    std::cout << "Mensaje enviado del servidor TCP"
    //              << "\n";
    //} else {
    //    std::cout << "Error al escribir: " << error.message() << "\n";
    //}
}

void TCPConnection::HandleSendOpenGame(const boost::system::error_code& error, size_t bytes_transferred) {
    //if (!error) {
    //    std::cout << "Mensaje enviado del servidor TCP open game\n";
    //} else {
    //    std::cout << "Error al escribir: " << error.message() << "\n";
    //}
}


void TCPConnection::CancelChar(){
    for(auto& p : players){
        if(p->endpointTCP == socket_.remote_endpoint())
            p->character = Constants::ANY_CHARACTER;
    }

    tcpServer->SendCharsSelected();
}


void TCPConnection::HandleReceivedCatchChar(std::shared_ptr<unsigned char[]> recevBuff, const size_t currentBufferSize){
    size_t currentIndex = 0;
    Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
    uint8_t characterRecvd = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);  // personaje
    bool alreadySelected = false;
    bool allSelected = true;

    for(const auto& p : players){
        if(p->character == characterRecvd){
            alreadySelected = true;
            break;
        }
    }

    // enviar mensaje si se ha podido seleccionar, en caso afirmativo a todos, en caso negativo a ti solo
    SendRequestSelChar(alreadySelected);

    // adjuntamos el personaje al usuario y comprobamos si todos ya han seleccionado
    if(!alreadySelected){
        for(auto& p : players){
            if(p->endpointTCP == socket_.remote_endpoint())
                p->character = characterRecvd;
            if(p->character==Constants::ANY_CHARACTER)
                allSelected = false;
        }
        
        if(allSelected && players.size() >= Constants::MIN_NUM_PLAYERS){
            // cout << "Ya hemos llegado al núm de conexiones para enviar partida, vamos a visar a los clientes" << endl;
            Server::GAME_STARTED = true;
            tcpServer->SendStartGame();
            // justo despues vaciar el tcp para otra conexion
        }else{
            tcpServer->SendCharsSelected();
        }
    }
}


void TCPConnection::SendRequestSelChar(bool selected){
    unsigned char request[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::PetitionTypes::TCP_CHARACTER_REQUEST;
    Serialization::Serialize(request, &petitionType, currentBuffSize);
    Serialization::Serialize(request, &selected, currentBuffSize);

    socket_.async_send(
        boost::asio::buffer(request, currentBuffSize),
        boost::bind(
            &TCPConnection::HandleWrite,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}
