#include "CBoundingChassis.h"
#include <math.h>
#include <iostream>
#include "CBoundingSphere.h"
#include "CBoundingPlane.h"
#include <math.h>
#include <cmath>

#include <limits>
typedef std::numeric_limits< double > dbl;


using namespace glm;
using namespace std;

// https://www.youtube.com/watch?v=5BJs02PaSok&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=5
// CBoundingPlane CBoundingPlane::Normalize() const {
//     float magnitude = glm::length(normal);
//     return CBoundingPlane(normal / magnitude, distance / magnitude);
// }

CBoundingChassis::CBoundingChassis(const vec3 &spCenterBehind, const float &spRadiusBehind, const vec3 &spCenterFront, const float &spRadiusFront, 
                        const vector<vec3> &plRight, const vector<vec3> &plLeft){

    m_compType = CompType::CompBoundingOBB;
    sphereBehind =  make_shared<CBoundingSphere>(spCenterBehind, spRadiusBehind);
    sphereFront =   make_shared<CBoundingSphere>(spCenterFront, spRadiusFront);
    planeRight =    make_shared<CBoundingPlane>(plRight[0], plRight[1], plRight[2], plRight[3]);
    planeLeft =     make_shared<CBoundingPlane>(plLeft[0], plLeft[1], plLeft[2], plLeft[3]);
}
