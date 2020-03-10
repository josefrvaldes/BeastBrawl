#include "TCPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../../include/include_json/include_json.hpp"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Utils.h"
#include "../Systems/Serialization.h"


using json = nlohmann::json;

using boost::asio::ip::tcp;
using namespace boost;
using namespace boost::asio;
using namespace std::chrono;
using namespace std;

TCPClient::TCPClient(string host_, uint16_t port_)
    : context{},
      //   endpoints{tcp::resolver(context).resolve(host_, port_)},
      serverEndpoint{*tcp::resolver(context).resolve(tcp::v4(), host_, to_string(port_)).begin()},
      stopped{false},
      socket(context),
      butler{[&]() {
          boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(context.get_executor());
          context.run();
          cout << "///////////////////////// SALIMOS DEL CONTEXT!!! ///////////////////////////////" << endl;
      }} {
    StartConnect(serverEndpoint);
}

TCPClient::~TCPClient() {
    context.stop();
    socket.close();
    butler.join();
}

void TCPClient::Stop() {
    stopped = true;
}

void TCPClient::StartConnect(tcp::endpoint serverEndpoint) {
    socket.async_connect(
        serverEndpoint,
        boost::bind(
            &TCPClient::HandleConnect,
            this,
            boost::asio::placeholders::error));
}

void TCPClient::StartConnect(tcp::resolver::results_type::iterator endpoint_iter) {
    // se trata de conectar el socket
    // if (endpoint_iter != endpoints.end()) {
    //     socket.async_connect(endpoint_iter->endpoint(),
    //                          boost::bind(&TCPClient::HandleConnect,
    //                                      this,
    //                                      boost::asio::placeholders::error,
    //                                      endpoint_iter));
    // } else {
    //     std::cout << "Se cierra el socket" << std::endl;
    //     Stop();
    // }
}

void TCPClient::HandleConnect(const boost::system::error_code& error) {
    if (error) {
        socket.close();
        cout << "No se pudo conectar con el" << endl;
    } else {
        std::cout << "Connectado a servidor, empezamos a recibir" << endl;
        StartReceiving();
    }
}
void TCPClient::HandleConnect(const boost::system::error_code& error, tcp::resolver::results_type::iterator endpoint_iter) {
    if (stopped) {
        cout << "Hemos intentado HandleConnect pero el cliente tcp estaba parado" << endl;
        return;
    }

    if (!socket.is_open()) {
        std::cout << "El socket esta cerrado" << std::endl;
        StartConnect(++endpoint_iter);  // probamos el siguiente endpoint disponible
    } else if (error) {
        std::cout << "Error al conectar: " << error.message() << std::endl;
        socket.close();  // cerramos el socket
        StartConnect(++endpoint_iter);
    } else {
        std::cout << "Connectado a " << endpoint_iter->endpoint() << "\n";
        StartReceiving();
    }
}

void TCPClient::StartReceiving() {
    // udp::endpoint senderEndpoint;
    //cout << "Esperamos recibir datos" << endl;
    //std::shared_ptr<string> recevBuff = make_shared<string>();
    cout << "Estamos en StartReceiving" << endl;

    // std::shared_ptr<boost::array<char, Constants::ONLINE_BUFFER_SIZE>> recevBuff = make_shared<boost::array<char, Constants::ONLINE_BUFFER_SIZE>>();
    // unsigned char *buff[Constants::ONLINE_BUFFER_SIZE];
    std::shared_ptr<unsigned char[]> buff ( new unsigned char[Constants::ONLINE_BUFFER_SIZE] );
    socket.async_receive(
        asio::buffer(buff.get(), Constants::ONLINE_BUFFER_SIZE),
        boost::bind(
            &TCPClient::HandleReceived,
            this,
            buff,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPClient::HandleReceived(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    cout << "Estamos en HandleReceived" << endl;
    if (stopped) {
        cout << "Hemos intentado recibir pero el cliente tcp estaba parado" << endl;
        return;
    }

    if (!errorCode && bytesTransferred > 0) {
        size_t currentIndex = 0;
        uint8_t petitionType = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex); // numero
        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);

        switch (callType){
            case Constants::PetitionTypes::TCP_START_GAME :{
                HandleReceivedStartGame(recevBuff, bytesTransferred);
            }break;
            case Constants::PetitionTypes::TCP_FULL_GAME :{
                HandleReceivedFullGame();
            }break;
            default:
                break;
        }

        std::cout << "El cliente TCP recibe cosas" << std::endl;
    } else if (errorCode) {
        cout << "Hubo un error con código " << errorCode << endl;
    } else {
        cout << "No ha habido error pero bytesTransferred = 0" << endl;
    }
    StartReceiving();
}



void TCPClient::HandleReceivedStartGame(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred){
    size_t currentIndex = 0;

    Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex); // petitionType
    uint16_t idPlayer = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
    uint8_t enemiesSize = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
    vector<uint16_t> idEnemies = Serialization::DeserializeVector<uint16_t>(enemiesSize, recevBuff.get(), currentIndex);
    
    std::shared_ptr<DataMap> data = make_shared<DataMap>();
    (*data)[DataType::ID_ONLINE] = idPlayer;
    (*data)[DataType::VECTOR_ID_ONLINE] = idEnemies;
    EventManager::GetInstance().AddEventMulti(Event{EventType::NEW_TCP_START_MULTI, data});
}


void TCPClient::HandleReceivedFullGame(){
    cout << "Se prepara a desconectarme" << "\n";
    EventManager::GetInstance().AddEventMulti(Event{EventType::PREPARE_TO_DISCONNECT});
}


void TCPClient::SendConnectionRequest() {
    if (stopped) {
        cout << "Hemos intentado SendConnectionRequest pero el cliente tcp estaba parado" << endl;
        return;
    }

    unsigned char request[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::CONNECTION_REQUEST;
    Serialization::Serialize(request, &petitionType, currentBuffSize);

    socket.async_send(
        boost::asio::buffer(request, currentBuffSize),
        boost::bind(
            &TCPClient::HandleSentConnectionRequest,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPClient::HandleSentConnectionRequest(const boost::system::error_code& errorCode, std::size_t bytes_transferred) {
    if (stopped) {
        cout << "Hemos intentado HandleSentConnectionRequest pero el cliente tcp estaba parado" << endl;
        return;
    }

    if (!errorCode) {
        cout << "Mensaje de conexion enviado cliente TCP" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje de conexion: " << errorCode.message() << endl;
    }
}


