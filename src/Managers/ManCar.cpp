#include "ManCar.h"
#include "ManPowerUp.h"
#include "ManBoxPowerUp.h"
#include "ManTotem.h"
#include "ManBoundingWall.h"
#include "ManNavMesh.h"

#include <functional>
#include <iostream>
#include <Entities/Camera.h>
#include <Entities/CarHuman.h>
#include <Entities/CarAI.h>
#include <Systems/Physics.h>
#include <Systems/PhysicsAI.h>
#include <Components/CShield.h>
#include <Components/CTotem.h>
#include <Components/CNitro.h>
#include <Components/CCar.h>
#include <Components/CDimensions.h>
#include <Game.h>

#include "../Components/COnline.h"

#include "../Facade/Render/RenderFacadeManager.h"
#include "../Game.h"
#include "Manager.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManBoundingWall.h"
#include "../Managers/ManNavMesh.h"
#include "../Systems/SystemBtMoveTo.h"
#include "../Systems/SystemBtLoDMove.h"
#include "../Systems/SystemBtPowerUp.h"


class Position;
using namespace std;

ManCar::ManCar() {
    SubscribeToEvents();
    CreateMainCar();
    
    // systemPathPlanning = make_unique<SystemPathPlanning>();
    physicsAI = make_unique<PhysicsAI>();

    cout << "Hemos creado un powerup, ahora tenemos " << entities.size() << " powerups" << endl;
}

ManCar::~ManCar() {
    cout << "Llamando al destructor de ManCar" << endl;
    entities.clear();
    entities.shrink_to_fit();
}

// TO-DO: este paso de physics es kk, hay que revisarlo de enviarlo como referencia o algo pero me da error
ManCar::ManCar(Physics* _physics, Camera* _cam) : ManCar() {
    this->physics = _physics;
    this->cam = _cam;
}

// comprueba si has superado el tiempo necesario para ganar
void ManCar::UpdateCar() {
    auto cTotem = static_cast<CTotem*>(GetCar()->GetComponent(CompType::TotemComp).get());
    if (cTotem->active) {
        cTotem->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
    }

    if (cTotem->accumulatedTime / 1000.0 > cTotem->durationTime / 1000.0) {
        cout << "Has ganado\n";
        //Game::GetInstance()->SetState(State::ENDRACE);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});
    }

    // Actualiza el componente nitro
    auto cNitro = static_cast<CNitro*>(GetCar()->GetComponent(CompType::NitroComp).get());
    if (cNitro->activePowerUp == true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime) {  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }

    // Actualiza el componente escudo
    auto cShield = static_cast<CShield*>(GetCar()->GetComponent(CompType::ShieldComp).get());
    if (cShield->activePowerUp == true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime) {  // comprueba el tiempo desde que se lanzo
        cShield->deactivePowerUp();
    }
}

// TODO: RECORDARRR!!!!!!!!!!!!!!!!!  TANTO EL "BtMoveTo" como el "systemPathPlanning" se deben hacer en la misma ITERACION!!!!
// Es importante esto porque el BtMoveTo es el que calcula la posicion a la que ir y el systemBtLoDMove es el que utiliza esta posicion para
// moverse a un sitio, si en algun momento intentamos ir a una posicion que no existe PETAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
void ManCar::UpdateCarAI(CarAI* carAI, ManPowerUp* m_manPowerUp, ManBoxPowerUp* m_manBoxPowerUp, ManTotem* m_manTotem, ManWayPoint* graph, ManNavMesh* manNavMesh, 
                        ManBoundingWall* m_manBoundingWall, SystemBtPowerUp* systemBtPowerUp, SystemBtMoveTo* systemBtMoveTo, SystemBtLoDMove* systemBtLoDMove, SystemPathPlanning *systemPathPlanning) {
    
    
    systemBtMoveTo->update(carAI, this, m_manPowerUp, m_manBoxPowerUp, m_manTotem, graph, manNavMesh);
    
    systemPathPlanning->Update(carAI, graph, manNavMesh);

    systemBtLoDMove->update(carAI, this, m_manPowerUp, m_manBoxPowerUp, m_manTotem, graph, manNavMesh, m_manBoundingWall);

    physicsAI->Update(carAI, graph);

    systemBtPowerUp->update(carAI, this, m_manPowerUp, m_manBoxPowerUp, m_manTotem, graph, manNavMesh);
    

}

void ManCar::UpdateCarHuman(Entity* CarHuman) {
    physicsAI->UpdateCarPowerUps(CarHuman);
}

void ManCar::CreateMainCar() {
    car = make_shared<CarHuman>();
    entities.push_back(car);
}

void ManCar::CreateHumanCar(glm::vec3 _position) {
    shared_ptr<CarHuman> p = make_shared<CarHuman>(_position);
    entities.push_back(p);
}

void ManCar::CreateCarAI(glm::vec3 _position) {
    shared_ptr<CarAI> p = make_shared<CarAI>(_position);
    entities.push_back(p);
}

void ManCar::CreateCarAI(glm::vec3 _position, CWayPoint* _waypoint) {
    shared_ptr<CarAI> p = make_shared<CarAI>(_position);
    entities.push_back(p);
    p->SetWayPoint(_waypoint);
}

void ManCar::CreateCarAI() {
    shared_ptr<CarAI> p = make_shared<CarAI>();
    entities.push_back(p);
}

void ManCar::SubscribeToEvents() {
    /**
     * auto accelerateCar = [&](Data d) {
     *     cout << "Estamos ejecutando lambda " << endl;
     *     Car *c = this->GetCar().get();
     *     auto components = c->GetComponents();
     *     CTransformable *trans = static_cast<CTransformable*>(components[CompType::TransformableComp].get()); 
     *     cout << "El coche está en  " << trans->position.x << "," << trans->position.y << "," << trans->position.z << endl;
     *     //physics->Accelerate(car.get(), cam);
     * };
     */

    /*int (*accelerate) (Data) = [&](Data d) {
     *   physics->Accelerate(car.get(), cam);
     *   return 0;
     *};
     *int (*func_pointer) (int) = [](int a) { return a; };
     */

    /*accelerateCar(0);
     * auto lambdaAccelerate = [&](DataMap d){this->AccelerateCar(d);};
     *
     *auto lambdaGuardaAccel = [&lambdaAccelerate](DataMap d) {lambdaAccelerate(d);};
     */

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::PRESS_I,
        bind(&ManCar::AccelerateCar, this, placeholders::_1),
        "AccelerateCar"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::PRESS_O,
        bind(&ManCar::Decelerate, this, placeholders::_1),
        "Decelerate"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::PRESS_A,
        bind(&ManCar::TurnLeftCar, this, placeholders::_1),
        "TurnLeftCar"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::PRESS_D,
        bind(&ManCar::TurnRightCar, this, placeholders::_1),
        "TurnRightCar"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NO_I_O_PRESS,
        bind(&ManCar::NotAcceleratingOrDecelerating, this, placeholders::_1),
        "NotAcceleratingOrDecelerating"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NO_A_D_PRESS,
        bind(&ManCar::NotTurning, this, placeholders::_1),
        "NotTurning"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::CATCH_BOX_POWERUP,
        bind(&ManCar::CatchPowerUp, this, placeholders::_1),
        "CatchPowerUp"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::CATCH_AI_BOX_POWERUP,
        bind(&ManCar::CatchPowerUpAI, this, placeholders::_1),
        "CatchPowerUpAI"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::PRESS_SPACE,
        bind(&ManCar::ThrowPowerUpCar, this, placeholders::_1),
        "ThrowPowerUpCar"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::THROW_POWERUP_AI,
        bind(&ManCar::ThrowPowerUpAI, this, placeholders::_1),
        "ThrowPowerUpAI"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::THROW_POWERUP_HUMAN,
        bind(&ManCar::ThrowPowerUpHuman, this, placeholders::_1),
        "ThrowPowerUpHuman"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_ENTITY_POWERUP,
        bind(&ManCar::CollisionPowerUp, this, placeholders::_1),
        "CollisionPowerUp"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_ENTITY_AI_POWERUP,
        bind(&ManCar::CollisionPowerUpAI, this, placeholders::_1),
        "CollisionPowerUpAI"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_PLAYER_TOTEM,
        bind(&ManCar::CatchTotemPlayer, this, placeholders::_1),
        "CatchTotemPlayer"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_AI_TOTEM,
        bind(&ManCar::CatchTotemAI, this, placeholders::_1),
        "CatchTotemAI"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::CHANGE_TOTEM_CAR,
        bind(&ManCar::ChangeTotemCar, this, placeholders::_1),
        "ChangeTotemCar"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_INPUTS_RECEIVED,
        bind(&ManCar::NewInputsReceived, this, placeholders::_1),
        "NewInputsReceived"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_SYNC_RECEIVED_CAR,
        bind(&ManCar::NewSyncReceived, this, placeholders::_1),
        "NewSyncReceived"));
}

void ManCar::NewInputsReceived(DataMap* d) {
    // cout << "Se ha lanzado el evento NewInputsReceived" << endl;
    auto idRecieved = any_cast<uint16_t>((*d)[DataType::ID]);
    auto inputs = any_cast<vector<Constants::InputTypes>>((*d)[DataType::INPUTS]);
    for (shared_ptr<Entity> car : entities) {
        if (car->HasComponent(CompType::OnlineComp)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            uint16_t currentIDOnline = compOnline->idClient;
            // cout << "El idOnline es " << currentIDOnline << endl;
            if (currentIDOnline == idRecieved) {
                // cout << "Hemos encontrado un coche con el id " << id << " y vamos a actualizarle la pos" << endl;
                compOnline->inputs = inputs;
                break;
            }
        }
    }
}

void ManCar::NewSyncReceived(DataMap* d) {
    // cout << "Se ha lanzado el evento NewInputsReceived" << endl;
    auto idRecieved = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    for (auto car : GetEntities()) {
        if (car->HasComponent(CompType::OnlineComp)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            uint16_t currentIDOnline = compOnline->idClient;
            // cout << "El idOnline es " << currentIDOnline << endl;
            if (currentIDOnline == idRecieved) {
                auto cTran = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
                auto cPowerUp = static_cast<CPowerUp*>(car->GetComponent(CompType::PowerUpComp).get());
                auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
                cTran->position = any_cast<glm::vec3>((*d)[DataType::VEC3_POS]);
                cTran->rotation = any_cast<glm::vec3>((*d)[DataType::VEC3_ROT]);
                cPowerUp->typePowerUp = any_cast<typeCPowerUp>((*d)[DataType::TYPE_POWER_UP]);
                // cTotem->active = any_cast<bool>((*d)[DataType::CAR_WITH_TOTEM]);
                cTotem->accumulatedTime = any_cast<int64_t>((*d)[DataType::TIME_TOTEM]);

                break;
            }
        }
    }
}

void ManCar::ChangeTotemCar(DataMap* d) {
    auto carWithTotem = any_cast<Entity*>((*d)[CAR_WITH_TOTEM]);
    auto carWithoutTotem = any_cast<Entity*>((*d)[CAR_WITHOUT_TOTEM]);
    ThrowTotem(carWithTotem);
    // activamos el totem al coche que ahora lo tiene
    auto cTotemWithout = static_cast<CTotem*>(carWithoutTotem->GetComponent(CompType::TotemComp).get());
    cTotemWithout->active = true;
    cTotemWithout->timeStart = system_clock::now();
    // Sonido coger totem
    EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_TOTEM});
}


void ManCar::CatchTotemAI(DataMap* d){
    auto cTotem = static_cast<CTotem*>(any_cast<Entity*>((*d)[ACTUAL_CAR])->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();
    // Sonido coger totem
    EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_TOTEM});
}

void ManCar::CatchTotemPlayer(DataMap* d){
    auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();
    // Sonido coger totem
    EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_TOTEM});
}

void ManCar::UseTotem(Entity* carWinTotem) {
    auto cTotem = static_cast<CTotem*>(carWinTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();
    //std::cout << "Has utilizado el robo jorobo de p*** madre coleeega ahora es tuyo" << std::endl;
}

void ManCar::ThrowTotem(Entity* carLoseTotem) {
    auto cTotem = static_cast<CTotem*>(carLoseTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = false;
    cTotem->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
    //std::cout << "El tiempo acumulado del totem hasta ahora del jugador es de:  " << cTotem->accumulatedTime/1000.0 << std::endl;
}

bool ManCar::useRoboJorobo(Entity* newCarWithTotem) {
    // recorremos los coches
    for(const shared_ptr<Entity>& cars : entities){
        auto cTotem = static_cast<CTotem*>(cars->GetComponent(CompType::TotemComp).get());
        // Si algun coche tenia el totem .... lo pierde, comprobamos que no sea el mmismo coche con las ID
        if (cTotem->active == true && newCarWithTotem != cars.get()) {
            ThrowTotem(cars.get());
            //al perderlo se lo asignamos al que ha usado el robo jorobo
            UseTotem(newCarWithTotem);

            return true;  // para salirnos y no hacer mas calculos
        }
    }
    return false;
}

void ManCar::CollisionPowerUp(DataMap* d) {
    // debemos desactivar el powerUp y para el contador de tiempo del totem
    auto cShield = static_cast<CShield*>(car.get()->GetComponent(CompType::ShieldComp).get());
    if (cShield->activePowerUp == false) {  // comprobamos si tiene el escudo
        auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get());
        if (cTotem->active == true) {
            ThrowTotem(car.get());
        }
        // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
        auto cCar = static_cast<CCar*>(car.get()->GetComponent(CompType::CarComp).get());
        cCar->speed = 0.0f;
        // Sonido choque con powerup
        shared_ptr<DataMap> data = make_shared<DataMap>();
        (*data)[MAIN_CAR] = true;
        EventManager::GetInstance().AddEventMulti(Event{EventType::HURT, data});
    } else {
        std::cout << "El escudo me salvo el culito :D" << std::endl;
        cShield->deactivePowerUp();  // desactivamos el escudo

        // Sonido coger totem
        EventManager::GetInstance().AddEventMulti(Event{EventType::NO_SHIELD});
    }
}

void ManCar::CollisionPowerUpAI(DataMap* d) {
    // debemos desactivar el powerUp y para el contador de tiempo del totem
    auto cShield = static_cast<CShield*>(any_cast<Entity*>((*d)[CAR_AI])->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp == false){            // comprobamos si tiene el escudo
        auto cTotem = static_cast<CTotem*>(any_cast<Entity*>((*d)[CAR_AI])->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            ThrowTotem(any_cast<Entity*>((*d)[CAR_AI]));
        }
        // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
        auto cCar = static_cast<CCar*>(any_cast<Entity*>((*d)[CAR_AI])->GetComponent(CompType::CarComp).get());
        cCar->speed = 0.0f;  // To-Do: no funciona en la IA por que la logica difusa no la hace acelerar
        // Sonido choque con powerup
        shared_ptr<DataMap> data = make_shared<DataMap>();
        (*data)[MAIN_CAR] = false;
        EventManager::GetInstance().AddEventMulti(Event{EventType::HURT, data});
    } else {
        std::cout << "El escudo me salvo el culito :D" << std::endl;
        cShield->deactivePowerUp();  // desactivamos el escudo

        // Sonido coger totem
        EventManager::GetInstance().AddEventMulti(Event{EventType::NO_SHIELD});
    }
}

// se calcula el coche mas cercano en el rango de vision, en caso de no haber se devuelve un nullptr
CTransformable* ManCar::calculateCloserCar(Entity* actualCar) {
    // Primero metemos al jugador por lo que no hace falta calcularlo luego
    CTransformable* closestCar = nullptr;
    auto cTransActualCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float distanceMimum = 9999999999.0;
    float distanceNext;
    float vectorXNext;
    float vectorZNext;

    // Para todos los coches
    for(const shared_ptr<Entity>& cars : entities){
        if(actualCar != cars.get() && carInVisionRange(actualCar, cars.get(), 60) == true){
            auto cTransNextCar = static_cast<CTransformable*>(cars.get()->GetComponent(CompType::TransformableComp).get()); 
            vectorXNext = cTransNextCar->position.x - cTransActualCar->position.x;     
            vectorZNext = cTransNextCar->position.z - cTransActualCar->position.z;
            distanceNext = sqrt((vectorXNext * vectorXNext) + (vectorZNext * vectorZNext));

            if (distanceMimum > distanceNext) {
                distanceMimum = distanceNext;
                closestCar = cTransNextCar;
            }
        }
    }
    return closestCar;
}

void ManCar::ThrowPowerUpCar(DataMap* d) {
    ThrowPowerUp(car.get());
}

void ManCar::ThrowPowerUpAI(DataMap* d) {
    ThrowPowerUp(any_cast<CarAI*>((*d)[ACTUAL_CAR]));
}

void ManCar::ThrowPowerUpHuman(DataMap* d) {
    ThrowPowerUp(any_cast<Car*>((*d)[ACTUAL_CAR]));
}

void ManCar::ThrowPowerUp(Car* car) {
    auto cPowerUpCar = static_cast<CPowerUp*>(car->GetComponent(CompType::PowerUpComp).get());
    //auto cRoboJorobo = static_cast<CRoboJorobo*>(any_cast<CarAI*>(d[ACTUAL_CAR])->GetComponent(CompType::RoboJoroboComp).get());
    auto cShield = static_cast<CShield*>(car->GetComponent(CompType::ShieldComp).get());
    auto cNitro = static_cast<CNitro*>(car->GetComponent(CompType::NitroComp).get());
    bool robado = false; 
    
    if(cPowerUpCar->typePowerUp != typeCPowerUp::None){
        shared_ptr<DataMap> data = make_shared<DataMap>();

        CTransformable* objectiveCar;
        switch (cPowerUpCar->typePowerUp) {
            case (typeCPowerUp::RoboJorobo):
                robado = useRoboJorobo(car);
                if (!robado)
                    std::cout << "La has cagado, el Totem no lo tenia nadie..." << std::endl;
                break;
            case (typeCPowerUp::EscudoMerluzo):
                cShield->activatePowerUp();
                break;
            case (typeCPowerUp::SuperMegaNitro):
                cNitro->activatePowerUp();
                break;
            case (typeCPowerUp::TeleBanana):
                objectiveCar = calculateCloserCar(car);
                if(objectiveCar != nullptr)
                    (*data)[CAR_FOLLOW_POSITION] = objectiveCar;
                (*data)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
                (*data)[CAR_EXIT_POSITION] = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
                (*data)[CAR_EXIT_DIMENSION] =  static_cast<CDimensions*>(car->GetComponent(CompType::DimensionsComp).get());
                EventManager::GetInstance().AddEventMulti(Event{EventType::PowerUp_Create, data});
                break;
            default:     // en caso del melon molon o el pudding
                (*data)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
                (*data)[CAR_EXIT_POSITION] = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
                (*data)[CAR_EXIT_DIMENSION] =  static_cast<CDimensions*>(car->GetComponent(CompType::DimensionsComp).get());
                EventManager::GetInstance().AddEventMulti(Event{EventType::PowerUp_Create, data});

                break;
        }
        cPowerUpCar->typePowerUp = typeCPowerUp::None;

        if(car == GetCar().get()){
            // Sonido de lanzar power-up
            shared_ptr<DataMap> dataHUD = make_shared<DataMap>();
            (*dataHUD)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
            //d->insert(TYPE_POWER_UP,cPowerUpCar->typePowerUp);
            EventManager::GetInstance().AddEventMulti(Event{EventType::THROW_POWERUP, dataHUD});

            // Ya no tenemos power-up
            shared_ptr<DataMap> dataSound = make_shared<DataMap>();

            cPowerUpCar->typePowerUp = typeCPowerUp::None;
            (*dataSound)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
            EventManager::GetInstance().AddEventMulti(Event{UPDATE_POWERUP_HUD, dataSound}); 
        }
    }
}

/*
int calculateProbabilityPowerUp(int totalPowerUps, std::vector<int> probabilityPU){
    int sum_of_weight = 0;

    int rnd = rand() % 100+1; // 100%
    for(int i=0; i<totalPowerUps; i++) {
      if(rnd < probabilityPU[i])
        return i;
      rnd -= probabilityPU[i];
    }
}
*/

void ManCar::CatchPowerUp(DataMap* d) {
    // To-Do: porcentajes temporales
    srand(time(NULL));
    int indx = rand() % 100 + 1;
    if (indx <= 5)  // 5%
        indx = 1;
    else if (indx > 5 && indx <= 20)  // 15%
        indx = 2;
    else if (indx > 20 && indx <= 40)  // 20%
        indx = 3;
    else if (indx > 40 && indx <= 55)  // 15%
        indx = 4;
    else if (indx > 55 && indx <= 70)  // 15%
        indx = 5;
    else if (indx > 70)  //  30%
        indx = 6;

    //indx = 3;
    //None,               // 0
    //RoboJorobo,         // 1
    //SuperMegaNitro,     // 2
    //PudinDeFrambuesa,   // 3
    //EscudoMerluzo,      // 4
    //TeleBanana,         // 5
    //MelonMolon          // 6
    //indx = 5;
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    if (cPowerUpCar->typePowerUp == typeCPowerUp::None) {
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        shared_ptr<DataMap> data = make_shared<DataMap>();

        (*data)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;

        EventManager::GetInstance().AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, data});
    }
}

void ManCar::CatchPowerUpAI(DataMap* d) {
    // To-Do: porcentajes temporales
    srand(time(NULL));
    int indx = rand() % 100 + 1;
    if (indx <= 5)  // 5%
        indx = 1;
    else if (indx > 5 && indx <= 20)  // 15%
        indx = 2;
    else if (indx > 20 && indx <= 40)  // 20%
        indx = 3;
    else if (indx > 40 && indx <= 55)  // 15%
        indx = 4;
    else if (indx > 55 && indx <= 70)  // 15%
        indx = 5;
    else if (indx > 70)  //  30%
        indx = 6;
    //indx = 5;
    auto cPowerUpCar = static_cast<CPowerUp*>(any_cast<Entity*>((*d)[ACTUAL_CAR])->GetComponent(CompType::PowerUpComp).get());
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        std::cout << "Power Up de la IA:   " << (int)cPowerUpCar->typePowerUp << std::endl;
    }
    //cPowerUp->typePowerUp = dynamic_cast<typeCPowerUp*>(indx);
}

// calcula si el otro coche se encuentra en el rango de vision del coche actual
bool ManCar::carInVisionRange(Entity* actualCar, Entity* otherCar, uint32_t rangeVision) {
    float seeCar = false;
    // calcular un desplazamiento para ser en tercera persona
    auto cTransformableActual = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float posXActualCar = cTransformableActual->position.x + 40 * cos((cTransformableActual->rotation.y * PI) / 180.0);
    float posZActualCar = cTransformableActual->position.z - 40 * sin((cTransformableActual->rotation.y * PI) / 180.0);

    // vector between actualCar and otherCar
    auto cTransformableOther = static_cast<CTransformable*>(otherCar->GetComponent(CompType::TransformableComp).get());
    float vetorWaypointX = (cTransformableOther->position.x - posXActualCar);
    float vetorWaypointZ = (cTransformableOther->position.z - posZActualCar);

    // calculate position rotated of otherCar atan2
    float valueAtan2 = atan2(vetorWaypointZ, vetorWaypointX) * 180 / PI;
    valueAtan2 = 180.0 - valueAtan2;  // se le restan ya que el eje empieza en el lado contrario
    if (valueAtan2 < 0)
        valueAtan2 += 360;

    //compare with actualCar actualRotation
    if (cTransformableActual->rotation.y - rangeVision >= 0 && cTransformableActual->rotation.y + rangeVision < 360) {
        if (cTransformableActual->rotation.y - rangeVision < valueAtan2 && cTransformableActual->rotation.y + rangeVision > valueAtan2) {
            seeCar = true;
        }
    } else {  // coge el angulo 0 de por medio
        float rotMin = cTransformableActual->rotation.y - rangeVision;
        float rotMax = cTransformableActual->rotation.y + rangeVision;
        if (cTransformableActual->rotation.y - rangeVision < 0)
            rotMin += 360;
        if (cTransformableActual->rotation.y + rangeVision >= 360)
            rotMax -= 360;
        if (rotMin < valueAtan2 || rotMax > valueAtan2) {
            seeCar = true;
        }
    }
    return seeCar;
}

// comprobamos si tenemos algun coche en el rango de vision
bool ManCar::anyCarInVisionRange(Entity* actualCar, uint32_t rangeVision) {
    bool seeCar = false;
    for (shared_ptr<Entity> cars : entities) {
        if (actualCar != cars.get() && carInVisionRange(actualCar, cars.get(), rangeVision) == true) {
            seeCar = true;
        }
    }
    return seeCar;
}

void ManCar::TurnLeftCar(DataMap* d) {
    // cout << "Han llamado izquierda" << endl;
    physics->TurnLeft(car.get(), cam);
}

void ManCar::NotTurning(DataMap* d) {
    // cout << "Han llamado not turning" << endl;
    physics->NotTurning(car.get(), cam);
}

void ManCar::Decelerate(DataMap* d) {
    // cout << "Han llamado decel" << endl;
    physics->Decelerate(car.get(), cam);
}

void ManCar::NotAcceleratingOrDecelerating(DataMap* d) {
    // cout << "Han llamado notaccel" << endl;
    physics->NotAcceleratingOrDecelerating(car.get(), cam);
}

void ManCar::TurnRightCar(DataMap* d) {
    // cout << "Han llamado derecha" << endl;
    physics->TurnRight(car.get(), cam);
}

void ManCar::AccelerateCar(DataMap* d) {
    // cout << "Estamos en AccelerateCar" << endl;
    // int mint = any_cast<int>(d["int"]);
    // cout << "Hemos obtenido el int" << mint << endl;
    // float mfloat = any_cast<float>(d["float"]);
    // cout << "Hemos obtenido el float" << endl;
    // vector<int> mvect = any_cast<vector<int>>(d["vector"]);
    // cout << "Hemos recibido por evento un int=" << mint << " un float=" << mfloat << " y un vector de int con size=" << mvect.size() << endl;

    physics->Accelerate(car.get(), cam);
}

void ManCar::Integrate(float delta) {
    //physics->update(GetCar().get(), cam.get());
}

// devuelve la entidad a por la que quiere ir el coche
Entity* ManCar::GetDesirableTarget(Entity* actualCar) {
    // va a tratar de disparar al que lleve el totem
    for(const auto& carAI : GetEntities()){
        if(carAI.get() != actualCar){
            auto cTotemCarAI = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());
            if (cTotemCarAI->active == true && actualCar != carAI.get())
                return carAI.get();
        }
    }
    // en caso de no llevarlo va a disparar al que mas tiempo tenga

    // en caso de no llevarlo va a disparar al que tenga en rango de vision
    CTransformable* closestCar = nullptr;
    Entity* closestCarEntity = nullptr;
    bool carPrincipal = false;
    if(actualCar != car.get()){
        closestCar = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
        closestCarEntity = car.get();
    }else{
        for(const shared_ptr<Entity>& carAI : entities){
            if(actualCar != carAI.get()){
                closestCar = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());
                closestCarEntity = carAI.get();
                carPrincipal = true;
            }
        }
    }
    auto cTransActualCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float vectorX = closestCar->position.x - cTransActualCar->position.x;
    float vectorZ = closestCar->position.z - cTransActualCar->position.z;
    float distanceMimum = sqrt((vectorX * vectorX) + (vectorZ * vectorZ));

    // reducimos cierta distancia en caso de que se encuentre en el radio de vision
    if (carPrincipal == true) {
        if (carInVisionRange(actualCar, closestCarEntity, 60) == true)
            distanceMimum = distanceMimum / 100.0;
    } else {
        if (carInVisionRange(actualCar, closestCarEntity, 60) == true)
            distanceMimum = distanceMimum / 100.0;
    }

    float distanceNext = 0.0;
    float vectorXNext = 0.0;
    float vectorZNext = 0.0;
    // Para CarAI
    for (shared_ptr<Entity> carAI : entities) {
        if (actualCar != carAI.get()) {
            auto cTransNextCar = static_cast<CTransformable*>(carAI.get()->GetComponent(CompType::TransformableComp).get());
            vectorXNext = cTransNextCar->position.x - cTransActualCar->position.x;
            vectorZNext = cTransNextCar->position.z - cTransActualCar->position.z;
            distanceNext = sqrt((vectorXNext * vectorXNext) + (vectorZNext * vectorZNext));

            if (carInVisionRange(actualCar, carAI.get(), 60) == true)
                distanceNext = distanceNext / 100.0;
            if (distanceMimum > distanceNext) {
                distanceMimum = distanceNext;
                closestCar = cTransNextCar;
                closestCarEntity = carAI.get();
            }
        }
    }
    return closestCarEntity;
}