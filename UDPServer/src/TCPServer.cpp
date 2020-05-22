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


#include "TCPServer.h"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <memory>
#include "../../src/Constants.h"
#include "../src/Systems/Utils.h"
#include "Server.h"
#include "../../src/Systems/Serialization.h"


using boost::asio::ip::tcp;
using namespace std::chrono;



TCPServer::TCPServer(boost::asio::io_context& context_, uint16_t port_, UDPServer &udpServer_)
    : context{context_}, acceptor_{context_, tcp::endpoint(tcp::v4(), port_)}, udpServer{udpServer_} {
    //StartReceiving();
}

TCPServer::~TCPServer() {
    //cout << "Se ha llamado al destructor de TCPServer" << endl;
}

void TCPServer::Close() {
    for(const auto connection : connections) {
        connection->Close();
    }
    acceptor_.close();
}

void TCPServer::StartReceiving() {
    TCPConnection::pointer new_connection = TCPConnection::Create(this, context, players, connections);
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
        if(Server::GAME_STARTED == false && players.size()<Constants::MIN_NUM_PLAYERS && PlayerExists(new_connection) == false){
            new_connection->Start();
        
            // Comprobaciones para ver si existe el player
            connections.push_back(new_connection);
            std::shared_ptr<Player> p = make_shared<Player>();
            p->endpointTCP = new_connection->socket().remote_endpoint();
            p->character = Constants::ANY_CHARACTER;
            // new_connection->currentPlayer = &p;
            players.push_back(p);
            new_connection->player = p;
            //cout << "Se ha conectado un nuevo jugador, ahora son " << players.size() << endl;

        }else{
            //std::cout << "Juego empezado: " << Server::GAME_STARTED  << std::endl;
            //std::cout << "Num Jugadores: " << players.size()  << std::endl;
            // no dejar entrar a la sala
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
        uint8_t posVector = 0;
        uint16_t idPlayer = 0;
        vector<uint16_t> idsEnemies;
        vector<uint8_t> charactersToSend;
        for (auto currentPlayerSub : connections) {
            if (currentPlayer == currentPlayerSub) {
                idPlayer = players[posVector]->id + 1;
            } else {
                idsEnemies.push_back(players[posVector]->id + 1);
                charactersToSend.push_back(players[posVector]->character);
            }
            posVector++;
        }

        std::shared_ptr<unsigned char[]> buff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
        size_t currentBuffSize = 0;
        uint8_t callType = Constants::PetitionTypes::TCP_START_GAME;
        uint8_t enemiesSize = idsEnemies.size();
        uint8_t charactersSize = charactersToSend.size();
    
        Serialization::Serialize(buff.get(), &callType, currentBuffSize);
        Serialization::Serialize(buff.get(), &idPlayer, currentBuffSize);
        Serialization::Serialize(buff.get(), &enemiesSize, currentBuffSize);
        Serialization::SerializeVector(buff.get(), idsEnemies, currentBuffSize);
        Serialization::Serialize(buff.get(), &charactersSize, currentBuffSize);
        Serialization::SerializeVector(buff.get(), charactersToSend, currentBuffSize);

        currentPlayer->SendStartMessage(buff, currentBuffSize);
    }
    Server::ACCEPTING_ENDGAME = true; 
}


// enviar los personajes ya seleccionados
void TCPServer::SendCharsSelected() {
    vector<uint8_t> charsSelected;
    for (const auto& currentPlayer : players) {
        if(currentPlayer->character != Constants::ANY_CHARACTER)
            charsSelected.emplace_back(currentPlayer->character);
    }

    std::shared_ptr<unsigned char[]> buff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::PetitionTypes::TCP_CHARACTERS_SELECTED;
    uint8_t charSelSize = charsSelected.size();

    Serialization::Serialize(buff.get(), &petitionType, currentBuffSize);
    Serialization::Serialize(buff.get(), &charSelSize, currentBuffSize);
    Serialization::SerializeVector(buff.get(), charsSelected, currentBuffSize);

    for (const auto& currentPlayer : connections) {
        currentPlayer->SendCharsSel(buff, currentBuffSize);
    }
}



void TCPServer::SendCharsSelectedToOther(uint16_t idConnection){
    vector<uint8_t> charsSelected;
    for (const auto& currentPlayer : players) {
        if(currentPlayer->character != Constants::ANY_CHARACTER)
            charsSelected.emplace_back(currentPlayer->character);
    }

    std::shared_ptr<unsigned char[]> buff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::PetitionTypes::TCP_CHARACTERS_SELECTED;
    uint8_t charSelSize = charsSelected.size();

    Serialization::Serialize(buff.get(), &petitionType, currentBuffSize);
    Serialization::Serialize(buff.get(), &charSelSize, currentBuffSize);
    Serialization::SerializeVector(buff.get(), charsSelected, currentBuffSize);

    for (const auto& currentPlayer : connections) {
        if(idConnection != currentPlayer->ID)
            currentPlayer->SendCharsSel(buff, currentBuffSize);
    }
}