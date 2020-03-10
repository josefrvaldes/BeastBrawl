#include "SystemOnline.h"

#include "../../include/glm/vec3.hpp"
#include "../Constants.h"
#include "../Entities/CarHuman.h"
#include "../Entities/PowerUp.h"
#include "../Entities/Totem.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManTotem.h"
#include "../Online/UDPClient.h"

#include "../Components/CIDOnline.h"
#include "../Components/COnline.h"
#include "../Components/CPowerUp.h"
#include "../Components/CTargetEntity.h"
#include "../Components/CTotem.h"
#include "../Components/CTransformable.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Utils.h"

using namespace boost::asio;

SystemOnline::SystemOnline(ManCar &manCar_, uint16_t idOnlineMainCar_) : idOnlineMainCar{idOnlineMainCar_}, manCar{manCar_}, udpClient{make_unique<UDPClient>(Constants::SERVER_HOST, SERVER_PORT_UDP)} {
    shared_ptr<CarHuman> car = manCar.GetCar();
    SubscribeToEvents();
}

void SystemOnline::SubscribeToEvents() {
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_ENDRACE,
        bind(&SystemOnline::EventEndgame, this, std::placeholders::_1),
        "Endgame"));
}

void SystemOnline::EventEndgame(DataMap *dataMap) {
    udpClient->SendEndgame();
}

void SystemOnline::SendInputs(const vector<Constants::InputTypes> &inputs) const {
    // cout << Utils::getISOCurrentTimestampMillis() << "id[" << idOnlineMainCar << "] Enviamos los inputs" << endl;
    udpClient->SendInputs(inputs, idOnlineMainCar);
}

void SystemOnline::SendEndgame() const {
    udpClient->SendEndgame();
}

void SystemOnline::SendSync(ManCar *manCars, ManTotem *manTotem) const {
    // cout << Utils::getISOCurrentTimestampMillis() << "id[" << idOnlineMainCar << "] Enviamos sync" << endl;
    auto cTransCar = static_cast<CTransformable *>(manCars->GetCar()->GetComponent(CompType::TransformableComp).get());
    auto cTotem = static_cast<CTotem *>(manCars->GetCar()->GetComponent(CompType::TotemComp).get());
    auto cPowerUp = static_cast<CPowerUp *>(manCars->GetCar()->GetComponent(CompType::PowerUpComp).get());
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
    udpClient->SendSync(idOnlineMainCar, cTransCar->position, cTransCar->rotation, cPowerUp->typePowerUp, cTotem->active, cTotem->accumulatedTime, totemInGround, posTotem);
}

void SystemOnline::SendCatchPU(CPowerUp &cPowerUp) const {
    udpClient->SendCatchPU(idOnlineMainCar, cPowerUp.typePowerUp);
}

// lo enviamos tres veces para evitar que se pierdan todos los paquetes
void SystemOnline::SendCatchTotem(uint16_t idCarCatched) const {
    for (uint8_t i = 0; i < TIMES_RESEND; ++i)
        udpClient->SendCatchTotem(idOnlineMainCar, idCarCatched);
}

void SystemOnline::SendLostTotem(uint16_t idCarCatched, const glm::vec3 &position, int numNavMesh) const {
    for (uint8_t i = 0; i < TIMES_RESEND; ++i)
        udpClient->SendLostTotem(idOnlineMainCar, idCarCatched, position, numNavMesh);
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
    cout << "Hemos creado un cidOnline que es " << cidOnline->idOnline << endl;
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
