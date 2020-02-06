#include "CBoundingChassis.h"
#include <math.h>
#include <iostream>
#include "CBoundingSphere.h"
#include "CBoundingPlane.h"
#include "CBoundingCilindre.h"
#include <math.h>
#include <cmath>

#include <limits>
typedef std::numeric_limits< double > dbl;


using namespace glm;
using namespace std;

// RECORDAR: LOS RADIOS DE LAS ESFERAS IGUALES
CBoundingChassis::CBoundingChassis(const vec3 &spCenterBehind, const float &spRadiusBehind, const vec3 &spCenterFront, const float &spRadiusFront, 
                        const vector<vec3> &plRight, const vector<vec3> &plLeft){

    m_compType      =   CompType::CompBoundingOBB;
    sphereBehind    =   make_unique<CBoundingSphere>(spCenterBehind, spRadiusBehind);
    sphereFront     =   make_unique<CBoundingSphere>(spCenterFront, spRadiusFront);
    cilindre        =   make_unique<CBoundingCilindre>(spCenterBehind, spCenterFront, spRadiusFront);
}
