#include "TCPConnection.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../src/Constants.h"
#include "../src/Systems/Utils.h"
#include "../../src/Systems/Serialization.h"


using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace std::chrono;

TCPConnection::TCPConnection(asio::io_context& io_context, std::vector<Player> &p, vector<TCPConnection::pointer>& connect) : socket_(io_context), players(p), connections(connect){

}

void TCPConnection::Close() {
    socket_.close();
}

void TCPConnection::Start(){
    std::shared_ptr<unsigned char[]> recevBuff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    socket_.async_receive(
        asio::buffer(recevBuff.get(), Constants::ONLINE_BUFFER_SIZE),
        boost::bind(&TCPConnection::HandleRead,
            shared_from_this(),
            recevBuff,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}



void TCPConnection::HandleRead(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytes_transferred){
    if(!error && bytes_transferred!=0){
        size_t currentIndex = 0;
        uint8_t petitionType = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex); // numero

        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);
        switch (callType){
            case Constants::PetitionTypes::CONNECTION_REQUEST :{
                cout << "Se ha conectado un usuario" << "\n";
            }break;
            default:
                break;
        }
    }else if( (boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error) ){
        std::cout << "Se desconecta un usuario: " << error.message() << std::endl;
        DeleteMe();
    }else if(error){
        std::cout << "Error al leer: " << error.message() << std::endl;
    }
    Start();
} 



void TCPConnection::DeleteMe(){
    // eliminar del array de jugadores
    players.erase(
        std::remove_if(
            players.begin(),
            players.end(),
            [&](Player& p) { return (p.endpointTCP==socket_.remote_endpoint()); }),
        players.end());
    
    // actualizamos las IDs
    uint16_t idPlayer = 0;
    for(auto& actualPlayer : players){
        actualPlayer.id = idPlayer;
        idPlayer++;
    }
    Player::nextId = idPlayer;

    /*for(const auto& actualConnection : connections){
        if(socket_.remote_endpoint() == actualConnection->socket().remote_endpoint()){
            actualConnection->Close();
        }
        break;
    }*/
    // eliminar del array de conexiones
    connections.erase(
        std::remove_if(
            connections.begin(),
            connections.end(),
            [&](TCPConnection::pointer& c) { return (c->socket().remote_endpoint()==socket_.remote_endpoint()); }),
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

void TCPConnection::SendStartMessage(unsigned char *buff, size_t buffSize){
    socket_.async_send(
        boost::asio::buffer(buff, buffSize),
        boost::bind(
            &TCPConnection::HandleWrite,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}


void TCPConnection::SendFullGame(){
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



void TCPConnection::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred){
    if(!error){
        std::cout << "Mensaje enviado del servidor TCP" << std::endl;
    }else{
        std::cout << "Error al escribir: " << error.message() << std::endl;
    }
    
}
