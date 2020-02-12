#include "UDPClient.h"
#include "../src/EventManager/Event.h"
#include "../src/EventManager/EventManager.h"
// #include <src/EventManager/EventManager.h>
#include "../src/Systems/Utils.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>

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
    std::shared_ptr<unsigned char[]> recvBuff ( new unsigned char[Constants::ONLINE_BUFFER_SIZE] );
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
        //string receivedString;
        //std::copy(recvBuff->begin(), recvBuff->begin() + bytesTransferred, std::back_inserter(receivedString));
        //json receivedJSON = json::parse(receivedString);
        //uint16_t auxCallType = receivedJSON["petitionType"];
        //const uint32_t id = receivedJSON["id"];
        size_t currentIndex = 0;
        uint8_t petitionType;
        Utils::Deserialize(&petitionType, recevBuff.get(), currentIndex);
        

        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);
        switch (callType) {
            case Constants::PetitionTypes::SEND_INPUTS: {
                uint16_t idRival;
                Utils::Deserialize(&idRival, recevBuff.get(), currentIndex);
                const vector<Constants::InputTypes> inputs = Utils::DeserializeInputs(recevBuff.get(), currentIndex);
                
                // std::cout << "Recibido inputs: " << bytesTransferred << " bytes" << std::endl; 
                HandleReceivedInputs(inputs, idRival);
            } break;

            case Constants::PetitionTypes::SEND_INPUT:
                break;

            case Constants::PetitionTypes::SEND_SYNC:
                HandleReceivedSync(recevBuff.get(), bytesTransferred);
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


void UDPClient::HandleReceivedSync(unsigned char* recevBuff, size_t bytesTransferred) const{
    size_t currentIndex = 0;
    uint8_t petitionType;
    uint16_t idCarOnline;
    typeCPowerUp typePU;
    int64_t totemTime;
    glm::vec3 posTotem(0.0, 0.0, 0.0);
    bool haveTotem;
    bool totemInGround;

    Utils::Deserialize(&petitionType, recevBuff, currentIndex);
    Utils::Deserialize(&idCarOnline, recevBuff, currentIndex);
    glm::vec3 posCar = Utils::DeserializeVec3(recevBuff, currentIndex);
    glm::vec3 rotCar = Utils::DeserializeVec3(recevBuff, currentIndex);

    Utils::DeserializePowerUpTotem(recevBuff, typePU, haveTotem, totemInGround, currentIndex);

    Utils::Deserialize(&totemTime, recevBuff, currentIndex);
    // realizar llamadas al event Manager de manCar
    std::shared_ptr<DataMap> data = make_shared<DataMap>();
    (*data)[DataType::ID_ONLINE] = idCarOnline;
    (*data)[DataType::VEC3_POS] = posCar;
    (*data)[DataType::VEC3_ROT] = rotCar;
    (*data)[DataType::TYPE_POWER_UP] = typePU;
    (*data)[DataType::CAR_WITH_TOTEM] = haveTotem;
    (*data)[DataType::TIME_TOTEM] = totemTime;
    EventManager::GetInstance().AddEventMulti(Event{EventType::NEW_SYNC_RECEIVED_CAR, data});

    if(totemInGround){
        posTotem = Utils::DeserializeVec3(recevBuff, currentIndex);
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
}


void UDPClient::SendDateTime() {
    // cout << "Vamos a enviar datos" << endl;
    boost::shared_ptr<string> message(new string(Utils::GetTime()));
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


void UDPClient::SendInputs(vector<Constants::InputTypes>& inputs, uint16_t id) {
    
    unsigned char requestBuff[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t numero = Constants::PetitionTypes::SEND_INPUTS;
    Utils::Serialize(requestBuff, &numero, currentBuffSize);
    Utils::Serialize(requestBuff, &id, currentBuffSize);
    Utils::SerializeInputs(requestBuff, inputs, currentBuffSize);


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
    }else{
        // std::cout << "Enviados Inputs: " << bytes_transferred << " bytes" << std::endl; 
    }
}


void UDPClient::SendSync(uint16_t idOnline, const glm::vec3 &posCar, const glm::vec3 &rotCar, typeCPowerUp tipoPU, bool haveTotem, 
                            int64_t totemTime, bool totemInGround, const glm::vec3 &posTotem){
    unsigned char requestBuff[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t numero = Constants::PetitionTypes::SEND_SYNC;
    Utils::Serialize(requestBuff, &numero, currentBuffSize);
    Utils::Serialize(requestBuff, &idOnline, currentBuffSize);
    Utils::SerializeVec3(requestBuff, posCar, currentBuffSize);
    Utils::SerializeVec3(requestBuff, rotCar, currentBuffSize);

    Utils::SerializePowerUpTotem(requestBuff, tipoPU, haveTotem, totemInGround, currentBuffSize);
    Utils::Serialize(requestBuff, &totemTime, currentBuffSize);
    if(totemInGround)
        Utils::SerializeVec3(requestBuff, posTotem, currentBuffSize);  // la pos del totem no se envia siempre

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
    }else{
        std::cout << "Enviado sincronizacion: " << bytes_transferred << " bytes" << std::endl; 
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