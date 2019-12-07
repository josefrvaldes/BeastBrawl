#include <iostream>
#include <functional>
#include "ManCar.h"
#include "../Entities/Car.h"
#include "../Entities/Camera.h"
#include "../Systems/Physics.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

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
    cout << "Llamando al destructor de manpowerup" << endl;
}

void ManCar::CreateMainCar() {
    car = make_shared<Car>();
}

void ManCar::CreateCar() {
    shared_ptr<Car> p = make_shared<Car>();
    entities.push_back(p);
}

void ManCar::SubscribeToEvents() {
    Listener listenerAcelerarCoche = Listener(
        EventType::PRESS_I, 
        bind(&ManCar::AcelerarCoche, this, placeholders::_1),
        "EjecutarMeHanCogido");
    EventManager::GetInstance()->SuscribeMulti(listenerAcelerarCoche);
}

void ManCar::AcelerarCoche(Data d) {
    // cout << "Se ha llamado al evento acelerar coche" << endl;
    // cout << "Tenemos en AcelerarCoche un physics con dir de memoria " << physics << endl;

    // auto components = car.get()->GetComponents();
    // auto cTransformable = static_cast<CTransformable *>(components[CompType::TransformableComp].get());

    // auto componentsCam = cam->GetComponents();
    // auto cTransformableCam = static_cast<CTransformable *>(componentsCam[CompType::TransformableComp].get());
    
    // cout << "La pos del coche antes es " << cTransformable->position.x << "," << cTransformable->position.y << "," <<  cTransformable->position.z << endl;
    // cout << "La pos de la cámara antes es " << cTransformableCam->position.x << "," << cTransformableCam->position.y << "," <<  cTransformableCam->position.z << endl;
    physics->Accelerate(car.get(), cam);
    // cout << "La pos del coche después es " << cTransformable->position.x << "," << cTransformable->position.y << "," <<  cTransformable->position.z << endl;
    // cout << "La pos de la cámara después es " << cTransformableCam->position.x << "," << cTransformableCam->position.y << "," <<  cTransformableCam->position.z << endl;
}

//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);
