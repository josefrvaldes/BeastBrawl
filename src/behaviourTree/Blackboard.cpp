#include "Blackboard.h"

Blackboard::Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint){
    actualCar = m_actualCar;
    manCars = m_manCars;
    manPowerUps = m_manPowerUps;
    manBoxPowerUps = m_manBoxPowerUps;
    manTotems = m_manTotems;
    manWayPoint = m_manWayPoint;
}


Blackboard::Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint, SystemFuzzyLogicAI* m_systemFuzzyLogicAI, SteeringBehaviours* m_steeringBehaviours){
    actualCar = m_actualCar;
    manCars = m_manCars;
    manPowerUps = m_manPowerUps;
    manBoxPowerUps = m_manBoxPowerUps;
    manTotems = m_manTotems;
    manWayPoint = m_manWayPoint;
    systemFuzzyLogicAI = m_systemFuzzyLogicAI;
    steeringBehaviours = m_steeringBehaviours;
}




