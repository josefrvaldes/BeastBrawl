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

#include <glm/vec3.hpp>
#include <memory>
#include <CLPhysics/CLPhysics.h>
#include "../Entities/Car.h"


using namespace std;

struct Entity;
struct ManCar;
struct ManBoxPowerUp;
struct ManTotem;
struct ManBoundingOBB;
struct ManBoundingGround;
struct ManBoundingWall;
struct CBrainAI;
struct CTransformable;
struct CCar;
struct RenderFacade;



class SystemVision{
public:
    SystemVision(CLPhysics *);
    ~SystemVision(){};

    // For AI
    void SaveCarInVision(Entity* actualCar, CBrainAI* cBrainAI, ManCar* manCar, ManBoundingWall* manBoundingWall, 
                            ManBoundingOBB* manBoundingOBB, ManBoundingGround* manBoundingGround);

    void SaveBoxPowerUpInVision(Entity* actualCar, CBrainAI* cBrainAI, ManBoxPowerUp* manBoxPowerUp, ManBoundingWall* manBoundingWall, 
                            ManBoundingOBB* manBoundingOBB, ManBoundingGround* manBoundingGround);

    void SaveTotemInVision(Entity* actualCar, CBrainAI* cBrainAI, ManTotem* manTotem, ManBoundingWall* manBoundingWall, 
                            ManBoundingOBB* manBoundingOBB, ManBoundingGround* manBoundingGround);

    bool EntityInVisionRange(Entity* actualCar, const glm::vec3& posEntity, int visionRange) const;

    // For Main Car
    bool EntityInVisionHuman(const glm::vec3& posEntity) const;

    // For telebanana
    Car* CalculateCentredCar(Entity* actualCar, int visionRange, ManCar* manCar, 
        ManBoundingWall* manBoundingWall, ManBoundingOBB* manBoundingOBB, ManBoundingGround* manBoundingGround) const;
    float DegreesFromCenter(Entity* actualCar, Entity* targetCar) const;



private:
    bool IsFar(Entity* actualCar, CBrainAI* cBrainAI, const glm::vec3& posEntity) const;
    bool CollideWithWall(Entity* actualCar, const glm::vec3& posTarget, ManBoundingWall* manBoundingWall) const;
    bool CollideWithOBB(Entity* actualCar, const glm::vec3& posTarget, ManBoundingOBB* manBoundingOBB) const;
    bool CollideWithGround(Entity* actualCar, const glm::vec3& posTarget, ManBoundingGround* manBoundingGround) const;

    CLPhysics *clPhysics;
    RenderFacade* renderEngine;
};