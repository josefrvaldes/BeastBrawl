#include "UDPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../Systems/Utils.h"


using boost::asio::ip::udp;
using namespace boost;
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

    StartReceiving();
}

void UDPClient::StartReceiving() {
    cout << "Esperamos recibir datos" << endl;
    std::shared_ptr<boost::array<char, 1024>> recvBuff = make_shared<boost::array<char, 1024>>();
    socket.async_receive_from(
        asio::buffer(*recvBuff),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleReceived,
            this,
            recvBuff,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::HandleReceived(std::shared_ptr<boost::array<char, 1024>> recvBuff, const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    if (!errorCode) {
        string receivedString;
        std::copy(recvBuff->begin(), recvBuff->begin() + bytesTransferred, std::back_inserter(receivedString));
        json receivedJSON = json::parse(receivedString);
        uint16_t auxCallType = receivedJSON["petitionType"];
        const uint32_t id = receivedJSON["id"];
        Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(auxCallType);
        switch (callType) {
            case Constants::PetitionTypes::SEND_INPUTS: {
                HandleReceivedInput(receivedJSON, id);
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

void UDPClient::HandleReceivedInput(const json revcdJSON, const uint32_t id) const {
    vector<Constants::InputTypes> inputs = revcdJSON["inputs"];
    cout << "Hemos recibido los inputs ";
    for (size_t i = 0; i < inputs.size(); i++) {
        cout << inputs[i] << " ";
    }
    cout << endl;
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

void UDPClient::SendInput(Constants::InputTypes newInput) {
    std::shared_ptr<Constants::InputTypes> auxInput = make_shared<Constants::InputTypes>(newInput);

    const uint32_t FAKE_ID = 1234;

    json j;
    j["petitionType"] = Constants::PetitionTypes::SEND_INPUT;
    j["id"] = FAKE_ID;
    j["input"] = newInput;

    string s = j.dump();
    socket.async_send_to(
        asio::buffer(s),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSentInput,
            this,
            auxInput,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}

void UDPClient::HandleSentInput(std::shared_ptr<Constants::InputTypes> input, const boost::system::error_code& errorCode,
                                std::size_t bytes_transferred) {
    if (errorCode) {
        Constants::InputTypes* ptrInput = input.get();
        Constants::InputTypes valueInput = *ptrInput;
        cout << Utils::GetTime() << " - Hubo un error enviando el mensaje de input " << valueInput << endl;
    }
}

void UDPClient::SendInputs(vector<Constants::InputTypes>& inputs) {
    const uint32_t FAKE_ID = 1234;

    json j;
    j["petitionType"] = Constants::PetitionTypes::SEND_INPUTS;
    j["id"] = FAKE_ID;
    j["inputs"] = inputs;
    string jsonToBeSent = j.dump();
    std::shared_ptr<vector<Constants::InputTypes>> auxInputs = std::make_shared<vector<Constants::InputTypes>>(inputs);
    socket.async_send_to(
        asio::buffer(jsonToBeSent),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSentInputs,
            this,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
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