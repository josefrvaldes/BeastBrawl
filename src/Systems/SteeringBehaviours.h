#pragma once

#include <math.h>
#include "../../lib/glm/vec2.hpp"
#include "../../lib/glm/vec3.hpp"
#include <memory>
#include <vector>

using namespace std;

struct ManCar;
struct Entity;
struct CTransformable;

class SteeringBehaviours{

public:
    SteeringBehaviours();
    ~SteeringBehaviours(){};


    void Update(ManCar* m_manCar);
    void UpdateTransformable(CTransformable* m_cTransformableCar, float angle);
    glm::vec2 Seek(Entity* m_originCar, Entity* m_destinyCar, glm::vec2 m_velocityVector);
    float CalculateAngle(glm::vec2 m_originVec, glm::vec2 m_destinyVec, float m_rotationY);

protected:
private:
    
};