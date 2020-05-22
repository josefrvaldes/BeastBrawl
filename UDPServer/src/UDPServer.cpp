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


#include "UDPServer.h"

#include <algorithm>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#include "../../src/Constants.h"
#include "../../src/Systems/Serialization.h"
#include "../../src/Systems/Utils.h"
#include "Server.h"

using boost::asio::ip::udp;
using namespace std::chrono;

UDPServer::UDPServer(boost::asio::io_context& context_, uint16_t port_)
    : context{context_}, socket(context_, udp::endpoint(udp::v4(), port_)), timer{make_unique<boost::asio::steady_timer>(context, boost::asio::chrono::seconds(TIME_BETWEEN_DISCONNECTION_CHECKS))} {
}

UDPServer::~UDPServer() {
    //cout << "Se ha llamado al destructor de UDPServer" << endl;
}

void UDPServer::Close() {
    socket.close();
}

void UDPServer::ResetTimerStartReceiving() {
    timeServerStartedReceiving = Utils::getMillisSinceEpoch();
}

void UDPServer::StartReceiving() {
    std::shared_ptr<unsigned char[]> recevBuff(new unsigned char[Constants::ONLINE_BUFFER_SIZE]);
    std::shared_ptr<udp::endpoint> receiverEndpoint = make_shared<udp::endpoint>();
    socket.async_receive_from(
        asio::buffer(recevBuff.get(), Constants::ONLINE_BUFFER_SIZE),
        *receiverEndpoint,
        boost::bind(
            &UDPServer::HandleReceive,
            this,
            recevBuff,
            receiverEndpoint,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPServer::HandleReceive(std::shared_ptr<unsigned char[]> recevBuff, std::shared_ptr<udp::endpoint> remoteClient, const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    if (!errorCode) {
        size_t currentIndex = 0;
        uint8_t petitionType = Serialization::Deserialize<uint8_t>(recevBuff.get(), currentIndex);
        int64_t time = Serialization::Deserialize<int64_t>(recevBuff.get(), currentIndex);

        // solo aceptamos peticiones nuevas, es decir, peticiones que salieron después de que
        // el servidor empezase a escuchar peticiones. Con esto nos evitamos recibir peticiones
        // de una ejecución anterior del juego
        if (time > timeServerStartedReceiving) {
            uint16_t idPlayer = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
            //cout << Utils::getISOCurrentTimestampMillis() << " hemos recibido una petición de type[" << unsigned(petitionType) << "] del jugador[" << idPlayer << "] con ip["<<remoteClient->address().to_string() << ":" << to_string(remoteClient->port()) << "]" << endl;
            //cout << Utils::getISOCurrentTimestampMillis() << " Hemos recibido en el server la llamada " << time << " de tipo " << unsigned(petitionType) << " del user " << idPlayer << endl;
            // TODO: esto creo que podría evitarse
            unsigned char buffRecieved[Constants::ONLINE_BUFFER_SIZE];
            memcpy(&buffRecieved[0], &recevBuff.get()[0], bytesTransferred);

            SavePlayerIfNotExists(idPlayer, *remoteClient.get());

            Constants::PetitionTypes callType = static_cast<Constants::PetitionTypes>(petitionType);
            auto player = GetPlayerById(idPlayer);
            if (player != nullptr) {
                Player& p = *player;
                switch (callType) {
                    case Constants::PetitionTypes::SEND_INPUTS: {
                        //std::cout << "Recibidos inputs: " << bytesTransferred << std::endl;
                        // int64_t gameTime = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                        if (p.lastInputTimeReceived < time) {
                            //cout << Utils::getISOCurrentTimestampMillis() << "Se ha recibido y reenviado un paquete de input del player " << idPlayer << " con time["<<time<<"] " << endl;
                            p.lastInputTimeReceived = time;
                            HandleReceivedInputs(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                        } 
                    } break;
                    case Constants::PetitionTypes::SEND_SYNC: {
                        // int64_t gameTime = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                        
                        //std::cout << "Recibida sincronizacion: " << bytesTransferred << std::endl;
                        if (p.lastSyncTimeReceived < time) {
                            //cout << Utils::getISOCurrentTimestampMillis() << "Se ha recibido y reenviado un paquete de sync del player " << idPlayer << endl;
                            p.lastSyncTimeReceived = time;
                            HandleReceivedSync(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                        } 
                    } break;
                    case Constants::PetitionTypes::CATCH_PU: {
                        if (p.lastCatchPUTimeReceived < time) {
                            p.lastCatchPUTimeReceived = time;
                            HandleReceivedCatchPU(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                        } else {
                            //cout << Utils::getISOCurrentTimestampMillis() << "Se ha ignorado un paquete de CatchPU porque era antiguo" << endl;
                        }
                    } break;
                    case Constants::PetitionTypes::CATCH_TOTEM: {
                        if (p.lastCatchTotemTimeReceived < time) {
                            p.lastCatchTotemTimeReceived = time;
                            HandleReceivedCatchTotem(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                        } //else {
                            //cout << Utils::getISOCurrentTimestampMillis() << "Se ha ignorado un paquete de CatchTotem porque era antiguo" << endl;
                        //}
                    } break;
                    case Constants::PetitionTypes::LOST_TOTEM: {
                        if (p.lastLostTotemTimeReceived < time) {
                            p.lastLostTotemTimeReceived = time;
                            HandleReceivedLostTotem(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                        } else {
                            //cout << Utils::getISOCurrentTimestampMillis() << "Se ha ignorado un paquete de lostTotem porque era antiguo" << endl;
                        }
                    } break;
                    case Constants::PetitionTypes::USED_ROBOJOROBO: {
                        if (p.lastUsedRoboJoroboTimeReceived < time) {
                            p.lastUsedRoboJoroboTimeReceived = time;
                            HandleReceivedUsedRoboJorobo(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                        } else {
                            //cout << Utils::getISOCurrentTimestampMillis() << "Se ha ignorado un paquete de lostTotem porque era antiguo" << endl;
                        }
                    } break;
                    case Constants::PetitionTypes::COLLIDE_NITRO: {
                        if (p.lastCollideNitroTimeReceived < time) {
                            p.lastCollideNitroTimeReceived = time;
                            HandleReceivedCollideNitro(idPlayer, buffRecieved, bytesTransferred, *remoteClient.get());
                        } else {
                            //cout << Utils::getISOCurrentTimestampMillis() << "Se ha ignorado un paquete de lostTotem porque era antiguo" << endl;
                        }
                    } break;
                    case Constants::PetitionTypes::ENDGAME: {
                        if (Server::ACCEPTING_ENDGAME) {
                            // cout << "Hemos recibido una petición de ENDGAME! vamos a reinciar el server!! ###########################" << endl;
                            Exit();
                        } else {
                            // cout << "Hemos recibido una petición de ENDGAME! pero la ignoramos!! ###########################" << endl;
                        }
                    } break;
                    case Constants::PetitionTypes::SEND_THROW_TELEBANANA:
                    case Constants::PetitionTypes::SEND_THROW_MELON_O_PUDIN:
                        if (p.lastThrowPUReceived < time) {
                            p.lastThrowPUReceived = time;
                            uint16_t idPUOnline = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                            HandleReceivedThrowPU(idPlayer, idPUOnline, buffRecieved, bytesTransferred, *remoteClient.get());
                        } else {
                            //cout << Utils::getISOCurrentTimestampMillis() << "Se ha ignorado un paquete de lostTotem porque era antiguo" << endl;
                        }
                        break;
                    case Constants::PetitionTypes::SEND_CRASH_PU_CAR:
                        if (p.lastCrashPUCarReceived < time) {
                            uint16_t idPowerUp = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                            uint16_t idCar = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                            HandleReceivedCrashPUCar(idPlayer, idPowerUp, idCar, buffRecieved, bytesTransferred, *remoteClient.get());
                        }
                        break;
                    case Constants::PetitionTypes::SEND_CRASH_PU_WALL:
                        if (p.lastCrashPUWallReceived < time) {
                            uint16_t idPowerUp = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                            HandleReceivedCrashPUWall(idPlayer, idPowerUp, buffRecieved, bytesTransferred, *remoteClient.get());
                        }
                        break;
                    case Constants::PetitionTypes::LAUNCH_ANIMATION_END:
                        if (p.lastLaunchAnimationEndReceived < time && animationEndRaceLaunched == false) {
                            p.lastLaunchAnimationEndReceived = time;
                            uint16_t idWinner = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                            HandleReceivedLaunchAnimationEnd(idPlayer, idWinner, buffRecieved, bytesTransferred, *remoteClient.get());
                        }
                        break;
                    case Constants::PetitionTypes::WAITING_FOR_COUNTDOWN:
                        if (p.lastWaitingForCountdownReceived < time) {
                            p.lastWaitingForCountdownReceived = time;
                            HandleReceivedWaitingForCountdown(p, buffRecieved, bytesTransferred, *remoteClient.get());
                        }
                        break;
                    case Constants::PetitionTypes::SEND_CLOCK_SYNC:
                        if (p.lastClockSyncReceived < time) {
                            p.lastClockSyncReceived = time;
                            uint16_t idOnline2 = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                            //cout << "Hemos recibido un sendClockSync del sender["<<idPlayer<<"] y receiver["<<idOnline2<<"]" << endl;
                            HandleReceivedClockSync(p, idOnline2, buffRecieved, bytesTransferred, *remoteClient.get());
                        } //else {
                            //cout << "Hemos descartado un sendClockSync del sender["<<idPlayer<<"]" << endl;
                        //}
                        break;
                    case Constants::PetitionTypes::SEND_FINAL_CLOCK_SYNC:
                        if (p.lastFinalClockSyncReceived < time) {
                            p.lastFinalClockSyncReceived = time;
                            p.lastInputTimeReceived = -1;
                            p.lastSyncTimeReceived = -1;
                            uint16_t idOnline2 = Serialization::Deserialize<uint16_t>(recevBuff.get(), currentIndex);
                            //cout << "Hemos recibido un sendClockSync del sender["<<idPlayer<<"] y receiver["<<idOnline2<<"]" << endl;
                            HandleReceivedFinalClockSync(p, idOnline2, buffRecieved, bytesTransferred, *remoteClient.get());
                        } //else {
                            //cout << "Hemos descartado un sendClockSync del sender["<<idPlayer<<"]" << endl;
                        //}
                        break;
                    default:
                        //cout << "Petición incorrecta" << endl;
                        break;
                }

                // metodo para detectar si algún paquete no ha llegado desde hace tiempo
                //DetectUsersDisconnected();
            } //else {
                //cout << idPlayer << " - No se ha encontrado el player que corresponde con esta llamada. CATÁSTROFE" << endl;
            //}
        }

    } //else {
        //cout << "¡¡HUBO UN ERROR AL RECIBIR DATOS EN EL SERVER!! errorcode:" << errorCode << endl;
    //}
    StartReceiving();  // antes estaba dentro del if, pero entonces si hay un error ya se rompe tó ¿?
}

void UDPServer::HandleReceivedInputs(const uint16_t id, const unsigned char resendInputs[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    ResendBytesToOthers(id, resendInputs, currentBufferSize, originalClient);
}

void UDPServer::HandleReceivedCatchPU(const uint16_t id, unsigned char resendPU[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    ResendBytesToOthers(id, resendPU, currentBufferSize, originalClient);
}

void UDPServer::HandleReceivedThrowPU(const uint16_t id, const uint16_t idPUOnline, unsigned char resendPU[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    // el id del melón es el id del user concatenado con el id real
    idsPUs.push_back(idPUOnline);
    //std::cout << "Hemos creado un PU con id " << idPUOnline << ", y lo hemos guardado así que ahora tenemos " << idsPUs.size() << endl;

    for (uint8_t i = 0; i < NUM_REINTENTOS; i++)
        ResendBytesToOthers(id, resendPU, currentBufferSize, originalClient);
}

void UDPServer::HandleReceivedCrashPUWall(const uint16_t idPlayer, const uint16_t idPowerUp, unsigned char resendPU[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    // si tenemos en nuestro vector de PUs el pu que acaba de chocar, entonces
    // operamos con él, si no, significa que ya ha chocado antes y no operamos
    if (std::binary_search(idsPUs.begin(), idsPUs.end(), idPowerUp)) {
        //std::cout << "Hemos recibido un choque de PU-Wall idPowerUp " << idPowerUp << ". Antes teníamos " << idsPUs.size();
        idsPUs.erase(
            std::remove_if(
                idsPUs.begin(),
                idsPUs.end(),
                [idPowerUp](const uint16_t currentIdPU) { return currentIdPU == idPowerUp; }),
            idsPUs.end());
        //std::cout << " y ahora tenemos " << idsPUs.size() << endl;

        for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
            for (Player& currentPlayer : players)
                SendBytes(resendPU, currentBufferSize, currentPlayer);
    }
}

void UDPServer::HandleReceivedCrashPUCar(const uint16_t idPlayer, const uint16_t idPowerUp, const uint16_t idCarCrashed, unsigned char resendPU[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    // si tenemos en nuestro vector de PUs el pu que acaba de chocar, entonces
    // operamos con él, si no, significa que ya ha chocado antes y no operamos
    //cout << Utils::getISOCurrentTimestampMillis() << "El coche " << idPlayer << " dice que hemos chocado con el PU-Car con el pu[" << idPowerUp << "] car[" << idCarCrashed << "], vamos a ver si está en la lista" << endl;
    //cout << Utils::getISOCurrentTimestampMillis() << "Hemos chocado con el PU-Car con el pu[" << idPowerUp << "] car[" << idCarCrashed << "], vamos a ver si está en la lista" << endl;

    bool encontrado = false;
    for (size_t i = 0; i < idsPUs.size(); i++) {
        if (idsPUs[i] == idPowerUp) {
            encontrado = true;
            break;
        }
    }

    if (encontrado) {
        //cout << Utils::getISOCurrentTimestampMillis() << "Lo hemos encontrado, así que vamos a borrarlo" << idsPUs.size() << endl;
        idsPUs.erase(
            std::remove_if(
                idsPUs.begin(),
                idsPUs.end(),
                [idPowerUp](const uint16_t currentIdPU) { return currentIdPU == idPowerUp; }),
            idsPUs.end());
        //std::cout << " y ahora tenemos " << idsPUs.size() << endl;

        for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
            for (Player& currentPlayer : players)
                SendBytes(resendPU, currentBufferSize, currentPlayer);
    } //else {
        //cout << Utils::getISOCurrentTimestampMillis() << "NOOOO Lo hemos encontrado" << idsPUs.size() << endl;
    //}
}

void UDPServer::HandleReceivedLaunchAnimationEnd(const uint16_t idPlayer, const uint16_t idWinner, unsigned char bufferToReSend[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    //cout << Utils::getISOCurrentTimestampMillis() << "El coche " << idPlayer << " dice que hemos acabado la carrera y que ha ganado [" << idWinner << "]" << endl;

    for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
        for (Player& currentPlayer : players)
            SendBytes(bufferToReSend, currentBufferSize, currentPlayer);

    animationEndRaceLaunched = true;
}

void UDPServer::HandleReceivedWaitingForCountdown(Player& p, unsigned char bufferToReSend[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    if (!animationCountdownLaunched) {
        //cout << Utils::getISOCurrentTimestampMillis() << "El coche " << p.id << " dice que está waiting for countdown\n";
        // marcamos que este coche ya está listo para la cuenta atrás
        p.waitingForCountdown = true;

        // buscamos si todos los coches están listos, si alguno todavía no está listo, abortamos misión
        bool launchCountdown = true;
        for (Player& currentPlayer : players) {
            if (!currentPlayer.waitingForCountdown) {
                launchCountdown = false;
                break;
            }
        }

        // si todos estaban ya listos y esperando para comenzar la cuenta atrás, efectivamente, les enviamos que empiecen la cuenta atrás
        if (launchCountdown) {
            //cout << "Vamos a decirle a los clientes que empiecen el countdown" << endl;
            animationCountdownLaunched = true;
            for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
                for (Player& currentPlayer : players) {
                    SendLaunchAnimationCountdown(currentPlayer);
                }
        }
    }
}

void UDPServer::HandleReceivedClockSync(Player& p, uint16_t idOnline2, unsigned char bufferToReSend[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    Player *p2 = GetPlayerById(idOnline2);
    if(p2 != nullptr) {
        for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
            SendBytes(bufferToReSend, currentBufferSize, *p2);       
    } //else {
        //cout << "No hemos encontrado el jugador con idSender[" << idOnline2 << "] al que hay que reenviarle un SyncClock" << endl;
    //}
}

void UDPServer::HandleReceivedFinalClockSync(Player& p, uint16_t idOnline2, unsigned char bufferToReSend[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    Player *p2 = GetPlayerById(idOnline2);
    if(p2 != nullptr) {
        p2->lastInputTimeReceived = -1;
        p2->lastSyncTimeReceived = -1;
        for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
            SendBytes(bufferToReSend, currentBufferSize, *p2);       
    } //else {
        //cout << "No hemos encontrado el jugador con idSender[" << idOnline2 << "] al que hay que reenviarle un SyncClock" << endl;
    //}
}

void UDPServer::SendLaunchAnimationCountdown(const Player& player) {
    unsigned char requestBuff[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t callType = Constants::PetitionTypes::LAUNCH_ANIMATION_COUNTDOWN;
    int64_t time = Utils::getMillisSinceEpoch();

    Serialization::Serialize(requestBuff, &callType, currentBuffSize);
    Serialization::Serialize(requestBuff, &time, currentBuffSize);
    Serialization::Serialize(requestBuff, &player.id, currentBuffSize);
    SendBytes(requestBuff, currentBuffSize, player);
}

void UDPServer::HandleReceivedCatchTotem(const uint16_t id, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient) {
    if (playerWithTotem == Constants::ANY_PLAYER) {  // en caso de que nadie tuviese el totem
        size_t currentIndex = 0;
        Serialization::Deserialize<uint8_t>(buffer, currentIndex);  // petitionType
        /*int64_t time = */ Serialization::Deserialize<int64_t>(buffer, currentIndex);
        /*uint16_t idCarOnline = */ Serialization::Deserialize<uint16_t>(buffer, currentIndex);
        uint16_t idCarCatchTotem = Serialization::Deserialize<uint16_t>(buffer, currentIndex);

        playerWithTotem = idCarCatchTotem;

        for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
            for (Player& currentPlayer : players)
                SendBytes(buffer, currentBufferSize, currentPlayer);
    }
}

void UDPServer::HandleReceivedLostTotem(const uint16_t id, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient) {
    // se comprueba que quien lo pierde es quien realmente lo tiene
    size_t currentIndex = 0;
    Serialization::Deserialize<uint8_t>(buffer, currentIndex);  // petitionType
    /*int64_t time = */ Serialization::Deserialize<int64_t>(buffer, currentIndex);
    /*uint16_t idCarOnline = */ Serialization::Deserialize<uint16_t>(buffer, currentIndex);
    uint16_t idCarLostTotem = Serialization::Deserialize<uint16_t>(buffer, currentIndex);

    if (playerWithTotem != Constants::ANY_PLAYER && playerWithTotem == idCarLostTotem) {  // en caso de que alguien tubiese el totem
        playerWithTotem = Constants::ANY_PLAYER;

        for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
            for (Player& currentPlayer : players)
                SendBytes(buffer, currentBufferSize, currentPlayer);
    }
}

void UDPServer::HandleReceivedUsedRoboJorobo(const uint16_t id, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient) {
    // se comprueba que quien lo pierde es quien realmente lo tiene
    size_t currentIndex = 0;
    Serialization::Deserialize<uint8_t>(buffer, currentIndex);  // petitionType
    int64_t time = Serialization::Deserialize<int64_t>(buffer, currentIndex);
    uint16_t idCarOnline = Serialization::Deserialize<uint16_t>(buffer, currentIndex);

    if (playerWithTotem != Constants::ANY_PLAYER && playerWithTotem != idCarOnline) {  // en caso de que alguien tubiese el totem otro
        unsigned char requestBuff[Constants::ONLINE_BUFFER_SIZE];
        size_t currentBuffSize = 0;
        uint8_t callType = Constants::PetitionTypes::USED_ROBOJOROBO;

        Serialization::Serialize(requestBuff, &callType, currentBuffSize);
        Serialization::Serialize(requestBuff, &time, currentBuffSize);
        Serialization::Serialize(requestBuff, &idCarOnline, currentBuffSize);
        Serialization::Serialize(requestBuff, &playerWithTotem, currentBuffSize);  // a quien se lo he robado

        playerWithTotem = idCarOnline;

        for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
            for (Player& currentPlayer : players)
                SendBytes(requestBuff, currentBuffSize, currentPlayer);
    }
}

void UDPServer::HandleReceivedCollideNitro(const uint16_t id, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient) {
    // se comprueba que quien lo pierde es quien realmente lo tiene
    size_t currentIndex = 0;
    Serialization::Deserialize<uint8_t>(buffer, currentIndex);  // petitionType
    /*int64_t time = */ Serialization::Deserialize<int64_t>(buffer, currentIndex);
    /*uint16_t idCarOnline = */ Serialization::Deserialize<uint16_t>(buffer, currentIndex);
    uint16_t idCarWithTotem = Serialization::Deserialize<uint16_t>(buffer, currentIndex);
    uint16_t idCarWithNitro = Serialization::Deserialize<uint16_t>(buffer, currentIndex);

    if (playerWithTotem != Constants::ANY_PLAYER && playerWithTotem == idCarWithTotem) {  // en caso de que alguien tubiese el totem otro
        playerWithTotem = idCarWithNitro;

        for (uint8_t i = 0; i < NUM_REINTENTOS; ++i)
            for (Player& currentPlayer : players)
                SendBytes(buffer, currentBufferSize, currentPlayer);
    }
}

void UDPServer::HandleReceivedSync(const uint16_t id, unsigned char recevBuff[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    size_t currentIndex = 0;
    typeCPowerUp typePU;

    Serialization::Deserialize<uint8_t>(recevBuff, currentIndex);  // petitionType
                                                                   // código de depuración
    /*int64_t time = */ Serialization::Deserialize<int64_t>(recevBuff, currentIndex);
    /*uint16_t idCarOnline = */ Serialization::Deserialize<uint16_t>(recevBuff, currentIndex);
    /*glm::vec3 posCar = */ Serialization::DeserializeVec3(recevBuff, currentIndex);
    /*glm::vec3 rotCar = */ Serialization::DeserializeVec3(recevBuff, currentIndex);
    /*float speed = */ Serialization::Deserialize<float>(recevBuff, currentIndex);
    /*float wheelRotation = */ Serialization::Deserialize<float>(recevBuff, currentIndex);
    /*float skidDeg = */ Serialization::Deserialize<float>(recevBuff, currentIndex);
    /*float skidRotation = */ Serialization::Deserialize<float>(recevBuff, currentIndex);

    glm::vec3 posTotem(0.0, 0.0, 0.0);
    bool haveTotem;
    bool totemInGround;
    Serialization::DeserializePowerUpTotem(recevBuff, typePU, haveTotem, totemInGround, currentIndex);

    /*int64_t totemTime = */ Serialization::Deserialize<int64_t>(recevBuff, currentIndex);  // totemTime

    if (totemInGround) {
        posTotem = Serialization::DeserializeVec3(recevBuff, currentIndex);
    }

    // código de depuración
    /*cout << Utils::getISOCurrentTimestampMillis() << "he recibido el sync " << time << " de [" << idCarOnline << "," << id << "] y está en la pos("
         << posCar.x << "," << posCar.y << "," << posCar.z << ") - rot("
         << rotCar.x << "," << rotCar.y << "," << rotCar.z << ")." << endl
         << "Lleva el PU " << (int)typePU << " y lleva el totem(" << haveTotem << ")." << endl
         << "El totem está en el suelo(" << totemInGround << ") y su pos es (" << posTotem.x << "," << posTotem.y << "," << posTotem.z << ") "
         << endl;*/

    ResendBytesToOthers(id, recevBuff, currentBufferSize, originalClient);
}

void UDPServer::ResendBytesToOthers(const uint16_t id, const unsigned char resendBytes[], const size_t currentBufferSize, const udp::endpoint& originalClient) {
    // string originalAddress = originalClient.address().to_string();
    // uint16_t originalPort = originalClient.port();
    // cout << "Vamos a reenviar bytes del cliente [" << id << "] y que tiene la dirección " << originalAddress << ":" << originalPort << " a los demás" << endl;

    for (Player& currentPlayer : players) {
        if (currentPlayer.id != id) {
            // const auto& currentEndpoint = currentPlayer.endpoint;
            // const auto& currentAddress = currentEndpoint.address().to_string();
            // const auto& currentPort = currentEndpoint.port();
            //cout << "Vamos a reenviar bytes del cliente [" << id << "] a [" << currentPlayer.id << "] " << currentAddress << ":" << currentPort << " a los demás" << endl;
            SendBytes(resendBytes, currentBufferSize, currentPlayer);
        }
    }
}

void UDPServer::SendBytes(const unsigned char resendBytes[], const size_t currentBufferSize, const Player& player) {
    //std::shared_ptr<string> message = make_shared<string>(s);
    socket.async_send_to(
        boost::asio::buffer(resendBytes, currentBufferSize),
        player.endpoint,
        boost::bind(
            &UDPServer::HandleSentBytes,
            this,
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}

void UDPServer::HandleSentBytes(const boost::system::error_code& errorCode, std::size_t bytesTransferred) const {
    if (errorCode) {
        //cout << "Hubo un error enviando Bytes. errorcode: " << errorCode << endl;
        // Resend();
    }
}

void UDPServer::SavePlayerIfNotExists(const uint16_t id, udp::endpoint& newClient) {
    string newAddress = newClient.address().to_string();
    uint16_t newPort = newClient.port();
    for (Player& currentPlayer : players) {
        // si el cliente ya lo tenemos guardado, nos salimos
        // string currentAddress = currentPlayer.endpoint.address().to_string();
        // uint16_t currentPort = currentPlayer.endpoint.port();
        if (currentPlayer.id == id)
            return;
    }
    Player p;
    p.id = id;
    p.endpoint = newClient;
    players.push_back(p);
    //cout << "Hemos guardado al cliente con id " << id << " y que tiene la dirección " << newAddress << ":" << newPort << " y ahora tenemos " << players.size() << " clientes" << endl;
}

Player* UDPServer::GetPlayerById(uint16_t idPlayer) {
    auto it = std::find_if(players.begin(), players.end(),
                           [&idPlayer](Player p) {
                               return p.id == idPlayer;
                           });
    if (it != players.end())
        return it.base();
    return nullptr;
}

// se comprueba que el ultimo input mandado no supere el tiempo de desconexion
void UDPServer::DetectUsersDisconnected() {
    if (animationCountdownLaunched) {
        // Recorremos el array de jugadores
        for (Player& currentPlayer : players) {
            // si su última petición llegó hace más tiempo del tiempo de desconexión...
            if (Utils::getMillisSinceEpoch() - currentPlayer.lastInputTimeReceived > TIEMPO_DESCONEXION) {
                //cout << "Se ha desconectado el jugador: " << currentPlayer.id << "\n";

                // hacemos esto para no volver a repetir esto cada iteración. Por tanto
                // si un jugador está desconectado, esta llamada solo se repetirá cada 10 segundos
                // y no todo el rato.
                currentPlayer.lastInputTimeReceived = Utils::getMillisSinceEpoch();
                if (currentPlayer.disconnected == false) {
                    //cout << "\tlo marcamos como desconectado" << endl;
                    currentPlayer.disconnected = true;
                } else {
                    //cout << "\tlo marcamos como readyToDelete" << endl;
                    currentPlayer.readyToDelete = true;
                }

                // creamos un buffer para avisar al resto de jugadores de que éste jugador se ha desconectado
                unsigned char sendBuff[Constants::ONLINE_BUFFER_SIZE];
                size_t currentBuffSize = 0;
                uint8_t callType = Constants::PetitionTypes::SEND_DISCONNECTION;

                Serialization::Serialize(sendBuff, &callType, currentBuffSize);
                Serialization::Serialize(sendBuff, &currentPlayer.id, currentBuffSize);

                // recorremos el resto de jugadores, y les avisamos
                for (Player& currentPlayerToReSend : players) {
                    if (currentPlayerToReSend.id != currentPlayer.id && currentPlayerToReSend.disconnected == false)
                        SendBytes(sendBuff, currentBuffSize, currentPlayerToReSend);
                }

                // To-Do: eliminarlo del array de players
                // To-Do: respuesta en el cliente
            }
        }

        size_t beforeDelete = players.size();

        // borramos los players que ya estén ready to delete
        players.erase(
            std::remove_if(
                players.begin(),
                players.end(),
                [](Player& p) { return p.readyToDelete; }),
            players.end());

        size_t afterDelete = players.size();
        // cout << "Ahora tenemos " << afterDelete << " jugadores" << endl;
        // si antes había 1 y ahora hay 0, salimos
        // si antes había 2 y ahora hay 1, salimos
        if ((beforeDelete == 1 && afterDelete == 0) || (beforeDelete > 1 && afterDelete <= 1)) {
            // avisamos a quien quede que la partida se ha acabado...
            for (const auto& player : players) {
                // pondremos como idWinner el único jugador que queda, que será el único que haya en este for
                // el idPlayer se supone que representa a la persona que enviaba esta petición, pero como ha sido por una desconexión,
                // simplemente enviaremos el mismo idPlayer que el winner
                uint16_t idWinner = player.id;
                SendLaunchAnimationEnd(player, idWinner, idWinner);
            }

            // y salimos
            //cout << "Se han caído todos los jugadores y solo queda uno o ninguno. Reiniciamos el server" << endl;
            Exit();
        }
    }
    CheckDisconnectionsAfterSeconds(); // este es el que tenemos que descomentar para que vuelva a funcionar la desconexión
}

void UDPServer::SendLaunchAnimationEnd(const Player& p, const uint16_t idPlayer, const uint16_t idWinner) {
    unsigned char requestBuff[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t callType = Constants::PetitionTypes::LAUNCH_ANIMATION_END;
    int64_t time = Utils::getMillisSinceEpoch();

    Serialization::Serialize(requestBuff, &callType, currentBuffSize);
    Serialization::Serialize(requestBuff, &time, currentBuffSize);
    Serialization::Serialize(requestBuff, &idPlayer, currentBuffSize);
    Serialization::Serialize(requestBuff, &idWinner, currentBuffSize);

    SendBytes(requestBuff, currentBuffSize, p);
}

void UDPServer::CheckDisconnectionsAfterSeconds() {
    timer->expires_at(timer->expiry() + boost::asio::chrono::seconds(TIME_BETWEEN_DISCONNECTION_CHECKS));
    timer->async_wait(boost::bind(
        &UDPServer::DetectUsersDisconnected,
        this));
}

void UDPServer::Exit() {
    players.clear();
    players.shrink_to_fit();
    context.stop();
}

void UDPServer::SendEndgame(const Player& p) {
    unsigned char requestBuff[Constants::ONLINE_BUFFER_SIZE];
    size_t currentBuffSize = 0;
    uint8_t callType = Constants::PetitionTypes::ENDGAME;
    int64_t time = Utils::getMillisSinceEpoch();

    Serialization::Serialize(requestBuff, &callType, currentBuffSize);
    Serialization::Serialize(requestBuff, &time, currentBuffSize);

    SendBytes(requestBuff, currentBuffSize, p);
}