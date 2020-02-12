#include "TCPConnection.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include "../../include/include_json/include_json.hpp"
#include "../src/Constants.h"
#include "../src/Systems/Utils.h"


using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace std::chrono;

TCPConnection::TCPConnection(asio::io_context& io_context) : socket_(io_context){

}

void TCPConnection::Start(){
    
    //message_ = GetTime();
    //asio::async_write(
    //    socket_,
    //    asio::buffer(message_),
    //    boost::bind(&TCPConnection::HandleWrite,
    //        shared_from_this(),
    //        boost::asio::placeholders::error,
    //        boost::asio::placeholders::bytes_transferred));
    //std::cout << "Direccion: " << socket_.remote_endpoint().address().to_string() << "   Puerto: " << socket_.remote_endpoint().port() << std::endl;

    //std::shared_ptr<boost::array<boost::asio::mutable_buffer,1>> recevBuff = make_shared<boost::array<boost::asio::mutable_buffer,1>>();
    //std::shared_ptr<string> buffer = make_shared<string>();
    //std::shared_ptr<boost::array<char, 512>> recevBuff = make_shared<boost::array<char, 512>>();
    std::shared_ptr<unsigned char[]> recevBuff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    socket_.async_receive(
        asio::buffer(recevBuff.get(), Constants::ONLINE_BUFFER_SIZE),
        boost::bind(&TCPConnection::HandleRead,
            shared_from_this(),
            recevBuff,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    //asio::async_read(
    //    socket_,
    //    asio::buffer(*recevBuff),
    //    boost::bind(&TCPConnection::HandleRead,
    //        shared_from_this(),
    //        recevBuff,
    //        boost::asio::placeholders::error,
    //        boost::asio::placeholders::bytes_transferred));
}



void TCPConnection::HandleRead(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytes_transferred){
    //std::cout << "Hola "<< std::endl;
    if(!error && bytes_transferred!=0){
        //uint16_t idPlayer;
        uint8_t numero;
        //vector<uint16_t> idEnemies;
        size_t currentIndex = 0;

        Utils::Deserialize(&numero, recevBuff.get(), currentIndex);

        
        //string receivedString;
        //std::copy(recevBuff->begin(), recevBuff->begin() + bytes_transferred, std::back_inserter(receivedString));
        //json receivedJSON = json::parse(receivedString);
        //uint16_t auxCallType = receivedJSON["requestConnection"];
        //std::cout << "El servidorTCP lee: "  << auxCallType << std::endl;
        //int ptrCallType = static_cast<int>(*buff1.data());
        //std::cout << "Numero: "<< *buff1.data() << std::endl;
    }else if(error){
        std::cout << "Error al leer: " << error.message() << std::endl;
    }
    Start();
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



void TCPConnection::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred){
    if(!error)
        std::cout << "Mensaje enviado del servidor TCP" << std::endl;
    else
        std::cout << "Error al leer: " << error.message() << std::endl;
    
}
