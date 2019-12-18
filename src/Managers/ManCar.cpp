#include "ManCar.h"
#include <functional>
#include <iostream>
#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Physics.h"
#include "../Components/CPowerUp.h"
#include "../Components/CShield.h"
#include "../Components/CTotem.h"
#include "../Components/CRoboJorobo.h"
#include "../Components/CNitro.h"
#include "../Facade/Render/RenderFacadeManager.h"


class Position;
using namespace std;

ManCar::ManCar() {
    SubscribeToEvents();
    CreateMainCar();
    cout << "Hemos creado un powerup, ahora tenemos " << entities.size() << " powerups" << endl;
}

// TODO: este paso de physics es kk, hay que revisarlo de enviarlo como referencia o algo pero me da error
ManCar::ManCar(Physics *_physics, Camera *_cam) : ManCar() {
    this->physics = _physics;
    cout << "Tenemos en ManCar un physics con dir de memoria " << physics << endl;
    this->cam = _cam;
}

ManCar::~ManCar() {
    cout << "Llamando al destructor de ManCar" << endl;
    CarAIs.clear();
    CarAIs.shrink_to_fit();
}

void ManCar::CreateMainCar() {
    car = make_shared<Car>();
}

void ManCar::CreateCar() {
    shared_ptr<Car> p = make_shared<Car>();
    entities.push_back(p);
}


void ManCar::CreateCarAI(glm::vec3 _position,  CWayPoint* _waypoint) 
{
	shared_ptr<CarAI> p = make_shared<CarAI>(_position);
    CarAIs.push_back(p);
    p->SetWayPoint(_waypoint); // tiene que tener un waypoint inicial To-Do: cambiar esto
}


void ManCar::CreateCarAI() 
{
	shared_ptr<CarAI> p = make_shared<CarAI>();
    CarAIs.push_back(p);
}

void ManCar::SubscribeToEvents() {
    // auto accelerateCar = [&](Data d) {
    //     cout << "Estamos ejecutando lambda " << endl;
    //     /*Car *c = this->GetCar().get();
    //     auto components = c->GetComponents();
    //     CTransformable *trans = static_cast<CTransformable*>(components[CompType::TransformableComp].get()); 
    //     cout << "El coche está en  " << trans->position.x << "," << trans->position.y << "," << trans->position.z << endl;*/
    //     //physics->Accelerate(car.get(), cam);
    // };

    /*int (*accelerate) (Data) = [&](Data d) {
        physics->Accelerate(car.get(), cam);
        return 0;
    };*/
    //int (*func_pointer) (int) = [](int a) { return a; };

    //accelerateCar(0);
    auto lambdaAccelerate = [&](DataMap d){this->AccelerateCar(d);};
    //
    //auto lambdaGuardaAccel = [&lambdaAccelerate](DataMap d) {lambdaAccelerate(d);};


    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_I,
        lambdaAccelerate,
        // [&](DataMap d){this->AccelerateCar(d);},
        "AccelerateCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_O,
        bind(&ManCar::Decelerate, this, placeholders::_1),
        "Decelerate"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_A,
        bind(&ManCar::TurnLeftCar, this, placeholders::_1),
        "TurnLeftCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_D,
        bind(&ManCar::TurnRightCar, this, placeholders::_1),
        "TurnRightCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::NO_I_O_PRESS,
        bind(&ManCar::NotAcceleratingOrDecelerating, this, placeholders::_1),
        "NotAcceleratingOrDecelerating"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::NO_A_D_PRESS,
        bind(&ManCar::NotTurning, this, placeholders::_1),
        "NotTurning"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CATCH_BOX_POWERUP,
        bind(&ManCar::CatchPowerUp, this, placeholders::_1),
        "CatchPowerUp"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CATCH_AI_BOX_POWERUP,
        bind(&ManCar::CatchPowerUpAI, this, placeholders::_1),
        "CatchPowerUpAI"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_SPACE,
        bind(&ManCar::ThrowPowerUp, this, placeholders::_1),
        "ThrowPowerUp"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_ENTITY_POWERUP,
        bind(&ManCar::CollisionPowerUp, this, placeholders::_1),
        "CollisionPowerUp"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_ENTITY_AI_POWERUP,
        bind(&ManCar::CollisionPowerUpAI, this, placeholders::_1),
        "CollisionPowerUpAI"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_PLAYER_TOTEM,
        bind(&ManCar::CatchTotemPlayer, this, placeholders::_1),
        "CatchTotemPlayer"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_AI_TOTEM,
        bind(&ManCar::CatchTotemAI, this, placeholders::_1),
        "CatchTotemAI"));

}



void ManCar::CatchTotemAI(DataMap d){
    std::cout << "Coge el totem una IA -- ERES MUUUY MALO COLEGA!" << std::endl;
    auto cTotem = static_cast<CTotem*>(any_cast<Entity*>(d["actualCar"])->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();

}

void ManCar::CatchTotemPlayer(DataMap d){
    std::cout << "Cogemos el Totem locoooo!" << std::endl;
    auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();

}

void ManCar::CatchTotem(Car* carWinTotem){
    auto cTotem = static_cast<CTotem*>(carWinTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();
    std::cout << "Has utilizado el robo jorobo de p*** madre coleeega ahora es tuyo" << std::endl; 
}

void ManCar::ThrowTotem(Car* carLoseTotem){
    auto cTotem = static_cast<CTotem*>(carLoseTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = false;
    cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
    std::cout << "El tiempo acumulado del totem hasta ahora del jugador es de:  " << cTotem->accumulatedTime/1000.0 << std::endl; 
}


bool ManCar::useRoboJorobo(Car* newCarWithTotem){
    // recorremos las IA
    for(shared_ptr<Entity> AIcar : CarAIs){
        auto cTotem = static_cast<CTotem*>(AIcar.get()->GetComponent(CompType::TotemComp).get()); 
        // Si algun coche tenia el totem .... lo pierde
        if(cTotem->active == true){
            ThrowTotem(static_cast<Car*>(AIcar.get()));
            //al perderlo se lo asignamos al que ha usado el robo jorobo
            CatchTotem(newCarWithTotem);

            return true;                                                               // para salirnos y no hacer mas calculos
        }
    }
    // comprobamos el player
    auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get()); 
    if(cTotem->active == true){
        ThrowTotem(car.get());
        //al perderlo se lo asignamos al que ha usado el robo jorobo
        CatchTotem(newCarWithTotem);
        return true;                                                               // para salirnos y no hacer mas calculos
    }

    return false;
}



void ManCar::CollisionPowerUp(DataMap d){
    std::cout << "Nos ha dado un powerUp neneeeee!" << std::endl;
    // debemos desactivar el powerUp y para el contador de tiempo del totem
    auto cShield = static_cast<CShield*>(car.get()->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp == false){            // comprobamos si tiene el escudo
        auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            ThrowTotem(car.get());
        }
        // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
        auto cCar = static_cast<CCar*>(car.get()->GetComponent(CompType::CarComp).get());
        cCar->speed = 0.0f;
    }else{
        std::cout << "El escudo me salvo el culito :D" << std::endl;
        cShield->deactivePowerUp(); // desactivamos el escudo
    }
}


void ManCar::CollisionPowerUpAI(DataMap d){
    // debemos desactivar el powerUp y para el contador de tiempo del totem
    auto cShield = static_cast<CShield*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp == false){            // comprobamos si tiene el escudo
        auto cTotem = static_cast<CTotem*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            ThrowTotem(static_cast<Car*>(any_cast<Entity*>(d["carAI"])));
        }
        // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
        auto cCar = static_cast<CCar*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::CarComp).get());
        cCar->speed = 0.0f;  // To-Do: no funciona en la IA por que la logica difusa no la hace acelerar
    }else{
        std::cout << "El escudo me salvo el culito :D" << std::endl;
        cShield->deactivePowerUp(); // desactivamos el escudo
    }
}



// calcula el coche IA mas cercano, se introduce el primer elemento de la IA por defecto
CTransformable* ManCar::calculateCloserAI(){
    CTransformable* closestAI = static_cast<CTransformable*>(CarAIs[0].get()->GetComponent(CompType::TransformableComp).get());
    auto cTransCar = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());
    auto cTransCarAI = static_cast<CTransformable*>(CarAIs[0].get()->GetComponent(CompType::TransformableComp).get());
    float vectorX = cTransCarAI->position.x - cTransCar->position.x;
    float vectorZ = cTransCarAI->position.z - cTransCar->position.z;
    float distanceMimum = sqrt((vectorX+vectorX) + (vectorZ+vectorZ));

    float distanceNext = 0.0;
    float vectorXNext = 0.0;
    float vectorZNext = 0.0;
    
    for(shared_ptr<CarAI> AIcar : CarAIs){
        auto cTransCarAI2 = static_cast<CTransformable*>(AIcar.get()->GetComponent(CompType::TransformableComp).get()); 
        vectorXNext = cTransCarAI2->position.x - cTransCar->position.x;     
        vectorZNext = cTransCarAI2->position.z - cTransCar->position.z;
        distanceNext = sqrt((vectorXNext+vectorXNext) + (vectorZNext+vectorZNext));
        if(distanceMimum > distanceNext){
            distanceMimum = distanceNext;
            closestAI = cTransCarAI2;
        }
    }

    return closestAI;
}



void ManCar::ThrowPowerUp(DataMap d) {
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    auto cRoboJorobo = static_cast<CRoboJorobo*>(car->GetComponent(CompType::RoboJoroboComp).get());
    auto cShield = static_cast<CShield*>(car.get()->GetComponent(CompType::ShieldComp).get());
    auto cNitro = static_cast<CNitro*>(car.get()->GetComponent(CompType::NitroComp).get());
    bool robado = false;  
    if(cPowerUpCar->typePowerUp != typeCPowerUp::None){

        switch (cPowerUpCar->typePowerUp){
            case typeCPowerUp::RoboJorobo:
                //cRoboJorobo->activatePowerUp();
                robado = useRoboJorobo(car.get());
                if (!robado)
                    std::cout << "La has cagado, el Totem no lo tenia nadie..." << std::endl; 
                break;
            case typeCPowerUp::EscudoMerluzo:
                cShield->activatePowerUp();
                break;
            case typeCPowerUp::SuperMegaNitro:
                cNitro->activatePowerUp();
                break;
            default:
                shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                DataMap d;
                d["typePowerUp"] = cPowerUpCar->typePowerUp;
                d["posCocheSalida"] = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());;
                // To-Do: actualmente solo se pasa el coche mas cercano, calcular aparte si se encuentra en pantalla
                d["posCochePerseguir"] = calculateCloserAI(); // To-Do: se le mete el coche desde el que sale, deberia ser el que persigue
                eventManager->AddEventMulti(Event{EventType::PowerUp_Create, d});

                break;
        }
        //std::cout << "Lanzamos el power up fiiiuuuuum" << std::endl;
        cPowerUpCar->typePowerUp = typeCPowerUp::None;
        DataMap d;
        d["typePowerUp"] = cPowerUpCar->typePowerUp;
        EventManager::GetInstance()->AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, d});
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

void ManCar::CatchPowerUp(DataMap d) {
    srand(time(NULL));
    int indx = rand() % 6+1;
    //None,               // 0
    //RoboJorobo,         // 1
    //SuperMegaNitro,     // 2
    //PudinDeFrambuesa,   // 3
    //EscudoMerluzo,      // 4
    //TeleBanana,         // 5
    //MelonMolon          // 6
    //indx = 3;
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        std::cout << "Mi super powerUp es:   " << (int)cPowerUpCar->typePowerUp << std::endl;
        d["typePowerUp"] = cPowerUpCar->typePowerUp;

        //RenderFacadeManager::GetInstance()->GetRenderFacade()->FacadeUpdatePowerUpHUD(d);
        EventManager::GetInstance()->AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, d});

    }
    //cPowerUp->typePowerUp = dynamic_cast<typeCPowerUp*>(indx);
}

void ManCar::CatchPowerUpAI(DataMap d) {
    srand(time(NULL));
    int indx = rand() % 6+1;
    //indx = 5;
    //indx=3;
    auto cPowerUpCar = static_cast<CPowerUp*>(any_cast<Entity*>(d["actualCar"])->GetComponent(CompType::PowerUpComp).get());
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        std::cout << "Mi super powerUp es:   " << (int)cPowerUpCar->typePowerUp << std::endl;
    }
    //cPowerUp->typePowerUp = dynamic_cast<typeCPowerUp*>(indx);
}

void ManCar::TurnLeftCar(DataMap d) {
    // cout << "Han llamado izquierda" << endl;
    physics->TurnLeft(car.get(), cam);
}

void ManCar::NotTurning(DataMap d) {
    // cout << "Han llamado not turning" << endl;
    physics->NotTurning(car.get(), cam);
}

void ManCar::Decelerate(DataMap d) {
    // cout << "Han llamado decel" << endl;
    physics->Decelerate(car.get(), cam);
}

void ManCar::NotAcceleratingOrDecelerating(DataMap d) {
    // cout << "Han llamado notaccel" << endl;
    physics->NotAcceleratingOrDecelerating(car.get(), cam);
}

void ManCar::TurnRightCar(DataMap d) {
    // cout << "Han llamado derecha" << endl;
    physics->TurnRight(car.get(), cam);
}

void ManCar::AccelerateCar(DataMap d) {
    // cout << "Estamos en AccelerateCar" << endl;
    // int mint = any_cast<int>(d["int"]);
    // cout << "Hemos obtenido el int" << mint << endl;
    // float mfloat = any_cast<float>(d["float"]);
    // cout << "Hemos obtenido el float" << endl;
    // vector<int> mvect = any_cast<vector<int>>(d["vector"]);
    // cout << "Hemos recibido por evento un int=" << mint << " un float=" << mfloat << " y un vector de int con size=" << mvect.size() << endl;
    
    physics->Accelerate(car.get(), cam);
}
