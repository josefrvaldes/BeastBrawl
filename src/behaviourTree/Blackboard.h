/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once

#include <iostream>
#include <memory>

#include <Entities/Car.h>
#include <Entities/CarHuman.h>
#include <Entities/CarAI.h>
#include <Managers/ManCar.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManBoxPowerUp.h>
#include <Managers/ManTotem.h>
#include <Managers/ManWayPoint.h>
#include <Managers/ManBoundingWall.h>
#include <Managers/ManBoundingOBB.h>
#include <Managers/ManNavMesh.h>
//#include <Systems/SystemFuzzyLogicAI.h>
#include <Systems/SteeringBehaviours.h>

class Blackboard{
    
    public:
    Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint, ManNavMesh* m_manNavMesh);
    Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint,
                    SteeringBehaviours* m_steeringBehaviours, ManNavMesh* m_manNavMesh, ManBoundingWall* m_manBoundingWall, ManBoundingOBB* m_manBoundingOBB);
    ~Blackboard() {};

    CarAI* actualCar = nullptr;
    ManCar* manCars = nullptr;
    ManPowerUp* manPowerUps = nullptr;
    ManBoxPowerUp* manBoxPowerUps = nullptr;
    ManTotem* manTotems = nullptr;
    ManWayPoint* manWayPoint = nullptr;
    ManNavMesh* manNavMesh = nullptr;
    ManBoundingWall* manBoundingWall = nullptr;
    ManBoundingOBB* manBoundingOBB = nullptr;
    //SystemFuzzyLogicAI* systemFuzzyLogicAI = nullptr;
    SteeringBehaviours* steeringBehaviours = nullptr;
};

