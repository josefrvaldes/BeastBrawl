#pragma once

#include "../Components/CType.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CCamera.h"
#include "../Components/Component.h"
#include "../Components/CCar.h"
#include "../Components/CWayPoint.h"
#include "../Components/CNitro.h"
#include "../Components/CShield.h"
#include "../Components/CWayPointEdges.h"
#include "../Components/CPath.h"

#include "../Entities/CarAI.h"
#include "../Entities/Entity.h"

#include "../Managers/ManWayPoint.h"
#include "../Entities/WayPoint.h"

#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"

#include "../fuzzyLogic/fuzzyLogic.h"

#include <vector>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <algorithm>
#include <stdlib.h>     /* srand, rand */

#include <math.h>
#include <memory>
using namespace std;
#define PI 3.141592

class PhysicsAI
{
public:
    PhysicsAI();
    ~PhysicsAI(){};

    void InitPhysicsIA(CarAI* car);
    void Update(ManWayPoint* graph, CarAI* car, float deltaTime);
    void fuzzyRules(CarAI* car);
    void fuzzyRulesVelocity(float maxSpeed, float minSpeed, float accelerationCar);
    void fuzzyRulesAngle();
    float calculateFuzzyVelocity(float speedCar, float angle);
    float calculateFuzzyDirection(float distance, float direction);
    stack<int> Dijkstra(ManWayPoint* graph, int start, int end);


protected:
private:

    uint32_t actualWayPoint = 0;
    shared_ptr<FuzzyLogic> flVelocity = make_shared<FuzzyLogic>();
    shared_ptr<FuzzyLogic> flAngle = make_shared<FuzzyLogic>();
};