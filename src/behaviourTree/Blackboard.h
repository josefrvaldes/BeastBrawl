#pragma once

#include <iostream>
#include <memory>
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManWayPoint.h"
#include "../Managers/ManNavMesh.h"
#include "../Systems/SystemFuzzyLogicAI.h"
#include "../Systems/SteeringBehaviours.h"

class Blackboard{
    
    public:
    Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint, ManNavMesh* m_manNavMesh);
    Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint, ManNavMesh* m_manNavMesh, SystemFuzzyLogicAI* m_systemFuzzyLogicAI, SteeringBehaviours* m_steeringBehaviours);
    ~Blackboard() {};

    CarAI* actualCar;
    ManCar* manCars;
    ManPowerUp* manPowerUps;
    ManBoxPowerUp* manBoxPowerUps;
    ManTotem* manTotems;
    ManWayPoint* manWayPoint;
    ManNavMesh* manNavMesh;
    SystemFuzzyLogicAI* systemFuzzyLogicAI;
    SteeringBehaviours* steeringBehaviours;
};

