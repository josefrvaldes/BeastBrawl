#pragma once

#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>
#include <map>
#include <CLPhysics/CLPhysics.h>

using namespace std;

class Entity;
class CTransformable;
class CNitro;
class CCar;
class CExternalForce;
class ManPowerUp;
class ManBoundingWall;
class ManBoundingOBB;
class ManNavMesh;


class SteeringBehaviours{

public:
    SteeringBehaviours();
    ~SteeringBehaviours(){};

    bool IsTargeteable(Entity* actualCar, Entity* targetCar, ManBoundingWall* manBoundingWall) const;
    
    void UpdateSeek(Entity* actualCar);
    void UpdateArrive(Entity* actualCar);
    float UpdatePursuePowerUp(Entity* actualCar, Entity* targetCar);
    bool UpdateCarAvoidance(Entity* actualCar) const;
    bool UpdateWallAvoidance(Entity* actualCar,  ManBoundingWall* manBoundingWall, ManBoundingOBB* manBoundingOBB) const;
    bool UpdateObstacleAvoidance(Entity* actualCar, ManBoundingOBB* manBoundingOBB) const;
    void UpdateThink(Entity* actualCar, ManNavMesh* manNavMesh);

    void SetCLPhysics(CLPhysics* _clPhysics){ clPhysics=_clPhysics; };


private:
    glm::vec2 Seek(Entity* originCar, const glm::vec3& posTargetCar, const glm::vec2& velocityVector) const;
    glm::vec2 Arrive(Entity* originCar, const glm::vec3& posTargetCar, const glm::vec2& velocityVector) const;
    glm::vec2 Pursue(Entity* originCar, Entity* targetCar, const glm::vec2& velocityVector) const;
    glm::vec2 PursuePowerUp(Entity* originCar, Entity* targetCar, const glm::vec2& velocityVector) const;
    glm::vec2 CarAvoidance(Entity* actualCar, const glm::vec2& velocityVector) const;
    glm::vec2 WallAvoidance(Entity* actualCar, ManBoundingWall* manBoundingWall, ManBoundingOBB* manBoundingOBB, const glm::vec2& velocityVector) const;
    glm::vec2 ObstacleAvoidance(Entity* actualCar, ManBoundingOBB* manBoundingOBB, const std::vector<glm::vec2>& vectorRay) const;

    void UpdateAngleRotation(CCar* cCar, float angle) const;
    void UpdateSpeed(CCar* cCar, CNitro* cNitro) const;
    void UpdateSpeedAvoidance(CCar* cCar, CNitro* cNitro) const;
    void UpdatePosition(CCar* cCar, CTransformable* cTransformableCar, CExternalForce* cExternalForce) const;

    glm::vec2 CalculateVectorVelocity(CCar &cCar, CTransformable &transformableCar) const;
    std::vector<glm::vec2> CalculateRayVector(CCar &cCar, CTransformable &transformableCar) const;
    float CalculateAngle(const glm::vec2& originVec, const glm::vec2& destinyVec, float rotationY) const;
    bool CollisionRaySphere(Entity* actualCar, Entity* object, const glm::vec2& velocityVector, float& distance, glm::vec2& vectorForceAvoid) const;
    bool CollisionRayPlane(Entity* actualCar, Entity* object, const glm::vec2& velocityVector, float& distance, glm::vec2& vectorForceAvoid, glm::vec3& target) const;

    bool CollisionFaceToFace(Entity* actualCar, Entity *object) const;
    void AvoidVibration(Entity* actualCar, Entity *actualObstacle, const glm::vec2& velocityVector, glm::vec2& vectorForce) const;
    void AvoidTrapCorner(Entity* actualCar, Entity *actualObstacle, const glm::vec2& velocityVector, const glm::vec3& target, glm::vec2& vectorForce) const;

    glm::vec3 CalculateCenterNavMesh(Entity* actualCar, ManNavMesh* manNavMesh) const;

    CLPhysics* clPhysics = nullptr;
    glm::vec2 ApplyExternalForce(CCar *cCar, CExternalForce *externalForce, const glm::vec2& carForce) const;
};