#include "Blackboard.h"

Blackboard::Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint, ManNavMesh* m_manNavMesh){
    actualCar = m_actualCar;
    manCars = m_manCars;
    manPowerUps = m_manPowerUps;
    manBoxPowerUps = m_manBoxPowerUps;
    manTotems = m_manTotems;
    manWayPoint = m_manWayPoint;
    manNavMesh = m_manNavMesh;
}


Blackboard::Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint, 
                SystemFuzzyLogicAI* m_systemFuzzyLogicAI, SteeringBehaviours* m_steeringBehaviours, ManNavMesh* m_manNavMesh, ManBoundingWall* m_manBoundingWall){
    actualCar = m_actualCar;
    manCars = m_manCars;
    manPowerUps = m_manPowerUps;
    manBoxPowerUps = m_manBoxPowerUps;
    manTotems = m_manTotems;
    manWayPoint = m_manWayPoint;
    manBoundingWall = m_manBoundingWall;
    manNavMesh = m_manNavMesh;
    systemFuzzyLogicAI = m_systemFuzzyLogicAI;
    steeringBehaviours = m_steeringBehaviours;
}




