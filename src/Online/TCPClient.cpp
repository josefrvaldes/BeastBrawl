#include "TCPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../../include/include_json/include_json.hpp"
#include "../Systems/Utils.h"

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
    //context.run();
    cout << "///////////////////////// SALIMOS DEL CONTEXT!!! ///////////////////////////////" << endl;
}

TCPClient::~TCPClient(){
    context.stop();
    socket.close();
    butler.join();  

}

void TCPClient::Stop(){
    /*stopped = true;
    boost::system::error_code ignored_ec;
    socket.close(ignored_ec);*/
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
    socket.async_receive(
        asio::buffer(recvBuff),
        boost::bind(
            &TCPClient::HandleReceived,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}





void TCPClient::HandleReceived(const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    if(stopped)
        return;
    
    if (!errorCode && bytesTransferred > 0) {
        cout << "Hemos recibido el mensaje " << bytesTransferred << endl;
    } else if(errorCode) {
        cout << "Hubo un error con código " << errorCode << endl;
    }
    StartReceiving();
}



void TCPClient::SendDateTime() {
    if(stopped)
        return;

    // cout << "Vamos a enviar datos" << endl;
    boost::shared_ptr<string> message(new string("Hola"));
    socket.async_send(
        boost::asio::buffer(*message),
        boost::bind(
            &TCPClient::HandleSentDateTime,
            this,
            message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}


void TCPClient::HandleSentDateTime(const boost::shared_ptr<std::string> message,
                                   const boost::system::error_code& errorCode,
                                   std::size_t bytes_transferred) {
    if(stopped)
        return;
    
    if (!errorCode) {
        cout << "Ya se ha enviado el mensaje, " << *message.get() << endl;
    } else {
        cout << "Hubo un error enviando el mensaje " << errorCode.message() << endl;
    }
}