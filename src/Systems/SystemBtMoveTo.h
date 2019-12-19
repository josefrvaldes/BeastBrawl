#pragma once

#include <iostream>
#include <memory>

using namespace std;

struct selector;
struct CarAI;
struct ManCar;
struct ManPowerUp;
struct ManBoxPowerUp;
struct ManTotem;
struct ManWayPoint;

class SystemBtMoveTo {
   public:
    SystemBtMoveTo();
    ~SystemBtMoveTo(){};

    void init();
    void update(CarAI* actualCar, ManCar* manCars, ManPowerUp* manPowerUps, ManBoxPowerUp* manBoxPowerUps, ManTotem* manTotems, ManWayPoint* manWayPoint);


   protected:
   private:
   shared_ptr<selector> selectorBehaviourTree;

};