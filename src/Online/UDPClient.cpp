#include "UDPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../../include/include_json/include_json.hpp"
#include "../Systems/Utils.h"

using json = nlohmann::json;

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
    // udp::endpoint senderEndpoint;
    cout << "Esperamos recibir datos" << endl;
    socket.async_receive_from(
        asio::buffer(recvBuff),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleReceived,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::SendDateTime() {
    // cout << "Vamos a enviar datos" << endl;
    boost::shared_ptr<string> message(new string(GetTime()));
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
    sendBuff[0] = s;
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
    if (!errorCode) {
        Constants::InputTypes* ptrInput = input.get();
        Constants::InputTypes valueInput = *ptrInput;
        cout << Utils::GetTime() << " - Ya se ha enviado el mensaje con input, " << valueInput << " madafaka" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
        // ResendInput();
    }
}

void UDPClient::SendInputs(vector<Constants::InputTypes>& inputs) {
    boost::array<Constants::PetitionTypes, 1> petitionType = {Constants::PetitionTypes::SEND_INPUTS};
    boost::array<mutable_buffer, 2> outputBuffer = {
        boost::asio::buffer(petitionType),
        boost::asio::buffer(inputs)};
    socket.async_send_to(
        boost::asio::buffer(outputBuffer),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSentInputs,
            this,
            inputs,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::HandleSentInputs(const vector<Constants::InputTypes>& inputs, const boost::system::error_code& errorCode,
                                 std::size_t bytes_transferred) {
    if (!errorCode) {
        auto now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(now);
        cout << std::ctime(&end_time) << " - Ya se ha enviado el mensaje con " << inputs.size() << " inputs" << endl;
    } else {
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

void UDPClient::HandleReceived(const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    if (!errorCode) {
        cout << "Hemos recibido el mensaje " << recvBuff.data() << endl;
    } else {
        cout << "Hubo un error con código " << errorCode << endl;
    }
    StartReceiving();
}