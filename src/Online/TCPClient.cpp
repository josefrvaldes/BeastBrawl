#include "TCPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../../include/include_json/include_json.hpp"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Utils.h"
//#include "../Systems/Serialization.h"


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
        uint16_t idPlayer;
        uint8_t enemiesSize;
        vector<uint16_t> idEnemies;
        size_t currentIndex = 0;

        Utils::Deserialize(&idPlayer, recevBuff.get(), currentIndex);
        Utils::Deserialize(&enemiesSize, recevBuff.get(), currentIndex);
        Utils::DeserializeVector(idEnemies, enemiesSize, recevBuff.get(), currentIndex);
        
        std::shared_ptr<DataMap> data = make_shared<DataMap>();
        (*data)[DataType::ID_ONLINE] = idPlayer;
        (*data)[DataType::VECTOR_ID_ONLINE] = idEnemies;
        EventManager::GetInstance().AddEventMulti(Event{EventType::NEW_TCP_START_MULTI, data});

        std::cout << "El cliente TCP recibe cosas" << std::endl;
    } else if (errorCode) {
        cout << "Hubo un error con código " << errorCode << endl;
    } else {
        cout << "No ha habido error pero bytesTransferred = 0" << endl;
    }
    StartReceiving();
}

void TCPClient::SendConnectionRequest() {
    if (stopped) {
        cout << "Hemos intentado SendConnectionRequest pero el cliente tcp estaba parado" << endl;
        return;
    }

    unsigned char request[Constants::ONLINE_BUFFER_SIZE];
    uint8_t numero = Constants::CONNECTION_REQUEST;
    size_t currentBuffSize = 0;
    Utils::Serialize(request, &numero, currentBuffSize);

    socket.async_send(
        boost::asio::buffer(request, currentBuffSize),
        boost::bind(
            &TCPClient::HandleSentConnectionRequest,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPClient::HandleSentConnectionRequest(const boost::system::error_code& errorCode,
                                            std::size_t bytes_transferred) {
    if (stopped) {
        cout << "Hemos intentado HandleSentConnectionRequest pero el cliente tcp estaba parado" << endl;
        return;
    }

    if (!errorCode) {
        cout << "Mensaje enviado cliente TCP" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje: " << errorCode.message() << endl;
    }
}