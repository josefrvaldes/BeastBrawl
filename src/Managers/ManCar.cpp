#include "ManCar.h"
#include "ManBoundingWall.h"
#include "ManBoxPowerUp.h"
#include "ManNavMesh.h"
#include "ManPowerUp.h"
#include "ManTotem.h"

#include <Components/CBufferOnline.h>
#include <Components/CCar.h>
#include <Components/CDimensions.h>
#include <Components/CNitro.h>
#include <Components/CShield.h>
#include <Components/CTotem.h>
#include <Entities/Camera.h>
#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>
#include <Systems/Physics.h>
#include <Systems/PhysicsAI.h>
#include <functional>
#include <iostream>

#include "../Components/COnline.h"

#include "../Facade/Render/RenderFacadeManager.h"
#include "../Game.h"
#include "../Managers/ManBoundingWall.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManNavMesh.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Systems/SystemBtLoDMove.h"
#include "../Systems/SystemBtMoveTo.h"
#include "../Systems/SystemBtPowerUp.h"
#include "../Systems/SystemGameRules.h"
#include "../Systems/Utils.h"
#include "Manager.h"

#include <Facade/Sound/SoundFacadeManager.h>

class Position;
using namespace std;

ManCar::ManCar() {
    SubscribeToEvents();
    CreateMainCar();

    // systemPathPlanning = make_unique<SystemPathPlanning>();
    //physicsAI = make_unique<PhysicsAI>();
    systemGameRules = make_unique<SystemGameRules>();

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
void ManCar::UpdateCarPlayer(ManTotem &manTotem_) {
    auto totem = manTotem_.GetEntities()[0].get();
    auto carPlayer =  static_cast<Entity*>(this->GetCar().get());
    UpdateGeneralCar(*carPlayer, *totem);
}

// TODO: RECORDARRR!!!!!!!!!!!!!!!!!  TANTO EL "BtMoveTo" como el "systemPathPlanning" se deben hacer en la misma ITERACION!!!!
// Es importante esto porque el BtMoveTo es el que calcula la posicion a la que ir y el systemBtLoDMove es el que utiliza esta posicion para
// moverse a un sitio, si en algun momento intentamos ir a una posicion que no existe PETAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
void ManCar::UpdateCarAI(CarAI* carAI, ManPowerUp* m_manPowerUp, ManBoxPowerUp* m_manBoxPowerUp, ManTotem* m_manTotem, ManWayPoint* graph, ManNavMesh* manNavMesh, 
                        ManBoundingWall* m_manBoundingWall, SystemBtPowerUp* systemBtPowerUp, SystemBtMoveTo* systemBtMoveTo, SystemBtLoDMove* systemBtLoDMove, SystemPathPlanning *systemPathPlanning) {
    
    //manNavMesh->UpdateNavMeshEntity(carAI);
    systemBtMoveTo->update(carAI, this, m_manPowerUp, m_manBoxPowerUp, m_manTotem, graph, manNavMesh);

    systemPathPlanning->Update(carAI, graph, manNavMesh);

    systemBtLoDMove->update(carAI, this, m_manPowerUp, m_manBoxPowerUp, m_manTotem, graph, manNavMesh, m_manBoundingWall);

    //physicsAI->Update(carAI, graph);
    UpdateGeneralCar(*carAI, *(m_manTotem->GetEntities()[0].get()));

    systemBtPowerUp->update(carAI, this, m_manPowerUp, m_manBoxPowerUp, m_manTotem, graph, manNavMesh);
    
}
void ManCar::UpdateCarHuman(Entity* CarHuman, ManTotem* m_manTotem) {
    UpdateGeneralCar(*CarHuman, *(m_manTotem->GetEntities()[0].get()));
}

void ManCar::UpdateGeneralCar(Entity& car_, Entity& totem_){
    systemGameRules->UpdateRulesCarPowerUps(car_, totem_);
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
        EventType::PRESS_SKID,
        bind(&ManCar::SkidCar, this, placeholders::_1),
        "SkidCar"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NOT_SKID_PRESS,
        bind(&ManCar::NotSkid, this, placeholders::_1),
        "NotSkid"));

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
        EventType::COLLISION_CAR_POWERUP,
        bind(&ManCar::CollisionCarPowerUp, this, placeholders::_1),
        "CollisionCarPowerUp"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_PLAYER_TOTEM,
        bind(&ManCar::CatchTotemCar, this, placeholders::_1),
        "CatchTotemCar"));

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
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_CATCH_PU_RECEIVED,
        bind(&ManCar::NewCatchPUReceived, this, placeholders::_1),
        "NewCatchPUReceived"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_CATCH_TOTEM_RECEIVED,
        bind(&ManCar::NewCatchTotemReceived, this, placeholders::_1),
        "NewCatchTotemReceived"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_LOST_TOTEM_RECEIVED,
        bind(&ManCar::NewLostTotemReceived, this, placeholders::_1),
        "NewLostTotemReceived"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_ROBOJOROBO_RECEIVED,
        bind(&ManCar::NewUsedRoboJoroboReceived, this, placeholders::_1),
        "NewUsedRoboJoroboReceived"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_NITRO_RECEIVED,
        bind(&ManCar::NewCollideNitroReceived, this, placeholders::_1),
        "NewCollideNitroReceived"));
    
    /*EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::DISCONNECTED_PLAYER,
        bind(&ManCar::DeletePlayer, this, placeholders::_1),
        "DeletePlayer"));*/
}


///////////////////////////////////////////////////////////////////////7
//                    EVENTOS DEL ONLINE
///////////////////////////////////////////////////////////////////////

void ManCar::NewInputsReceived(DataMap* d) {
    // cout << "Se ha lanzado el evento NewInputsReceived" << endl;
    auto idRecieved = any_cast<uint16_t>((*d)[DataType::ID]);
    // cout << Utils::getISOCurrentTimestampMillis() << " Hemos recibido un input del id " << idRecieved << endl;
    auto inputs = any_cast<vector<Constants::InputTypes>>((*d)[DataType::INPUTS]);
    for (const auto& car : GetEntities()) {
        if (car->HasComponent(CompType::OnlineComp) && car->HasComponent(CompType::BufferOnline)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            uint16_t currentIDOnline = compOnline->idClient;
            if (currentIDOnline == idRecieved) {

                compOnline->inputs = inputs;
                break;
            }
        }
    }
}

void ManCar::NewSyncReceived(DataMap* d) {
    // cout << "Se ha lanzado el evento NewInputsReceived" << endl;
    auto idRecieved = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    // cout << Utils::getISOCurrentTimestampMillis() << " Hemos recibido un sync del id " << idRecieved << endl;
    for (const auto& car : GetEntities()) {
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


// establecemos el Power Up que ha cogido
void ManCar::NewCatchPUReceived(DataMap* d) {
    auto idRecieved = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    for (const auto& car : GetEntities()) {
        if (car->HasComponent(CompType::OnlineComp)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            if (compOnline->idClient == idRecieved) {
                auto cPowerUp = static_cast<CPowerUp*>(car->GetComponent(CompType::PowerUpComp).get());
                cPowerUp->typePowerUp = any_cast<typeCPowerUp>((*d)[DataType::TYPE_POWER_UP]);
                break;
            }
        }
    }
}


// establecemos quien ha cogido el totem
void ManCar::NewCatchTotemReceived(DataMap* d) {
    auto idCarRecieved = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    for (const auto& car : GetEntities()) {
        if (car->HasComponent(CompType::OnlineComp)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
            if (compOnline->idClient == idCarRecieved) {
                ObtainTotem(car.get());
            }else{
                cTotem->active = false;
            }
        }
    }
}


// establecemos quien ha perdido el totem
void ManCar::NewLostTotemReceived(DataMap* d) {
    auto idCarLosted = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    for (const auto& car : GetEntities()) {
        if (car->HasComponent(CompType::OnlineComp)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            if (compOnline->idClient == idCarLosted) {
                ThrowTotem(car.get());
            }
        }
    }
}

// establecemos quien ha cogido el totem y quien lo ha perdido
void ManCar::NewUsedRoboJoroboReceived(DataMap* d) {
    auto idCarObtained = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    for (const auto& car : GetEntities()) {
        if (car->HasComponent(CompType::OnlineComp)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
            if (compOnline->idClient == idCarObtained) {
                ObtainTotem(car.get());
            }else if(cTotem->active == true){
                ThrowTotem(car.get());
            }
        }
    }
}


void ManCar::NewCollideNitroReceived(DataMap* d) {
    auto idCarObtainedTotem = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    for (const auto& car : GetEntities()) {
        if (car->HasComponent(CompType::OnlineComp)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
            if (compOnline->idClient == idCarObtainedTotem) {
                ObtainTotem(car.get());
            }else if(cTotem->active == true){
                ThrowTotem(car.get());
            }
        }
    }
}

/*void ManCar::DeletePlayer(DataMap* d){
    auto idRecieved = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    for (auto& car : GetEntities()) {
        if (car->HasComponent(CompType::OnlineComp)) {
            COnline* compOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            if (compOnline->idClient == idRecieved) {
                auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
                if(cTotem->active){     // comprobamos si tiene totem para quitarselo
                    ThrowTotem(car.get());
                    // auto dataTransformableCar = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());
                    // shared_ptr<DataMap> dataTransfCar = make_shared<DataMap>();                                                        
                    // (*dataTransfCar)[CAR_TRANSFORMABLE] = dataTransformableCar;  
                    // (*dataTransfCar)[ACTUAL_CAR] = car; 
                    // (*dataTransfCar)[MAN_NAVMESH] = manNavMesh;
                    // EventManager::GetInstance().AddEventMulti(Event{EventType::DROP_TOTEM, dataTransfCar}); 
                    auto renderFacadeManager = RenderFacadeManager::GetInstance();
                    auto renderEngine = renderFacadeManager->GetRenderFacade();
                    for(auto i=0; i< entities.size(); ++i){
                        if(entities[i] == car){
                            renderEngine->DeleteEntity(entities[i].get());
                            entities.erase(entities.begin()+i);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}*/

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////




void ManCar::ChangeTotemCar(DataMap* d) {
    auto carWithTotem = any_cast<Entity*>((*d)[CAR_WITH_TOTEM]);
    auto carWithoutTotem = any_cast<Entity*>((*d)[CAR_WITHOUT_TOTEM]);
    if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_SINGLE){
        ThrowTotem(carWithTotem);
        // activamos el totem al coche que ahora lo tiene
        ObtainTotem(carWithoutTotem);
    }else if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_MULTI){
        if(carWithTotem == GetCar().get() || carWithoutTotem == GetCar().get()){
            auto cOnlineCarWithTotem = static_cast<COnline*>(carWithTotem->GetComponent(CompType::OnlineComp).get());
            auto cOnlineCarWithoutTotem = static_cast<COnline*>(carWithoutTotem->GetComponent(CompType::OnlineComp).get());
            systemOnline->SendNitro(cOnlineCarWithTotem->idClient, cOnlineCarWithoutTotem->idClient);
        }
    }
}



void ManCar::CatchTotemCar(DataMap* d) {    
    if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_SINGLE){ // estamos en solo
        ObtainTotem(any_cast<Entity*>((*d)[ACTUAL_CAR]));
    }else if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_MULTI){
        auto cOnline = static_cast<COnline*>(any_cast<Entity*>((*d)[ACTUAL_CAR])->GetComponent(CompType::OnlineComp).get());
        systemOnline->SendCatchTotem(cOnline->idClient);
    }
}


void ManCar::ObtainTotem(Entity* carWinTotem) {
    auto cTotem = static_cast<CTotem*>(carWinTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();

    auto cTransformable = static_cast<CTransformable*>(carWinTotem->GetComponent(CompType::TransformableComp).get());

    // Sonido coger totem
    shared_ptr<DataMap> data = make_shared<DataMap>();
    (*data)[VEC3_POS] = cTransformable->position;
    EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_TOTEM, data});
}

void ManCar::ThrowTotem(Entity* carLoseTotem) {
    auto cTotem = static_cast<CTotem*>(carLoseTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = false;
    cTotem->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
    //std::cout << "El tiempo acumulado del totem hasta ahora del jugador es de:  " << cTotem->accumulatedTime/1000.0 << std::endl;
}

bool ManCar::useRoboJorobo(Entity* newCarWithTotem) {
    // recorremos los coches
    if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_SINGLE){
        for (const auto& cars : entities) {
            auto cTotem = static_cast<CTotem*>(cars->GetComponent(CompType::TotemComp).get());
            // Si algun coche tenia el totem .... lo pierde, comprobamos que no sea el mmismo coche con las ID
            if (cTotem->active == true && newCarWithTotem != cars.get()) {
                ThrowTotem(cars.get());
                //al perderlo se lo asignamos al que ha usado el robo jorobo
                ObtainTotem(newCarWithTotem);
                return true;  // para salirnos y no hacer mas calculos
            }
        }
    }else if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_MULTI){
        if(newCarWithTotem == GetCar().get()){
            systemOnline->SendRoboJorobo();
        }
    }
    return false;
}


void ManCar::CollisionCarPowerUp(DataMap* d) {
    auto car = any_cast<Entity*>((*d)[ACTUAL_CAR]);
    
    // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
    auto cCar = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());
    cCar->speed = 0.0f;  // To-Do: no funciona en la IA por que la logica difusa no la hace acelerar
        
    // Sonido choque con powerup
    shared_ptr<DataMap> dataSound = make_shared<DataMap>();
    auto cTransf = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
    auto cIdMainCar = static_cast<CId*>(GetCar()->GetComponent(CompType::IdComp).get());
    (*dataSound)[VEC3_POS] = cTransf->position;
    (*dataSound)[ID] = cId->id;
    (*dataSound)[MAIN_CAR] = false;
    if (cIdMainCar->id == cId->id) {
        (*dataSound)[MAIN_CAR] = true;
    }
    EventManager::GetInstance().AddEventMulti(Event{EventType::HURT, dataSound});

    // debemos desactivar el powerUp y para el contador de tiempo del totem
    if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_SINGLE){
        auto cTotem = static_cast<CTotem*>(any_cast<Entity*>((*d)[ACTUAL_CAR])->GetComponent(CompType::TotemComp).get());
        if (cTotem->active == true) {
            ThrowTotem(car);
        }
    }


}


// se calcula el coche mas cercano en el rango de vision, en caso de no haber se devuelve un nullptr
Car* ManCar::calculateCloserCar(Entity* actualCar) {
    // Primero metemos al jugador por lo que no hace falta calcularlo luego
    Car* closestCar = nullptr;
    auto cTransActualCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float distanceMimum = 9999999999.0;
    float distanceNext;
    float vectorXNext;
    float vectorZNext;

    // Para todos los coches
    for (const shared_ptr<Entity>& car : entities) {
        if (actualCar != car.get() && carInVisionRange(actualCar, car.get(), 60) == true) {
            auto cTransNextCar = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());
            vectorXNext = cTransNextCar->position.x - cTransActualCar->position.x;
            vectorZNext = cTransNextCar->position.z - cTransActualCar->position.z;
            distanceNext = sqrt((vectorXNext * vectorXNext) + (vectorZNext * vectorZNext));

            if (distanceMimum > distanceNext) {
                distanceMimum = distanceNext;
                closestCar = static_cast<Car*>(car.get());
            }
        }
    }
    return closestCar;
}

void ManCar::ThrowPowerUpCar(DataMap* d) {
    ThrowPowerUp( static_cast<Car *>(car.get()) );
}

void ManCar::ThrowPowerUpAI(DataMap* d) {
    ThrowPowerUp(any_cast<CarAI *>((*d)[ACTUAL_CAR]));
}

void ManCar::ThrowPowerUpHuman(DataMap* d) {
    ThrowPowerUp(any_cast<Car *>((*d)[ACTUAL_CAR]));
}

void ManCar::ThrowPowerUp(Car* car_) {
    shared_ptr<DataMap> dataSound = make_shared<DataMap>();
    
    auto cIdCar = static_cast<CId*>(car_->GetComponent(CompType::IdComp).get());
    auto cPowerUpCar = static_cast<CPowerUp*>(car_->GetComponent(CompType::PowerUpComp).get());
    //auto cRoboJorobo = static_cast<CRoboJorobo*>(any_cast<CarAI*>(d[ACTUAL_CAR])->GetComponent(CompType::RoboJoroboComp).get());
    auto cShield = static_cast<CShield*>(car_->GetComponent(CompType::ShieldComp).get());
    auto cNitro = static_cast<CNitro*>(car_->GetComponent(CompType::NitroComp).get());
    bool robado = false; 
    
    if(cPowerUpCar->typePowerUp != typeCPowerUp::None){
        shared_ptr<DataMap> data = make_shared<DataMap>();

        
        switch (cPowerUpCar->typePowerUp) {
            case (typeCPowerUp::RoboJorobo):
                robado = useRoboJorobo(car_);
                if (!robado)
                    std::cout << "La has cagado, el Totem no lo tenia nadie..." << std::endl;
                break;

            case (typeCPowerUp::EscudoMerluzo):
                cShield->activatePowerUp();
                //Para el sonido
                (*dataSound)[ID] = cIdCar->id;
                break;

            case (typeCPowerUp::SuperMegaNitro):
                cNitro->activatePowerUp();
                break;
            case (typeCPowerUp::TeleBanana): {
                if(CheckIfPUWillBeFired(car_)) {
                    Car* objectiveCar = calculateCloserCar(car_);
                    if(objectiveCar != nullptr) {
                        (*data)[CAR_FOLLOW] = objectiveCar;
                    }
                    (*data)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
                    (*data)[CAR_EXIT_POSITION] = static_cast<CTransformable*>(car_->GetComponent(CompType::TransformableComp).get());
                    (*data)[CAR_EXIT_DIMENSION] =  static_cast<CDimensions*>(car_->GetComponent(CompType::DimensionsComp).get());
                    EventManager::GetInstance().AddEventMulti(Event{EventType::PowerUp_Create, data});
                }
            } break;
            default:  // en caso del melon molon o el pudding
                if(CheckIfPUWillBeFired(car_)) {
                    (*data)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
                    (*data)[CAR_EXIT_POSITION] = static_cast<CTransformable*>(car_->GetComponent(CompType::TransformableComp).get());
                    (*data)[CAR_EXIT_DIMENSION] =  static_cast<CDimensions*>(car_->GetComponent(CompType::DimensionsComp).get());
                    EventManager::GetInstance().AddEventMulti(Event{EventType::PowerUp_Create, data});
                }
                break;
        }


        // Sonido de lanzar power-up
        auto mainCarId = static_cast<CId*>(GetCar()->GetComponent(CompType::IdComp).get());
        (*dataSound)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
        (*dataSound)[MAIN_CAR] = false;
        if (mainCarId && cIdCar && mainCarId->id == cIdCar->id) {
            (*dataSound)[MAIN_CAR] = true;
        }
        EventManager::GetInstance().AddEventMulti(Event{EventType::THROW_POWERUP, dataSound});

        if(car_ == GetCar().get()){

            // Ya no tenemos power-up
            shared_ptr<DataMap> dataHUD = make_shared<DataMap>();

            cPowerUpCar->typePowerUp = typeCPowerUp::None;
            (*dataHUD)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
            EventManager::GetInstance().AddEventMulti(Event{UPDATE_POWERUP_HUD, dataHUD}); 
        }
        cPowerUpCar->typePowerUp = typeCPowerUp::None;
    }
}

bool ManCar::CheckIfPUWillBeFired(Car* car_) {
    bool send = true;
    if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_MULTI) {
        // si no es el jugador original, no enviará el powerup porque eso lo gestiona el server
        COnline *cOnlineCarReceived = static_cast<COnline*>(car_->GetComponent(CompType::OnlineComp).get());
        COnline *cOnlineMainCar = static_cast<COnline*>(GetCar()->GetComponent(CompType::OnlineComp).get());
        if(cOnlineCarReceived->idClient != cOnlineMainCar->idClient)
            send = false;
    }
    return send;
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
    // NO ENTRA AQUI YAAAA
    // To-Do: porcentajes temporales
    /*srand(time(NULL));
    int indx = rand() % 100 + 1;
    if (indx <= 5)  // 5%
        indx = 1;
    else if (indx > 5 && indx <= 25)  // 15%
        indx = 2;
    else if (indx > 25 && indx <= 50)  // 20%
        indx = 3;
    else if (indx > 50 && indx <= 65)  // 15%
        indx = 4;
    else if (indx > 65 && indx <= 80)  // 15%
        indx = 5;
    else if (indx > 80)  //  30%
        indx = 6;

    //indx = 3;
    //None,               // 0
    //RoboJorobo,         // 1
    //SuperMegaNitro,     // 2
    //PudinDeFrambuesa,   // 3
    //EscudoMerluzo,      // 4
    //TeleBanana,         // 5
    //MelonMolon          // 6
    //indx = 6;
    auto cPowerUpCar = static_cast<CPowerUp*>(car->GetComponent(CompType::PowerUpComp).get());
    if (cPowerUpCar->typePowerUp == typeCPowerUp::None) {
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;

        shared_ptr<DataMap> data = make_shared<DataMap>();
        (*data)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
        EventManager::GetInstance().AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, data});

        if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_MULTI){
            systemOnline->SendCatchPU(*cPowerUpCar);
        }
    }*/
}

void ManCar::CatchPowerUpAI(DataMap* d) {

    int maxRobojorobo = 50;
    int maxNitro = maxRobojorobo + 150;
    int maxPudin = maxNitro + 250;
    int maxEscudo = maxPudin + 150;
    int maxTelebanana = maxEscudo + 150;
    //No voy a poner el MelonMolon porque no se usa para comparar. De base tiene un 20%, del [80-100]

    auto actualCar = any_cast<Entity*>((*d)[ACTUAL_CAR]);
    auto cTotem = static_cast<CTotem*>(actualCar->GetComponent(CompType::TotemComp).get());
    //cout << "------------- POSICION: " << cTotem->positionRanking << endl;

    if (cTotem) {
        auto cPositionRanking = cTotem->positionRanking;
        if (cPositionRanking == 1 || cPositionRanking == 2) {
            //cout << "------------- SOY EL 1/2" << endl;
            maxRobojorobo -= 50;
            maxNitro -= 75;
            maxPudin += 100;
            maxEscudo -= 50;
            maxTelebanana -= 50;

            if (cPositionRanking == 2) {
                maxRobojorobo -= 25;
            }

        } else if ( cPositionRanking == entities.size() || (entities.size() > 3 && cPositionRanking == (entities.size()-1))){
            //cout << "------------- SOY EL" << (entities.size()-1) << "/" << entities.size() << endl;
            maxRobojorobo += 50;
            maxPudin -= 150;
            maxTelebanana += 100;
            maxNitro += 25;
        }
    }

    srand(time(nullptr));
    int indx = rand() % 1000 + 1;
    typeCPowerUp type = typeCPowerUp::None;
    if (indx <= maxRobojorobo)  // ROBOJOROBO -> 5% base - 0% primero - 2.5% segundo - 10% ultimo/s
        type = typeCPowerUp::RoboJorobo;
    else if (indx > maxRobojorobo && indx <= maxNitro)  // SUPERMEGANITRO -> 15% base - 175% primero - 12.5% segundo - 17.5% ultimo/s
        type = typeCPowerUp::SuperMegaNitro;
    else if (indx > maxNitro && indx <= maxPudin)  // PUDIN -> 25% base - 35% primero - 35% segundo - 10% ultimo/s
        type = typeCPowerUp::PudinDeFrambuesa;
    else if (indx > maxPudin && indx <= maxEscudo)  // ESCUDO -> 15% base - 10% primero - 10% segundo - 15% ultimo/s
        type = typeCPowerUp::EscudoMerluzo;
    else if (indx > maxEscudo && indx <= maxTelebanana)  // TELEBANANA -> 15% base - 10% primero - 10% segundo - 25% ultimo/s
        type = typeCPowerUp::TeleBanana;
    else if (indx > maxTelebanana)  // MELONMOLON ->  25% base - 37.5% primero - 35% segundo - 22.5% ultimo/s
        type = typeCPowerUp::MelonMolon;


    //type = typeCPowerUp::MelonMolon;
    auto cPowerUpCar = static_cast<CPowerUp*>(actualCar->GetComponent(CompType::PowerUpComp).get());
    if (cPowerUpCar->typePowerUp == typeCPowerUp::None) {
        cPowerUpCar->typePowerUp = type;
        std::cout << "Power Up del coche es:   " << (int)cPowerUpCar->typePowerUp << std::endl;
        if(this->GetCar().get() == any_cast<Entity*>((*d)[ACTUAL_CAR])){
            shared_ptr<DataMap> data = make_shared<DataMap>();
            (*data)[TYPE_POWER_UP] = cPowerUpCar->typePowerUp;
            EventManager::GetInstance().AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, data});

            if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_MULTI){
                cout << "Se envia el PU mensaje 2" << "\n";
                systemOnline->SendCatchPU(*cPowerUpCar);
            }
        }
    }
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

// comprobamos si el coche indicado en nuestro rango de vision // el rango de vision sera 60
bool ManCar::CarTotemInVisionRange(Entity* actualCar, Entity* desCar, uint32_t rangeVision) {
    if (carInVisionRange(actualCar, desCar, rangeVision) == true)
        return true;
    return false;
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

void ManCar::SkidCar(DataMap* d) {
    physics->Skid(car.get(), cam);
}

void ManCar::NotSkid(DataMap* d) {
    physics->NotSkidding(car.get(), cam);
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
    for (const auto& carAI : GetEntities()) {
        if (carAI.get() != actualCar) {
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
    if (actualCar != car.get()) {
        closestCar = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
        closestCarEntity = car.get();
    } else {
        for (const shared_ptr<Entity>& carAI : entities) {
            if (actualCar != carAI.get()) {
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