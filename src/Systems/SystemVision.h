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