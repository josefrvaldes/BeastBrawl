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
CBoundingChassis::CBoundingChassis(const vec3 &spCenterBehind, const float &spRadiusBehind, const float &spRadiusBehindFloor, const vec3 &spCenterFront, const float &spRadiusFront, const float &spRadiusFrontFloor){
    m_compType      =   CompType::CompBoundingChassis;
    sphereBehind    =   make_shared<CBoundingSphere>(spCenterBehind, spRadiusBehind, spRadiusBehindFloor);
    sphereFront     =   make_shared<CBoundingSphere>(spCenterFront, spRadiusFront, spRadiusFrontFloor);
    cilindre        =   make_shared<CBoundingCilindre>(spCenterBehind, spCenterFront, spRadiusFront);
}
