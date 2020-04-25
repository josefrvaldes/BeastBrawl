#pragma once

#include <iostream>
#include <memory>
#include <glm/vec3.hpp>

#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <Systems/PhysicsCamera.h>

using namespace std;
using namespace glm;

class Car;

class ManCamera{
   public:
    //using ManNavMesh::CalculateNavMesh;
    ManCamera(Car*, float deltaTime);
    ~ManCamera();

    void Update();
    void CreateCamera();
    void CreateCamera(Car*);
    void FollowPlayer(Car*);


    Camera* getCamera(){ return camera.get(); };

   private:
    unique_ptr<Camera> camera;
    Car* playerFollow;
    unique_ptr<PhysicsCamera> physicsCamera;

    bool followPlayer {false};
    void SubscribeToEvents();
    void TurnLeftCar(DataMap* d);
    void TurnRightCar(DataMap* d);
    void NotTurning(DataMap* d);

};