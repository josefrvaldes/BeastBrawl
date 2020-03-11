#include "Server.h"

Server::Server()
    : context{make_shared<asio::io_context>()}, serverUDP{make_unique<UDPServer>(*context, SERVER_PORT_UDP)}, serverTCP{make_unique<TCPServer>(*context, SERVER_PORT_TCP, *serverUDP)} {
}

void Server::Start() {
    while (true) {
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guardUDP(context->get_executor());
        serverTCP->StartReceiving();
        // serverUDP->StartReceiving();
        cout << "El servidor está en marcha otra vez!! #############################" << endl;
        context->run();
        GAME_STARTED = false;
        ACCEPTING_ENDGAME = false;
        Restart();
    }
}

void Server::Restart() {
    serverTCP->Close();
    serverUDP->Close();
    Player::nextId = 0;
    context = make_shared<asio::io_context>();
    serverUDP = make_unique<UDPServer>(*context, SERVER_PORT_UDP);
    serverTCP = make_unique<TCPServer>(*context, SERVER_PORT_TCP, *serverUDP);
    Start();
}
