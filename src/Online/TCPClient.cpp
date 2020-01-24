#include "TCPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../../include/include_json/include_json.hpp"
#include "../Systems/Utils.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

using json = nlohmann::json;

using boost::asio::ip::tcp;
using namespace boost;
using namespace boost::asio;
using namespace std::chrono;
using namespace std;


TCPClient::TCPClient(string host_, string port_)
    : context{},
      endpoints{tcp::resolver(context).resolve(host_, port_)},
      stopped{false},
      socket(context),
      butler{[&]() {
          boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(context.get_executor());
          context.run();
          cout << "///////////////////////// SALIMOS DEL CONTEXT!!! ///////////////////////////////" << endl;
      }} {

    StartConnect(endpoints.begin());
}

TCPClient::~TCPClient(){
    context.stop();
    socket.close();
    butler.join();  

}

void TCPClient::Stop(){
    stopped = true;
}



void TCPClient::StartConnect(tcp::resolver::results_type::iterator endpoint_iter){
    // se trata de conectar el socket
    if (endpoint_iter != endpoints.end()){
        socket.async_connect(endpoint_iter->endpoint(),
            boost::bind(&TCPClient::HandleConnect,
                this, 
                boost::asio::placeholders::error, 
                endpoint_iter));
    }else{
        std::cout << "Se cierra el socket" << std::endl;
        Stop();
    }
}


void TCPClient::HandleConnect(const boost::system::error_code& error, tcp::resolver::results_type::iterator endpoint_iter){
    if(stopped)
        return;

    if (!socket.is_open()){
        std::cout << "El socket esta cerrado" << std::endl;
        StartConnect(++endpoint_iter);    // probamos el siguiente endpoint disponible
    }else if (error){
        std::cout << "Error al conectar: " << error.message() << std::endl;
        socket.close();                   // cerramos el socket
        StartConnect(++endpoint_iter);
    }else{
        std::cout << "Connectado a " << endpoint_iter->endpoint() << "\n";
        StartReceiving();
    }
}




void TCPClient::StartReceiving() {
    // udp::endpoint senderEndpoint;
    //cout << "Esperamos recibir datos" << endl;
    //std::shared_ptr<string> recevBuff = make_shared<string>();
    std::shared_ptr<boost::array<char, 1024>> recevBuff = make_shared<boost::array<char, 1024>>();
    socket.async_receive(
        asio::buffer(*recevBuff),
        boost::bind(
            &TCPClient::HandleReceived,
            this,
            recevBuff,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}





void TCPClient::HandleReceived(std::shared_ptr<boost::array<char, 1024>> recevBuff, const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    if(stopped)
        return;
    
    if (!errorCode && bytesTransferred > 0) {
        string receivedString;
        std::copy(recevBuff->begin(), recevBuff->begin() + bytesTransferred, std::back_inserter(receivedString));

        std::shared_ptr<DataMap> data = make_shared<DataMap>();
        (*data)["dataServer"] = receivedString;
        EventManager::GetInstance().AddEventMulti(Event{EventType::NEW_TCP_START_MULTI, data});

        json receivedJSON = json::parse(receivedString);
        uint32_t idPlayer = receivedJSON["idPlayer"];
        vector<uint32_t> arrayIdEnemies = receivedJSON["idEnemies"];
        std::cout << "El cliente TCP recibe: "  << receivedString << std::endl;
    } else if(errorCode) {
        cout << "Hubo un error con código " << errorCode << endl;
    }
    StartReceiving();
}



void TCPClient::SendConnectionRequest() {
    if(stopped)
        return;

    //json j;
    //j["petitionType"] = Constants::PetitionTypes::SEND_INPUTS;
    //j["id"] = id;
    //j["inputs"] = inputs;
    //string jsonToBeSent = j.dump();
    // cout << "Vamos a enviar datos" << endl;
    uint16_t numero = 1;
    //sendBuff2[0] = boost::asio::buffer(&numero, sizeof(numero));
    json j;
    j["requestConnection"] = numero;

    string s = j.dump();
    socket.async_send(
        boost::asio::buffer(s, s.size()),
        boost::bind(
            &TCPClient::HandleSentConnectionRequest,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}


void TCPClient::HandleSentConnectionRequest(const boost::system::error_code& errorCode,
                                   std::size_t bytes_transferred) {
    if(stopped)
        return;
    
    if (!errorCode) {
        cout << "Mensaje enviado cliente TCP" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje: " << errorCode.message() << endl;
    }
}