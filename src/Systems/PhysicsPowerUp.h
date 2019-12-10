#pragma once

#include "../Components/CType.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CCamera.h"
#include "../Components/Component.h"
#include "../Components/CCar.h"
#include "../Components/CWayPoint.h"

#include "../Entities/PowerUp.h"
#include "../Entities/Entity.h"


// solo lo necesitaran algunos como la banana
#include "../Managers/ManWayPoint.h"
#include "../Entities/WayPoint.h"

#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"

// solo algunos como la banana
#include "../fuzzyLogic/fuzzyLogic.h"

#include <vector>
#include <stdlib.h>     /* srand, rand */

#include <math.h>
#include <memory>
using namespace std;
#define PI 3.141592

class PhysicsPowerUp
{
public:
    PhysicsPowerUp();
    ~PhysicsPowerUp(){};

   // void InitPhysicsIA(CarAI* car);
   // void Update(vector<shared_ptr<WayPoint>> wayPoints, CarAI* car, float deltaTime);
   // void fuzzyRules(CarAI* car);
   // void fuzzyRulesVelocity(float maxSpeed, float minSpeed, float accelerationCar);
   // void fuzzyRulesAngle();
   // float calculateFuzzyVelocity(float speedCar, float angle);
   // float calculateFuzzyDirection(float distance, float direction);

protected:
private:
    //uint32_t actualWayPoint = 0;
    //shared_ptr<FuzzyLogic> flVelocity = make_shared<FuzzyLogic>();
    //shared_ptr<FuzzyLogic> flAngle = make_shared<FuzzyLogic>();
};