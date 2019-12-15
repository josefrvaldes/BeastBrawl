#pragma once

#include <stdlib.h> /* srand, rand */
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "../../lib/glm/vec3.hpp"
#include "../Aliases.h"
#include "../Components/CTransformable.h"
#include "Manager.h"

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
    shared_ptr<Car> &GetCar() { return car; };

    void CreateCarAI();
    void CreateCarAI(glm::vec3 _position, glm::vec3 _waypoint);
    vector<shared_ptr<CarAI>> GetEntitiesAI() const { return CarAIs; };
    CTransformable *calculateCloserAI();
    void Integrate(float) override;

   private:
    Physics *physics;
    Camera *cam;
    void AccelerateCar(DataMap d);
    void SubscribeToEvents() override;
    void TurnLeftCar(DataMap d);
    void TurnRightCar(DataMap d);
    void NotAcceleratingOrDecelerating(DataMap d);
    void Decelerate(DataMap d);
    void NotTurning(DataMap d);
    void CollisionPowerUp(DataMap d);

    void ThrowPowerUp(DataMap d);
    void CatchPowerUp(DataMap d);
    shared_ptr<Car> car;
    vector<shared_ptr<CarAI>> CarAIs;
};
