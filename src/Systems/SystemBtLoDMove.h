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
struct ManNavMesh;
struct SystemFuzzyLogicAI;
struct SteeringBehaviours;

class SystemBtLoDMove {
   public:
      SystemBtLoDMove();
      ~SystemBtLoDMove(){};

      void init();
      void update(CarAI* actualCar, ManCar* manCars, ManPowerUp* manPowerUps, ManBoxPowerUp* manBoxPowerUps, ManTotem* manTotems, ManWayPoint* manWayPoint, ManNavMesh* manNavMesh);


   private:
      shared_ptr<selector> selectorBehaviourTree;
      shared_ptr<SystemFuzzyLogicAI> fuzzyLogic;
      shared_ptr<SteeringBehaviours> steeringBehaviours;

      bool entradoFL = false;

};