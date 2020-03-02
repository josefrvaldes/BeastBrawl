#include "SystemOnline.h"

#include "../../include/glm/vec3.hpp"
#include "../Constants.h"
#include "../Entities/CarHuman.h"
#include "../Entities/Totem.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManTotem.h"
#include "../Online/UDPClient.h"

#include "../Components/COnline.h"
#include "../Components/CPowerUp.h"
#include "../Components/CTotem.h"
#include "../Components/CTransformable.h"
#include "../Systems/Utils.h"

using namespace boost::asio;

SystemOnline::SystemOnline(ManCar &manCar_, uint16_t idOnlineMainCar_) : idOnlineMainCar{idOnlineMainCar_}, manCar{manCar_}, udpClient{make_unique<UDPClient>(SERVER_HOST, SERVER_PORT_UDP)} {
    shared_ptr<CarHuman> car = manCar.GetCar();
}

void SystemOnline::SendInputs(const vector<Constants::InputTypes> &inputs) {
    // cout << Utils::getISOCurrentTimestampMillis() << "id[" << idOnlineMainCar << "] Enviamos los inputs" << endl;
    udpClient->SendInputs(inputs, idOnlineMainCar);
}

void SystemOnline::SendSync(ManCar *manCars, ManTotem *manTotem) {
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