#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Manager.h"
#include <stdlib.h>     /* srand, rand */
#include "../../lib/glm/vec3.hpp"

using namespace std;
class Car;
class CarAI;
class Data;
class Physics;
class Camera;

class ManCar : public Manager {
   public:
    ManCar();
    ManCar(Physics *, Camera *);
    ~ManCar();

    void CreateCar();
    void CreateMainCar();
    shared_ptr<Car>& GetCar() { return car; };

    void CreateCarAI();
    void CreateCarAI(glm::vec3 _position, glm::vec3 _waypoint);
    vector<shared_ptr<CarAI>> GetEntitiesAI() const { return CarAIs; };

   private:
    Physics *physics;
    Camera *cam;
    void SubscribeToEvents();
    void AccelerateCar(Data d);
    void TurnLeftCar(Data d);
    void TurnRightCar(Data d);
    void NotAcceleratingOrDecelerating(Data d);
    void Decelerate(Data d);
    void NotTurning(Data d);
    void CatchPowerUp(Data d);
    void ThrowPowerUp(Data d);
    shared_ptr<Car> car;
    vector<shared_ptr<CarAI>> CarAIs;
};
