#include "UDPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;
// using namespace boost;
using namespace std::chrono;

UDPClient::UDPClient(asio::io_context& context_, string host_, string port_)
    : context{context_}, serverEndpoint{*udp::resolver(context).resolve(udp::v4(), host_, port_).begin()}, socket(context) {
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
            &UDPClient::HandleReceive,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::HandleReceive(const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    if (!errorCode) {
        cout << "Hemos recibido el mensaje " << recvBuff.data() << endl;
    } else {
        cout << "Hubo un error con código " << errorCode << endl;
    }
    StartReceiving();
}

void UDPClient::Send() {
    cout << "Vamos a enviar datos" << endl;
    boost::shared_ptr<string> message(new string(GetTime()));
    socket.async_send_to(
        boost::asio::buffer(*message),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSend,
            this,
            message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::SendInput(InputType input) {
    boost::array<char, 1> sendBuffer = {{input}};
    socket.async_send_to(
        boost::asio::buffer(sendBuffer),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSendInput,
            this,
            input,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::HandleSendInput(InputType input, const boost::system::error_code& errorCode,
                                std::size_t bytes_transferred) {
    if (!errorCode) {
        cout << "Ya se ha enviado el mensaje con input, " << input << " madafaka" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
        // ResendInput();
    }
}

void UDPClient::HandleSend(boost::shared_ptr<std::string> message,
                           const boost::system::error_code& errorCode,
                           std::size_t bytes_transferred) {
    if (!errorCode) {
        cout << "Ya se ha enviado el mensaje, " << message << " madafaka" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
    }
}