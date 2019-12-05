#pragma once

#include "../Components/CType.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CLastPosition.h"
#include "../Components/CCamera.h"
#include "../Components/Component.h"
#include "../Components/CCar.h"
#include "../Components/CWayPoint.h"

#include "../Entities/CarAI.h"
#include "../Entities/Entity.h"

#include "../Managers/ManWayPoint.h"
#include "../Entities/WayPoint.h"

#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"

#include "../fuzzyLogic/fuzzyLogic.h"

#include <vector>
#include <stdlib.h>     /* srand, rand */

#include <math.h>
using namespace std;
#define PI 3.141592

class PhysicsAI
{
public:
    PhysicsAI();
    ~PhysicsAI();

    void InitPhysicsIA(CarAI* car);
    void Update(vector<WayPoint *> wayPoints, CarAI* car, float deltaTime);
    void fuzzyRulesVelocity(CarAI* car);
    float calculateFuzzyVelocity(float speedCar, float angle);

protected:
private:
    uint32_t actualWayPoint = 0;
    FuzzyLogic flVelocity;
};