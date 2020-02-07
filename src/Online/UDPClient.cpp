#include "UDPClient.h"
#include <src/EventManager/Event.h>
#include <src/EventManager/EventManager.h>
#include <src/Systems/Utils.h>
//#include "src/Systems/Serialization.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>

using boost::asio::ip::udp;
using namespace boost::asio;
using namespace std::chrono;
using namespace std;

UDPClient::UDPClient(string host_, string port_)
    : context{},
      serverEndpoint{*udp::resolver(context).resolve(udp::v4(), host_, port_).begin()},
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

                HandleReceivedInputs(inputs, idRival);
            } break;

            case Constants::PetitionTypes::SEND_INPUT:
                break;

            case Constants::PetitionTypes::SEND_SYNC:
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
    //json j;
    
    //j["petitionType"] = Constants::PetitionTypes::SEND_INPUTS;
    //j["id"] = id;
    //j["inputs"] = inputs;
    //string jsonToBeSent = j.dump();

    
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

void UDPClient::HandleSentInputs(const boost::system::error_code& errorCode,
                                 std::size_t bytes_transferred) {
    if (errorCode) {
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
        // ResendInput();
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