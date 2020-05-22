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


#include "Server.h"

Server::Server()
    : context{make_shared<asio::io_context>()}, serverUDP{make_unique<UDPServer>(*context, SERVER_PORT_UDP)}, serverTCP{make_unique<TCPServer>(*context, SERVER_PORT_TCP, *serverUDP)} {
}

void Server::Start() {
    while (true) {
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guardUDP(context->get_executor());
        serverTCP->StartReceiving();
        // serverUDP->StartReceiving();
        //cout << "El servidor está en marcha otra vez!! #############################" << endl;
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
