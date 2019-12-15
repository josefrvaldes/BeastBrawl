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
        EventType::PRESS_C,
        bind(&ManCar::CatchPowerUp, this, placeholders::_1),
        "CatchPowerUp"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_SPACE,
        bind(&ManCar::ThrowPowerUp, this, placeholders::_1),
        "ThrowPowerUp"));

}


void ManCar::ThrowPowerUp(DataMap d) {
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    auto cRoboJorobo = static_cast<CRoboJorobo*>(car->GetComponent(CompType::RoboJoroboComp).get());
    auto cShield = static_cast<CShield*>(car.get()->GetComponent(CompType::ShieldComp).get());
    auto cNitro = static_cast<CNitro*>(car.get()->GetComponent(CompType::NitroComp).get());
    
    if(cPowerUpCar->typePowerUp != typeCPowerUp::None){

        switch (cPowerUpCar->typePowerUp){
            case typeCPowerUp::RoboJorobo:
                cRoboJorobo->activatePowerUp();
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
                CTransformable *transfor = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());
                d["posCocheSalida"] = transfor;
                // To-Do: actualmente se pasa la posicion del coche desde el que sale, falta calcular con un metodo el cTransformable del coche a perseguir y pasarlo
                // auto cTransCocheSeguir = calcularCocheCercano();
                //d.cTransformable = cTransCocheSeguir
                d["posCochePerseguir"] = transfor; // To-Do: se le mete el coche desde el que sale, deberia ser el que persigue
                eventManager->AddEventMulti(Event{EventType::PowerUp_Create, d});

                break;
        }
        //std::cout << "Lanzamos el power up fiiiuuuuum" << std::endl;
        cPowerUpCar->typePowerUp = typeCPowerUp::None;
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
    // cout << "Han llamado izquierda" << endl;
    //physics->TurnLeft(car.get(), cam);
    int indx = rand() % 6+1;
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
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
