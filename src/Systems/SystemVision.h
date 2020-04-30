#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include <CLPhysics/CLPhysics.h>

using namespace std;

struct Entity;
struct ManCar;
struct ManBoxPowerUp;
struct ManTotem;
struct ManBoundingOBB;
struct CBrainAI;
struct CTransformable;
struct CCar;
struct RenderFacade;



class SystemVision{
public:
    SystemVision();
    ~SystemVision(){};

    // For AI
    void SaveCarInVision(Entity* actualCar, CBrainAI* cBrainAI, ManCar* manCar, ManBoundingOBB* manBoundingOBB);
    void SaveBoxPowerUpInVision(Entity* actualCar, CBrainAI* cBrainAI, ManBoxPowerUp* manBoxPowerUp, ManBoundingOBB* manBoundingOBB);
    void SaveTotemInVision(Entity* actualCar, CBrainAI* cBrainAI, ManTotem* manTotem, ManBoundingOBB* manBoundingOBB);
    bool EntityInVisionRange(Entity* actualCar, const glm::vec3& posEntity, int rangeVision) const;
    bool IsOcludedOrFar(Entity* actualCar, CBrainAI* cBrainAI, const glm::vec3& posEntity, ManBoundingOBB* manBoundingOBB) const;
    glm::vec3 CalculateVectorVelocity(CCar &cCar, CTransformable &transformableCar) const;

    // For Main Car
    bool EntityInVisionHuman(const glm::vec3& posEntity) const;



private:
    unique_ptr<CLPhysics> clPhysics;
    RenderFacade* renderEngine;
};