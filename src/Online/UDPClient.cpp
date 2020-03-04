#include "UDPClient.h"
#include "../src/EventManager/Event.h"
#include "../src/EventManager/EventManager.h"
// #include <src/EventManager/EventManager.h>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../src/Systems/Utils.h"
#include "../src/Systems/Serialization.h"

using boost::asio::ip::udp;
using namespace boost::asio;
using namespace std::chrono;
using namespace std;

UDPClient::UDPClient(string host_, uint16_t port_)
    : context{},
      serverEndpoint{*udp::resolver(context).resolve(udp::v4(), host_, to_string(port_)).begin()},
      socket(context),
      butler{[&]() {
          boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(context.get_executor());
          context.run();
          cout << "///////////////////////// SALIMOS DEL CONTEXT!!! ///////////////////////////////" << endl;
      }} {
    cout << "Server endpoint is " << serverEndpoint.address() << ":" << serverEndpoint.port() << endl;
    socket.open(udp::v4());
    // udp::endpoint localEndpoint = socket.local_endpoint();
    // cout << "Local endpoint is " << socket.local_endpoint().address() << ":" << socket.local_endpoint().port() << endl;

    // EventManager::GetInstance().AddStrand(strand);

    StartReceiving();
}

UDPClient::~UDPClient() {
    context.stop();
    socket.close();
    butler.join();
}

void UDPClient::StartReceiving() {
    // cout << "Esperamos recibir datos" << endl;
    std::shared_ptr<unsigned char[]> recvBuff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    socket.async_receive_from(
        boost::asio::buffer(recvBuff.get(), Constants::ONLINE_BUFFER_SIZE),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleReceived,
            this,
            recvBuff,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::HandleReceived(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    if (!errorCode) {
        size_t currentIndex = 0;
        uint8_t petitionType = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
        int64_t time = Serialization::Deserialize<int64_t>(recevBuff.get(), currentIndex);
        uint16_t idPlayer = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);

        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);
        switch (callType) {
            case Constants::PetitionTypes::SEND_INPUTS: {
                if (time > lastTimeInputReceived[idPlayer]) {
                    const vector<Constants::InputTypes> inputs = Serialization::DeserializeInputs(recevBuff.get(), currentIndex);
                    lastTimeInputReceived[idPlayer] = time;
                    HandleReceivedInputs(inputs, idPlayer);
                }
            } break;

            case Constants::PetitionTypes::SEND_SYNC:
                if (time > lastTimeSyncReceived[idPlayer]) {
                    lastTimeSyncReceived[idPlayer] = time;
                    HandleReceivedSync(recevBuff.get(), bytesTransferred);
                }
                break;

            default:
                cout << "Tipo de petición no contemplada" << endl;
                break;
        }
    } else {
        cout << "¡¡HUBO UN ERROR AL RECIBIR DATOS EN EL CLIENTE!! errorcode:" << errorCode << endl;
    }
    StartReceiving();
}

void UDPClient::HandleReceivedInputs(const vector<Constants::InputTypes> inputs, const uint16_t idRival) const {
    //cout << "Hemos recibido los inputs " << recvdJSON.dump() << endl;
    //vector<Constants::InputTypes> inputs = recvdJSON["inputs"];
    std::shared_ptr<DataMap> data = make_shared<DataMap>();
    (*data)[DataType::ID] = idRival;
    (*data)[DataType::INPUTS] = inputs;
    EventManager::GetInstance().AddEventMulti(Event{EventType::NEW_INPUTS_RECEIVED, data});
    // cout << "Hemos recibido los inputs ";
    // for (size_t i = 0; i < inputs.size(); i++) {
    //     cout << inputs[i] << " ";
    // }
    // cout << endl;
}

// buffer circular
// void UDPClient::HandleReceivedSync(nuevoPaquete) {
//     // sabemos ya que es del coche 2
//     // tenemos que obtener el coche 2
//     Car car2 = getCar(2);

//     // comprobamos en qué pos del array va este paquete nuevo y eliminamos
//     // los anteriores
//     arrayPaquetesCoche2[2] = nuevoPaquete;
//     arrayPaquetesCoche2[1] = null; // son viejos
//     arrayPaquetesCoche2[0] = null; // son viejos

//     coche2.pos = nuevoPaquete.pos;
//     // los 4 paquetes que hay válidos son:
//     // izq izq abajo dch
//     for(Paquete p : arrayPaquetesCoche2) {
//         if(p.arriba)
//             car.accelerate();
//         else if(p.derecha)
//             car.derecha();  // 4.- vamos dch
//         else if(p.izquierda)
//             car.izquierda(); // 1.- vamos izq 2.- vamos izq
//         else if(p.abajo)
//             car.abajo(); // 3.- vamos abajo
//     }
// }

void UDPClient::HandleReceivedSync(unsigned char* recevBuff, size_t bytesTransferred) {
    size_t currentIndex = 0;

    Serialization::Deserialize<uint8_t>(recevBuff, currentIndex); // petition time
    int64_t time = Serialization::Deserialize<int64_t>(recevBuff, currentIndex); 
    uint16_t idCarOnline = Serialization::Deserialize<uint16_t>(recevBuff, currentIndex);

    glm::vec3 posCar = Serialization::DeserializeVec3(recevBuff, currentIndex);
    glm::vec3 rotCar = Serialization::DeserializeVec3(recevBuff, currentIndex);


    typeCPowerUp typePU;
    bool haveTotem;
    bool totemInGround;
    Serialization::DeserializePowerUpTotem(recevBuff, typePU, haveTotem, totemInGround, currentIndex);

    int64_t totemTime = Serialization::Deserialize<int64_t>(recevBuff, currentIndex);
    
    // realizar llamadas al event Manager de manCar
    std::shared_ptr<DataMap> data = make_shared<DataMap>();
    (*data)[DataType::ID_ONLINE] = idCarOnline;
    (*data)[DataType::VEC3_POS] = posCar;
    (*data)[DataType::VEC3_ROT] = rotCar;
    (*data)[DataType::TYPE_POWER_UP] = typePU;
    (*data)[DataType::CAR_WITH_TOTEM] = haveTotem;
    (*data)[DataType::TIME_TOTEM] = totemTime;
    EventManager::GetInstance().AddEventMulti(Event{EventType::NEW_SYNC_RECEIVED_CAR, data});

    glm::vec3 posTotem(0.0, 0.0, 0.0);
    if (totemInGround) {
        posTotem = Serialization::DeserializeVec3(recevBuff, currentIndex);
    }
    std::shared_ptr<DataMap> data2 = make_shared<DataMap>();
    (*data2)[DataType::CAR_WITHOUT_TOTEM] = totemInGround;
    (*data2)[DataType::VEC3_POS] = posTotem;
    EventManager::GetInstance().AddEventMulti(Event{EventType::NEW_SYNC_RECEIVED_TOTEM, data2});
    //std::cout << "RECIBIDO -------------------------------" << std::endl;
    //std::cout << "Id: " << idCarOnline << std::endl;
    //std::cout << "Pos coche: " << posCar.x << " , " << posCar.z << std::endl;
    //std::cout << "Tengo totem: " << haveTotem << std::endl;
    //std::cout << "Tiempo Totem: " << totemTime << std::endl;
    //std::cout << "Totem en suelo: " << totemInGround << std::endl;
    //std::cout << "Pos totem: " << posTotem.x << " , " << posTotem.z << std::endl;
    //std::cout << "----------------------------------------" << std::endl;
    // std::cout << "Recibido sincronizacion: " << bytesTransferred << " bytes" << std::endl;

    cout << Utils::getISOCurrentTimestampMillis() << "he recibido el sync " << time << " de [" << idCarOnline << "] y está en la pos("
         << posCar.x << "," << posCar.y << "," << posCar.z << ") - rot("
         << rotCar.x << "," << rotCar.y << "," << rotCar.z << ")." << endl
         << "Lleva el PU " << (int)typePU << " y lleva el totem(" << haveTotem << ")." << endl
         << "El totem está en el suelo(" << totemInGround << ") y su pos es (" << posTotem.x << "," << posTotem.y << "," << posTotem.z << ") "
         << endl;
}

void UDPClient::SendDateTime() {
    // cout << "Vamos a enviar datos" << endl;
    boost::shared_ptr<string> message(new string(Utils::GetFullDateTime()));
    socket.async_send_to(
        boost::asio::buffer(*message),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSentDateTime,
            this,
            message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::SendInputs(const vector<Constants::InputTypes>& inputs, uint16_t idPlayer) {
    unsigned char requestBuff[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t callType = Constants::PetitionTypes::SEND_INPUTS;
    int64_t time = Utils::getMillisSinceEpoch();
    Serialization::Serialize(requestBuff, &callType, currentBuffSize);
    Serialization::Serialize(requestBuff, &time, currentBuffSize);
    Serialization::Serialize(requestBuff, &idPlayer, currentBuffSize);
    Serialization::SerializeInputs(requestBuff, inputs, currentBuffSize);

    socket.async_send_to(
        boost::asio::buffer(requestBuff, currentBuffSize),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSentInputs,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::HandleSentInputs(const boost::system::error_code& errorCode, std::size_t bytes_transferred) {
    if (errorCode) {
        cout << "Hubo un error enviando los inputs, madafaka" << endl;
        // ResendInput();
    }
}

void UDPClient::SendSync(uint16_t idOnline, const glm::vec3& posCar, const glm::vec3& rotCar, typeCPowerUp tipoPU, bool haveTotem,
                         int64_t totemTime, bool totemInGround, const glm::vec3& posTotem) {
    unsigned char requestBuff[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t callType = Constants::PetitionTypes::SEND_SYNC;
    int64_t time = Utils::getMillisSinceEpoch();

    Serialization::Serialize(requestBuff, &callType, currentBuffSize);
    Serialization::Serialize(requestBuff, &time, currentBuffSize);
    Serialization::Serialize(requestBuff, &idOnline, currentBuffSize);
    Serialization::SerializeVec3(requestBuff, posCar, currentBuffSize);
    Serialization::SerializeVec3(requestBuff, rotCar, currentBuffSize);

    Serialization::SerializePowerUpTotem(requestBuff, tipoPU, haveTotem, totemInGround, currentBuffSize);
    Serialization::Serialize(requestBuff, &totemTime, currentBuffSize);
    if (totemInGround)
        Serialization::SerializeVec3(requestBuff, posTotem, currentBuffSize);  // la pos del totem no se envia siempre
    cout << Utils::getISOCurrentTimestampMillis() << "soy el [" << idOnline << "] estoy enviando el sync " << time << " y estoy en la pos("
         << posCar.x << "," << posCar.y << "," << posCar.z << ") - rot("
         << rotCar.x << "," << rotCar.y << "," << rotCar.z << ")." << endl
         << "Llevo el PU " << (int)tipoPU << " y llevo el totem(" << haveTotem << ")." << endl
         << "El totem está en el suelo(" << totemInGround << ") y su pos es (" << posTotem.x << "," << posTotem.y << "," << posTotem.z << ") "
         << endl;
    socket.async_send_to(
        boost::asio::buffer(requestBuff, currentBuffSize),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSentSync,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::HandleSentSync(const boost::system::error_code& errorCode, std::size_t bytes_transferred) {
    if (errorCode) {
        cout << "Hubo un error enviando la sincronizacion" << endl;
    }
}

void UDPClient::HandleSentDateTime(const boost::shared_ptr<std::string> message,
                                   const boost::system::error_code& errorCode,
                                   std::size_t bytes_transferred) {
    if (!errorCode) {
        cout << "Ya se ha enviado el mensaje, " << message << " madafaka" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
    }
}