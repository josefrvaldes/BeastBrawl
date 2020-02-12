#pragma once

#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>
#include <map>
#include <CLPhysics/CLPhysics.h>

//#include "../../lib/glm/mat4x4.hpp"
//#include "../../lib/glm/gtc/matrix_transform.hpp"
//#include "../../lib/glm/gtc/quaternion.hpp"
//#include "../../lib/glm/gtx/quaternion.hpp"

using namespace std;

struct ManCar;
class Entity;
class CTransformable;
struct CNitro;
class CCar;
struct ManPowerUp;
struct ManBoundingWall;

class SteeringBehaviours{

public:
    SteeringBehaviours();
    ~SteeringBehaviours(){};

    
    void UpdateSeek(Entity* m_actualCar);
    void UpdateArrive(Entity* m_actualCar);
    float UpdatePursuePowerUp(Entity* m_actualCar, Entity* m_targetCar);
    bool UpdateObstacleAvoidance(Entity* m_Car,  ManCar* m_manCar) const;
    bool UpdateWallAvoidance(Entity* m_Car,  ManBoundingWall* m_manBoundingWall) const;

    glm::vec2 Seek(Entity* m_originCar, const glm::vec3& m_posTargetCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 Arrive(Entity* m_originCar, const glm::vec3& m_posTargetCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 Pursue(Entity* m_originCar, Entity* m_targetCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 PursuePowerUp(Entity* m_originCar, Entity* m_targetCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 ObstacleAvoidance(Entity* m_Car, ManCar* m_manCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 WallAvoidance(Entity* m_Car, ManBoundingWall* m_manBoundingWall, const glm::vec2& m_velocityVector) const;

    void UpdateAngleRotation(CCar* m_cCar, float angle) const;
    void UpdateSpeed(CCar* m_cCar, CNitro* m_cNitro) const;
    void UpdateSpeedAvoidance(CCar* m_cCar, CNitro* m_cNitro) const;
    void UpdatePosition(CCar* m_cCar, CTransformable* m_cTransformableCar) const;

    glm::vec2 CalculateVectorVelocity(CCar &m_cCar, CTransformable &transformableCar) const;
    float CalculateAngle(const glm::vec2& m_originVec, const glm::vec2& m_destinyVec, float m_rotationY) const;
    bool CollisionRaySphere(Entity* m_Car, Entity* m_object, const glm::vec2& m_velocityVector, float& distance, glm::vec2& vectorForceAvoid) const;
    bool CollisionRayPlane(Entity* m_Car, Entity* m_object, const glm::vec2& m_velocityVector, float& distance, glm::vec2& vectorForceAvoid, glm::vec3& target) const;

    bool CollisionFaceToFace(Entity* m_Car, Entity *m_object) const;
    void AvoidVibration(Entity* m_Car, Entity *actualObstacle, const glm::vec2& m_velocityVector, glm::vec2& vectorForce) const;
    void AvoidTrapCorner(Entity* m_Car, Entity *actualObstacle, const glm::vec2& m_velocityVector, const glm::vec3& target, glm::vec2& vectorForce) const;

protected:
private:
    unique_ptr<CLPhysics> clPhysics;
};