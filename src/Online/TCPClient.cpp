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

#include "TCPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Utils.h"
#include "../Systems/Serialization.h"
#include "../GameValues.h"


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

        

        cout << "Enviamos SendConnectionRequest al server" << endl;
        SendConnectionRequest();
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
            case Constants::PetitionTypes::TCP_OPEN_GAME :{
                HandleReceivedOpenGame(recevBuff, bytesTransferred);
            }break;
            case Constants::PetitionTypes::TCP_CHARACTER_REQUEST :{
                HandleReceivedCharReq(recevBuff, bytesTransferred);
            }break;
                case Constants::PetitionTypes::TCP_CHARACTERS_SELECTED :{
                HandleReceivedCharSel(recevBuff, bytesTransferred);
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
    uint8_t charactersSize = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
    vector<uint8_t> characters = Serialization::DeserializeVector<uint8_t>(charactersSize, recevBuff.get(), currentIndex);
    cout << Utils::getISOCurrentTimestampMillis() << " hemos recibido un startGame y voy a ser el id " << idPlayer << endl;
    
    std::shared_ptr<DataMap> data = make_shared<DataMap>();
    (*data)[DataType::ID_ONLINE] = idPlayer;
    (*data)[DataType::VECTOR_ID_ONLINE] = idEnemies;
    (*data)[DataType::CHARACTERS_ONLINE] = characters;
    EventManager::GetInstance().AddEventMulti(Event{EventType::NEW_TCP_START_MULTI, data});
}


void TCPClient::HandleReceivedFullGame(){
    cout << "Se prepara a desconectarme" << "\n";
    EventManager::GetInstance().AddEventMulti(Event{EventType::PREPARE_TO_DISCONNECT});
}

// recibe los jugadores seleccionados
void TCPClient::HandleReceivedOpenGame(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred){
    EventManager::GetInstance().AddEventMulti(Event{EventType::PREPARE_TO_SELECT_CHAR});

    size_t currentIndex = 0;
    Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
    uint8_t charSize = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
    vector<uint8_t> charSelected = Serialization::DeserializeVector<uint8_t>(charSize, recevBuff.get(), currentIndex);

    GameValues::GetInstance()->SetCharacterSel(charSelected);
}


void TCPClient::HandleReceivedCharReq(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred){
    size_t currentIndex = 0;
    Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
    bool alreadySelected = Serialization::Deserialize<bool>(recevBuff.get(), currentIndex);

    if(!alreadySelected){ // cambiamos a la espera
        EventManager::GetInstance().AddEventMulti(Event{EventType::TCP_WAIT_OTHERS});
    }else{  // ya se encuentra seleccionado por lo que no haremos nada o notificaremos al usuario con sonido de error
        EventManager::GetInstance().AddEventMulti(Event{EventType::TCP_SEL_CHAR});
    }
}


// pone los jugadores seleccionados
void TCPClient::HandleReceivedCharSel(std::shared_ptr<unsigned char[]> recevBuff, size_t bytesTransferred){
    size_t currentIndex = 0;
    Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
    uint8_t charSize = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
    vector<uint8_t> charSelected = Serialization::DeserializeVector<uint8_t>(charSize, recevBuff.get(), currentIndex);

    cout << " - Character: "; 
    for(uint8_t cs : charSelected)
        cout << int(cs) << ", ";
    cout << "\n"; 

    GameValues::GetInstance()->SetCharacterSel(charSelected);
}



void TCPClient::SendConnectionRequest() {
    if (stopped) {
        cout << "Hemos intentado SendConnectionRequest pero el cliente tcp estaba parado" << endl;
        return;
    }

    std::shared_ptr<unsigned char[]> request(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::CONNECTION_REQUEST;
    Serialization::Serialize(request.get(), &petitionType, currentBuffSize);

    socket.async_send(
        boost::asio::buffer(request.get(), currentBuffSize),
        boost::bind(
            &TCPClient::HandleSentConnectionRequest,
            this,
            request,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPClient::SendSelCharacterRequest() {
    if (stopped) {
        cout << "Hemos intentado SendCharacterRequest pero el cliente tcp estaba parado" << endl;
        return;
    }

    std::shared_ptr<unsigned char[]> request(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::PetitionTypes::TCP_CHARACTER_REQUEST;
    uint8_t character = GameValues::GetInstance()->GetCharacter();
    Serialization::Serialize(request.get(), &petitionType, currentBuffSize);
    Serialization::Serialize(request.get(), &character, currentBuffSize);

    socket.async_send(
        boost::asio::buffer(request.get(), currentBuffSize),
        boost::bind(
            &TCPClient::HandleSentCharacterRequest,
            this,
            request,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void TCPClient::SendCancelChar(){
    if (stopped) {
        cout << "Hemos intentado SendCancelChar pero el cliente tcp estaba parado" << endl;
        return;
    }

    std::shared_ptr<unsigned char[]> request(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    size_t currentBuffSize = 0;
    uint8_t petitionType = Constants::TCP_CANCEL_CHARACTER;
    Serialization::Serialize(request.get(), &petitionType, currentBuffSize);

    socket.async_send(
        boost::asio::buffer(request.get(), currentBuffSize),
        boost::bind(
            &TCPClient::HandleSentCharacterRequest,
            this,
            request,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}


void TCPClient::HandleSentConnectionRequest(std::shared_ptr<unsigned char[]> request, const boost::system::error_code& errorCode, std::size_t bytes_transferred) {
    if (stopped) {
        cout << "Hemos intentado HandleSentConnectionRequest pero el cliente tcp estaba parado" << endl;
        return;
    }

    if (!errorCode) {
        size_t currentBuffSize = 0;
        uint8_t petitionType = Serialization::Deserialize<uint8_t>(request.get(), currentBuffSize);
        cout << "Mensaje de conexion enviado cliente TCP con petitionType " << unsigned(petitionType) << endl;
    } else {
        cout << "Hubo un error enviando el mensaje de conexion: " << errorCode.message() << endl;
    }
}


void TCPClient::HandleSentCharacterRequest(std::shared_ptr<unsigned char[]> request, const boost::system::error_code& errorCode, std::size_t bytes_transferred) {
    if (stopped) {
        cout << "Hemos intentado HandleSentCahrRequest pero el cliente tcp estaba parado" << endl;
        return;
    }

    if (!errorCode) {
        size_t currentBuffSize = 0;
        uint8_t petitionType = Serialization::Deserialize<uint8_t>(request.get(), currentBuffSize);
        uint8_t personaje = Serialization::Deserialize<uint8_t>(request.get(), currentBuffSize);
        cout << "Mensaje de conexion enviado cliente TCP con petitionType " << unsigned(petitionType) << " y personaje " << unsigned(personaje) << endl;
    } else {
        cout << "Hubo un error enviando el mensaje de conexion: " << errorCode.message() << endl;
    }
}


