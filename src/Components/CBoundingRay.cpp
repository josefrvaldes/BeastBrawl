#include "CBoundingRay.h"



CBoundingRay::CBoundingRay() : iteratorSamePlane{0}, iteratorSameSphere{0}, previousPlane{nullptr}, previousSphere{nullptr}, 
                vectorForce{glm::vec2(0,0)}, target{glm::vec3(0,0,0)} {
    m_compType = CompType::CompBoundingRay;
}