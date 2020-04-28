#pragma once

#include "SystemAI.h"
#include <glm/vec3.hpp>
#include <memory>
#include <CLPhysics/CLPhysics.h>


using namespace std;

struct Manager;
struct CarAI;
struct Entity;
struct ManBoxPowerUp;
struct ManTotem;
struct ManBoundingOBB;
struct ManCar;
struct CBrainAI;
struct CTransformable;
struct CCar;



class SystemVision : public SystemAI {
public:
    SystemVision();
    ~SystemVision(){};

    void update(CarAI* actualCar) override;
    void AddManager(Manager &e);
    int getFrecuency(){ return frec; };


    void SaveCarInVision(Entity* actualCar, CBrainAI* cBrainAI, ManCar* manCar, ManBoundingOBB* manBoundingOBB);
    void SaveBoxPowerUpInVision(Entity* actualCar, CBrainAI* cBrainAI, ManBoxPowerUp* manBoxPowerUp, ManBoundingOBB* manBoundingOBB);
    void SaveTotemInVision(Entity* actualCar, CBrainAI* cBrainAI, ManTotem* manTotem, ManBoundingOBB* manBoundingOBB);
    bool EntityInVisionRange(Entity* actualCar, const glm::vec3& posEntity, int rangeVision) const;
    bool IsOcludedOrFar(Entity* actualCar, CBrainAI* cBrainAI, const glm::vec3& posEntity, ManBoundingOBB* manBoundingOBB) const;
    glm::vec3 CalculateVectorVelocity(CCar &cCar, CTransformable &transformableCar) const;
private:

    vector<Manager *> managers;
    int frec {1};

    unique_ptr<CLPhysics> clPhysics;
};