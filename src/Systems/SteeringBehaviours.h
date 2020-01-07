#pragma once

#include <math.h>
#include "../../lib/glm/vec2.hpp"
#include "../../lib/glm/vec3.hpp"
#include <memory>
#include <vector>
#include <map>


using namespace std;

struct ManCar;
struct Entity;
struct CTransformable;
struct CNitro;
struct CCar;
struct ManBoxPowerUp;

class SteeringBehaviours{

public:
    SteeringBehaviours();
    ~SteeringBehaviours(){};

    
    void UpdateSeek(Entity* m_actualCar);
    void UpdateArrive(Entity* m_actualCar);
    float UpdatePursuePowerUp(Entity* m_actualCar, Entity* m_targetCar);
    bool UpdateObstacleAvoidance(Entity* m_Car, ManBoxPowerUp* m_manBoxPowerUp) const;

    glm::vec2 Seek(Entity* m_originCar, const glm::vec3& m_posTargetCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 Arrive(Entity* m_originCar, const glm::vec3& m_posTargetCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 Pursue(Entity* m_originCar, Entity* m_targetCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 PursuePowerUp(Entity* m_originCar, Entity* m_targetCar, const glm::vec2& m_velocityVector) const;
    glm::vec2 ObstacleAvoidance(Entity* m_Car, ManBoxPowerUp* m_manBoxPowerUp, const glm::vec2& m_velocityVector) const;

    void UpdateTransformable(CCar* m_cCar, CTransformable* m_cTransformableCar, CNitro* m_cNitro, float angle) const;
    float CalculateAngle(const glm::vec2& m_originVec, const glm::vec2& m_destinyVec, float m_rotationY) const;
    bool CollisionRaySphere(Entity* m_Car, Entity* m_object, const glm::vec2& m_velocityVector, float& distance, glm::vec2& vectorForceAvoid) const;

protected:
private:
    
};