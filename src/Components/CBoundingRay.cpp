#include "CBoundingRay.h"



CBoundingRay::CBoundingRay() : iteratorSamePlane{0}, previousPlane{nullptr}, target{glm::vec3(0,0,0)}, 
                            iteratorSameSphere{0}, previousSphere{nullptr}, vectorForce{glm::vec2(0,0)} {
    m_compType = CompType::CompBoundingRay;
} 