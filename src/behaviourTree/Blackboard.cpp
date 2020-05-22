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
                            SteeringBehaviours* m_steeringBehaviours, ManNavMesh* m_manNavMesh, ManBoundingWall* m_manBoundingWall, ManBoundingOBB* m_manBoundingOBB){
    actualCar = m_actualCar;
    manCars = m_manCars;
    manPowerUps = m_manPowerUps;
    manBoxPowerUps = m_manBoxPowerUps;
    manTotems = m_manTotems;
    manWayPoint = m_manWayPoint;
    manBoundingWall = m_manBoundingWall;
    manNavMesh = m_manNavMesh;
    manBoundingOBB = m_manBoundingOBB;
    //systemFuzzyLogicAI = m_systemFuzzyLogicAI;
    steeringBehaviours = m_steeringBehaviours;
}




