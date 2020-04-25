#include "ManCamera.h"
#include "Manager.h"

#include <Entities/Car.h>
#include <Entities/Camera.h>
#include <Components/CCar.h>

ManCamera::ManCamera(Car* entityFollow, float deltaTime_){
    // constructor de la camara
    SubscribeToEvents();

    physicsCamera = make_unique<PhysicsCamera>(deltaTime_);
    CreateCamera(entityFollow);
}

ManCamera::~ManCamera(){
    cout << "llamamos al destructor de ManCamera \n"; 
}


void ManCamera::CreateCamera(){
    camera = make_unique<Camera>(glm::vec3(100.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}

void ManCamera::CreateCamera(Car* car){

    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto vec3Param = glm::vec3(1.0f, 1.0f, 1.0f);
    camera = make_unique<Camera>(vec3(-20.0, 1.0, -400.0), vec3Param, vec3Param, cCar->maxSpeed, cCar->acceleration, cCar->reverseMaxSpeed, cCar->slowDown);
    FollowPlayer(car);
}

void ManCamera::FollowPlayer(Car* car){
    this->playerFollow = car;
    if(!followPlayer){
        followPlayer = true;
        SubscribeToEvents();
    }
}

void ManCamera::Update(){
    physicsCamera->update( playerFollow, camera.get());
}

void ManCamera::SubscribeToEvents() {

    if(followPlayer){
        EventManager::GetInstance().SubscribeMulti(Listener(
            EventType::PRESS_A,
            bind(&ManCamera::TurnLeftCar, this, placeholders::_1),
            "TurnLeftCar"));

        EventManager::GetInstance().SubscribeMulti(Listener(
            EventType::PRESS_D,
            bind(&ManCamera::TurnRightCar, this, placeholders::_1),
            "TurnRightCar"));

        EventManager::GetInstance().SubscribeMulti(Listener(
            EventType::NO_A_D_PRESS,
            bind(&ManCamera::NotTurning, this, placeholders::_1),
            "NotTurning"));
    }
}

void ManCamera::TurnLeftCar(DataMap* d) {
    // cout << "Han llamado izquierda" << endl;
    physicsCamera->TurnLeft(playerFollow, camera.get());
}
void ManCamera::TurnRightCar(DataMap* d) {
    // cout << "Han llamado derecha" << endl;
    physicsCamera->TurnRight(playerFollow, camera.get());
}

void ManCamera::NotTurning(DataMap* d) {
    // cout << "Han llamado not turning" << endl;
    physicsCamera->NotTurning(playerFollow, camera.get());
}
