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
 */
 
 
 #include "SystemOnline.h"

#include "../../include/glm/vec3.hpp"
#include "../Components/CCar.h"
#include "../Components/CIDOnline.h"
#include "../Components/COnline.h"
#include "../Components/CPowerUp.h"
#include "../Components/CTargetEntity.h"
#include "../Components/CTotem.h"
#include "../Components/CTransformable.h"
#include "../Constants.h"
#include "../Entities/CarHuman.h"
#include "../Entities/PowerUp.h"
#include "../Entities/Totem.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManTotem.h"
#include "../Online/UDPClient.h"
#include "../Systems/Utils.h"

using namespace boost::asio;

SystemOnline::SystemOnline(ManCar &manCar_, uint16_t idOnlineMainCar_) : idOnlineMainCar{idOnlineMainCar_}, manCar{manCar_}, udpClient{make_unique<UDPClient>(Constants::SERVER_HOST, SERVER_PORT_UDP)}, timeStartClock{-1} {
    shared_ptr<CarHuman> car = manCar.GetCar();  // esto sirve para algo? se podrá borrar, no?
    SubscribeToEvents();
}

SystemOnline::~SystemOnline() {
    //cout << "Llamando al destructor de SystemOnline" << endl;
}

void SystemOnline::SubscribeToEvents() {
    // ya no vamos a enviar la petición de fin de juego, el juego se acabará automáticamente al acabar la animationEnd
    // la animationEnd sí se envía específicamente como veis en la suscripción de debajo
    // EventManager::GetInstance().SubscribeMulti(Listener(
    //     EventType::STATE_ENDRACE,
    //     bind(&SystemOnline::EventEndgame, this, std::placeholders::_1),
    //     "Endgame in SystemOnline"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::LAUNCH_ANIMATION_END_MULTI,
        bind(&SystemOnline::EventLaunchAnimationEnd, this, std::placeholders::_1),
        "Launch end animation"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_CLOCK_SYNC_RECEIVED,
        bind(&SystemOnline::NewClockSyncReceived, this, std::placeholders::_1),
        "New clock sync received"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_FINAL_CLOCK_SYNC_RECEIVED,
        bind(&SystemOnline::NewFinalClockSyncReceived, this, std::placeholders::_1),
        "New clock sync received"));
}

void SystemOnline::EventEndgame(DataMap *dataMap) {
    udpClient->SendEndgame(idOnlineMainCar);
}

void SystemOnline::EventLaunchAnimationEnd(DataMap *dataMap) {
    auto car = manCar.GetCurrentWinner();
    auto cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
    uint16_t idOnlineWinner = cOnline->idClient;
    udpClient->SendLaunchAnimationEnd(idOnlineMainCar, idOnlineWinner);
}

void SystemOnline::SendInputs(const vector<Constants::InputTypes> &inputs, float speed, float wheelRotation, float skidDeg, float skidRotation) const {
    // cout << Utils::getISOCurrentTimestampMillis() << "id[" << idOnlineMainCar << "] Enviamos los inputs" << endl;
    udpClient->SendInputs(GetGameTime(), inputs, idOnlineMainCar, speed, wheelRotation, skidDeg, skidRotation);
}

/*void SystemOnline::SendEndgame() const {
    udpClient->SendEndgame(idOnlineMainCar);
}*/

void SystemOnline::SendSync(ManCar *manCars, ManTotem *manTotem) const {
    // cout << Utils::getISOCurrentTimestampMillis() << "id[" << idOnlineMainCar << "] Enviamos sync" << endl;
    auto cTransCar = static_cast<CTransformable *>(manCars->GetCar()->GetComponent(CompType::TransformableComp).get());
    auto cTotem = static_cast<CTotem *>(manCars->GetCar()->GetComponent(CompType::TotemComp).get());
    auto cPowerUp = static_cast<CPowerUp *>(manCars->GetCar()->GetComponent(CompType::PowerUpComp).get());
    auto cCar = static_cast<CCar *>(manCars->GetCar()->GetComponent(CompType::CarComp).get());
    bool totemInGround = true;
    glm::vec3 posTotem(0.0, 0.0, 0.0);

    for (const auto &car : manCar.GetEntities()) {
        auto cTotemCar = static_cast<CTotem *>(car->GetComponent(CompType::TotemComp).get());
        if (cTotemCar->active == true) {
            totemInGround = false;
            break;
        }
    }

    if (manTotem->GetEntities().size() != 0) {
        auto cTransTotem = static_cast<CTransformable *>(manTotem->GetEntities()[0]->GetComponent(CompType::TransformableComp).get());
        posTotem = cTransTotem->position;
    }

    //std::cout << "ENVIADO -------------------------------" << std::endl;
    //std::cout << "Id: " << idOnlineMainCar << std::endl;
    //std::cout << "Pos coche: " << cTransCar->position.x << " , " << cTransCar->position.z << std::endl;
    //std::cout << "Tengo totem: " << cTotem->active << std::endl;
    //std::cout << "Tiempo Totem: " << cTotem->accumulatedTime << std::endl;
    //std::cout << "Totem en suelo: " << totemInGround << std::endl;
    //std::cout << "Pos totem: " << posTotem.x << " , " << posTotem.z << std::endl;
    //std::cout << "---------------------------------------" << std::endl;
    udpClient->SendSync(GetGameTime(), idOnlineMainCar, cTransCar->position, cTransCar->rotation, cCar->speed, cCar->wheelRotation, cCar->skidDeg, cCar->skidRotation, cPowerUp->typePowerUp, cTotem->active, cTotem->accumulatedTime, totemInGround, posTotem);
}

void SystemOnline::SendCatchPU(CPowerUp &cPowerUp) const {
    udpClient->SendCatchPU(idOnlineMainCar, cPowerUp.typePowerUp);
}

// lo enviamos tres veces para evitar que se pierdan todos los paquetes
void SystemOnline::SendCatchTotem(uint16_t idCarCatched) const {
    for (uint8_t i = 0; i < TIMES_RESEND; ++i)
        udpClient->SendCatchTotem(idOnlineMainCar, idCarCatched);
}

void SystemOnline::SendLostTotem(uint16_t idCarCatched, const glm::vec3 &position, float speed, int rotationTotemY, int numNavMesh) const {
    for (uint8_t i = 0; i < TIMES_RESEND; ++i)
        udpClient->SendLostTotem(idOnlineMainCar, idCarCatched, position, speed, rotationTotemY, numNavMesh);
}

void SystemOnline::SendCrashPUCar(const uint16_t idPowerUp, const uint16_t idCar) const {
    for (uint8_t i = 0; i < TIMES_RESEND; ++i)
        udpClient->SendCrashPUCar(idOnlineMainCar, idPowerUp, idCar);
}

void SystemOnline::SendCrashPUWall(const uint16_t idPowerUp) const {
    for (uint8_t i = 0; i < TIMES_RESEND; ++i)
        udpClient->SendCrashPUWall(idOnlineMainCar, idPowerUp);
}

void SystemOnline::SendThrowPU(const shared_ptr<PowerUp> &powerUp, const uint16_t idToPursue) const {
    // se le asigna al powerup un id que será el que se guardará el server y
    // servirá luego para eliminar este PU concreto cuando choque
    shared_ptr<CIDOnline> cidOnline = make_shared<CIDOnline>(idOnlineMainCar);
    powerUp->AddComponent(cidOnline);
    //cout << "Hemos creado un cidOnline que es " << cidOnline->idOnline << endl;
    auto cTransformable = static_cast<CTransformable *>(powerUp->GetComponent(CompType::TransformableComp).get());
    auto cPowerUp = static_cast<CPowerUp *>(powerUp->GetComponent(CompType::PowerUpComp).get());
    int64_t time = Utils::getMillisSinceEpoch();
    if (cPowerUp->typePowerUp == typeCPowerUp::TeleBanana) {
        for (uint8_t i = 0; i < TIMES_RESEND; ++i)
            udpClient->SendThrowTelebanana(idOnlineMainCar, time, cidOnline->idOnline, cTransformable->position, cTransformable->rotation, static_cast<int8_t>(cPowerUp->typePowerUp), idToPursue);
    } else {
        for (uint8_t i = 0; i < TIMES_RESEND; ++i)
            udpClient->SendThrowMelonOPudin(idOnlineMainCar, time, cidOnline->idOnline, cTransformable->position, cTransformable->rotation, static_cast<int8_t>(cPowerUp->typePowerUp));
    }
}

void SystemOnline::SendRoboJorobo() const {
    for (uint8_t i = 0; i < TIMES_RESEND; ++i)
        udpClient->SendRoboJorobo(idOnlineMainCar);
}

// se le pasa primero el coche que lleva totem, y luego el coche que choca con el
void SystemOnline::SendNitro(uint16_t idCarWithTotem, uint16_t idCarWithNitro) const {
    for (uint8_t i = 0; i < TIMES_RESEND; ++i)
        udpClient->SendCollideNitro(idOnlineMainCar, idCarWithTotem, idCarWithNitro);
}

void SystemOnline::SendWaitingForCountdown() const {
    udpClient->SendWaitingForCountdown(idOnlineMainCar);
}

void SystemOnline::SyncClocks() {
    clocksStartedSyncing = true;
    auto car = manCar.GetCar();
    COnline *cOnlineMainCar = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
    if (cOnlineMainCar->idClient == 1) {
        // ACTUALIZACIÓN: no ordenamos porque judith dice que utiliza el orden para no se qué cosa y como para cambiárselo a la chiquilla
        // ordenamos las entities dentro del vector por idOnline
        // auto entities = manCar.GetEntities();
        // std::sort(entities.begin(), entities.end(), []( std::shared_ptr<Entity> &a, std::shared_ptr<Entity> &b) -> bool {
        //     COnline* cOnlineA = static_cast<COnline *>(a->GetComponent(CompType::OnlineComp).get());
        //     COnline* cOnlineB = static_cast<COnline *>(b->GetComponent(CompType::OnlineComp).get());
        //     return cOnlineA->idClient > cOnlineB->idClient;
        // });


        
        for (auto car : manCar.GetEntities()) {
            COnline* cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
            // si no somos el coche principal, ya que es el coche principal precisamente el que inicia estas sincronizaciones..
            if(cOnline->idClient != 1) {
                timeStartClock = Utils::getMillisSinceEpoch();
                cOnlineMainCar->timeSyncClock[cOnline->idClient] = timeStartClock;
                // enviamos a nuestros compis la petición de sincronizar el reloj
                for (uint8_t i = 0; i < TIMES_RESEND; ++i) // ojo con el reenvío aquí, la podemos liar madafaka
                    udpClient->SendClockSync(cOnlineMainCar->idClient, cOnline->idClient, GetGameTime(), 0, 0);
            }
        }
    }
}

int64_t SystemOnline::GetGameTime() const {
    return Utils::getMillisSinceEpoch() - timeStartClock;
}


void SystemOnline::NewClockSyncReceived(DataMap *d) {
    uint16_t idSender = any_cast<uint16_t>((*d)[DataType::ID]);
    //uint16_t idReceiver = any_cast<uint16_t>((*d)[DataType::ID_DESTINATION]);
    float turnoutReceived = any_cast<float>((*d)[DataType::TURNOUT]);
    // int64_t time = any_cast<int64_t>((*d)[DataType::TIME]);
    int8_t numMeasurements = any_cast<int8_t>((*d)[DataType::NUM]);
    
    auto car = manCar.GetCar();
    COnline* cOnlineMainCar = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
    
    // si llevamos cero y NO somos el coche lider (id=1), entonces debemos iniciar nuestro propio sistema de sync, por tanto, iniciamos nuestro reloj por primera vez, el turnout, y el numMeasurements
    // la comprobación de timeStartClock < 0 la hacemos para que si recibimos un reenvío (cada petición se envía 3 veces), no entrar el resto de veces
    if(cOnlineMainCar->numMeasurements[idSender] == 0 && cOnlineMainCar->idClient != 1 && timeStartClock < 0) {
        //cout << "NO soy el líder, voy a iniciar por primera vez mi sync" << endl;

        // iniciamos el reloj
        timeStartClock = Utils::getMillisSinceEpoch();
        cOnlineMainCar->timeSyncClock[idSender] = timeStartClock;

        // inicializamos el turnout a 0, todavía no hemos recibido ningún rebote
        cOnlineMainCar->currentTurnout[idSender] = 0.f;

        // y decimos que ya tenemos una medida hecha
        cOnlineMainCar->numMeasurements[idSender] = 0;

        //cout << "T["<<cOnlineMainCar->timeSyncClock[idSender]<<"] TO["<<cOnlineMainCar->currentTurnout[idSender]<<"] N["<<unsigned(cOnlineMainCar->numMeasurements[idSender])<<"]" << endl<< endl<< endl;
        for(uint8_t i = 0; i < TIMES_RESEND; i++) {
            udpClient->SendClockSync(cOnlineMainCar->idClient, idSender, GetGameTime(), 0.f, cOnlineMainCar->numMeasurements[idSender]);
        }


        // si somos el lider y recibimos un numMeasurements = 0, significa que este es nuestro primer rebote, por tanto, calculamos el primer turnout y tal
    } else if(cOnlineMainCar->numMeasurements[idSender] == 0 && cOnlineMainCar->idClient == 1) {
        //cout << "Soy el líder, he recibido el primer rebote de ["<<idSender<<"]. Voy a calcular el primer turnout" << endl;

        float calculatedTurnout = GetGameTime();
        cOnlineMainCar->currentTurnout[idSender] = calculatedTurnout;
        cOnlineMainCar->numMeasurements[idSender] += 1;
        cOnlineMainCar->timeSyncClock[idSender] = Utils::getMillisSinceEpoch();

        //cout << "T["<<cOnlineMainCar->timeSyncClock[idSender]<<"] TO["<<cOnlineMainCar->currentTurnout[idSender]<<"] N["<<unsigned(cOnlineMainCar->numMeasurements[idSender])<<"]" << endl<< endl<< endl;
        for(uint8_t i = 0; i < TIMES_RESEND; i++) {
            udpClient->SendClockSync(cOnlineMainCar->idClient, idSender, GetGameTime(), calculatedTurnout, cOnlineMainCar->numMeasurements[idSender]);
        }


        // si no era la primera medida, podemos ser o el lider o los otros, el comportamiento será el mismo
        // OJO, como dato interesante: los numMeasurements que recibe el lider siempre son impares, y los demás, pares
    } else {

        // con esta comprobación nos evitamos recalcular si recibimos algún reenvío (cada petición se envía 3 veces, podríamso recibir dos veces seguidas un numMeasurements = 3 por ejemplo, pero solo analizamos una)
        if(cOnlineMainCar->numMeasurements[idSender] < numMeasurements) {
            //cout << "Soy ["<<idReceiver<<"], he recibido un rebote de ["<<idSender<<"] con los datos TO["<<turnoutReceived<<"] N["<<unsigned(numMeasurements)<<"] voy a hacer calculitos" << endl;

            int64_t interval = Utils::getMillisSinceEpoch() - cOnlineMainCar->timeSyncClock[idSender]; // esto es el tiempo que ha tardado el último rebote/turnout
            //cout << "Este rebote concreto ha tardado " << interval << "ms" << endl;
            float calculatedTurnout = (turnoutReceived + interval) / 2.0f;
            cOnlineMainCar->currentTurnout[idSender] = calculatedTurnout;
            cOnlineMainCar->numMeasurements[idSender] = numMeasurements + 1;
            cOnlineMainCar->timeSyncClock[idSender] = Utils::getMillisSinceEpoch();
            //cout << "T["<<cOnlineMainCar->timeSyncClock[idSender]<<"] TO["<<cOnlineMainCar->currentTurnout[idSender]<<"] N["<<unsigned(cOnlineMainCar->numMeasurements[idSender])<<"]" << endl;


            // significa que ya hemos terminado, por tanto NO volvemos a reenviar ya, pasamos a fase dos. Por ahora solamente imprimimos los valores por pantalla para depurar
            if(idOnlineMainCar == 1 && cOnlineMainCar->numMeasurements[idSender] >= MAX_NUM_MEASUREMENTS) {
                //cout << "Soy el líder supremo y he terminado con  los datos ";
                //cout << "T["<<cOnlineMainCar->timeSyncClock[idSender]<<"] TO["<<cOnlineMainCar->currentTurnout[idSender]<<"] N["<<unsigned(cOnlineMainCar->numMeasurements[idSender])<<"]" << endl<< endl<< endl;
                bool syncFinished = CheckIfSyncFinished(cOnlineMainCar, cOnlineMainCar->numMeasurements[idSender]);
                if(syncFinished) {
                    //cout << "Hemos terminado el cálculo de lag con todos los players, ahora vamos a mandarle ya definitivamente la petición de sincronización de reloj" << endl;
                    SendFinalClockSync(cOnlineMainCar);
                } else {
                    //cout << "Hemos terminado el cálculo de lag con un player pero no con todos" << endl;
                }


                // si todavía no hemos acabado de medir, reenvío de nuevo
            } else if(cOnlineMainCar->numMeasurements[idSender] <= MAX_NUM_MEASUREMENTS) {
                //cout << "Voy a reenviar los datos a ["<<idSender<<"]" << endl<< endl<< endl;
                // reenvío el paquete de nuevo a la persona que me lo envió
                for(uint8_t i = 0; i < TIMES_RESEND; i++) {
                    udpClient->SendClockSync(cOnlineMainCar->idClient, idSender, GetGameTime(), calculatedTurnout, cOnlineMainCar->numMeasurements[idSender]);
                }
            }
        }
    }
    
}


/**
 * Recibimos el numMeasurementsToCompare en vez de usar la constante MAX_NUM_MEASUREMENTS porque como el numMeasurement es intercalado (1 yo, 2 el otro, 3 yo, 4 el otro...)
 * No podemos usar esa variable exacta porque igual nos pasamos exactos, porque justo coincide que yo hago el incremento que finalmente llega a ese valor
 * o igual me paso por 1 porque es el otro el que supera el valor
 */
bool SystemOnline::CheckIfSyncFinished(COnline *cOnlineMainCar, const uint8_t numMeasurementsToCompare) const {
    //cout << "Vamos a comprobar si el sync está finalizado" << endl;
    for  (auto currentCar: manCar.GetEntities()) {
        COnline* cOnline = static_cast<COnline*>(currentCar->GetComponent(CompType::OnlineComp).get());
        if(cOnline->idClient != 1) {
            auto currentMeasurements = cOnlineMainCar->numMeasurements[cOnline->idClient];
            //cout << "El cliente id["<<cOnline->idClient<<"] tiene medidas["<<currentMeasurements<<"]" << endl;
            if(currentMeasurements < numMeasurementsToCompare) {
                //cout << "es false que hemos terminado la sync" << endl;
                return false;
            }
        }
    }
    //cout << "es true que hemos terminado la sync" << endl;
    return true;
};


void SystemOnline::SyncNOW() {
    timeStartClock = Utils::getMillisSinceEpoch();
    //cout << Utils::getISOCurrentTimestampMillis() << " Hemos acabado de sincronizar!!" << endl;
}


void SystemOnline::SendFinalClockSync(COnline *cOnlineMainCar) {
    int64_t msSync = TIME_TO_WAIT_FOR_SYNC;
    for  (auto currentCar: manCar.GetEntities()) {
        COnline* cOnline = static_cast<COnline*>(currentCar->GetComponent(CompType::OnlineComp).get());
        if(cOnline->idClient != 1) {
            udpClient->SendFinalClockSync(cOnlineMainCar->idClient, cOnline->idClient, cOnlineMainCar->currentTurnout[cOnline->idClient], msSync);
        }
    }
    
    //cout << Utils::getISOCurrentTimestampMillis() << " Vamos a lanzar el timer" << endl;
    timer = make_unique<steady_timer>(context, boost::asio::chrono::milliseconds(msSync));
    timer->async_wait(std::bind(
        &SystemOnline::SyncNOW,
        this));
    context.run();
    //cout << "Hemos acabado de sincronizar" << endl;
}



void SystemOnline::NewFinalClockSyncReceived(DataMap *d) {
    int64_t time = any_cast<int64_t>((*d)[DataType::TIME]);
    float turnout = any_cast<float>((*d)[DataType::TURNOUT]);
    int64_t msSync = time - round(turnout / 2.0);
    
    //cout << Utils::getISOCurrentTimestampMillis() << " Vamos a lanzar el timer" << endl;
    timer = make_unique<steady_timer>(context, boost::asio::chrono::milliseconds(msSync));
    timer->async_wait(std::bind(
        &SystemOnline::SyncNOW,
        this));
    context.run();
    //cout << "Hemos acabado de sincronizar" << endl;
}