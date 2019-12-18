#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Manager.h"
#include <stdlib.h>     /* srand, rand */
#include "../../lib/glm/vec3.hpp"
#include "../Aliases.h"
#include "../Components/CWayPoint.h"
#include "../Components/CTransformable.h"
#include "../Components/CSpeed.h"

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
    void UpdateCar();
    void UpdateCarAI();
    shared_ptr<Car>& GetCar() { return car; };

    void CreateCarAI();
    void CreateCarAI(glm::vec3 _position, CWayPoint* _waypoint);
    vector<shared_ptr<CarAI>> GetEntitiesAI() const { return CarAIs; };
    CTransformable* calculateCloserAI();

   private:
    Physics *physics;
    Camera *cam;
    void AccelerateCar(DataMap d);
    void SubscribeToEvents();
    void TurnLeftCar(DataMap d);
    void TurnRightCar(DataMap d);
    void NotAcceleratingOrDecelerating(DataMap d);
    void Decelerate(DataMap d);
    void NotTurning(DataMap d);
    void CollisionPowerUp(DataMap d);
    void CollisionPowerUpAI(DataMap d);
    void CatchTotemPlayer(DataMap d);
    void CatchTotemAI(DataMap d);
    void ThrowTotem(Car* carLoseTotem);
    void CatchTotem(Car* carWinTotem);
    bool useRoboJorobo(Car* newCarWithTotem);

    
    void ThrowPowerUp(DataMap d);
    void CatchPowerUp(DataMap d);
    void CatchPowerUpAI(DataMap d);
    shared_ptr<Car> car;
    vector<shared_ptr<CarAI>> CarAIs;
};
