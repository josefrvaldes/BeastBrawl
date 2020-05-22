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
CBoundingChassis::CBoundingChassis(const vec3 &spCenterBehind,const vec3 &spCenterFront, const float &spRadius,
                                        const float &spRadiusFloor, const float spDistaC){
    m_compType      =   CompType::CompBoundingChassis;
    sphereBehind    =   make_shared<CBoundingSphere>(spCenterBehind, spRadius, spRadiusFloor, spDistaC);
    sphereFront     =   make_shared<CBoundingSphere>(spCenterFront, spRadius, spRadiusFloor, spDistaC);
    cilindre        =   make_shared<CBoundingCilindre>(spCenterBehind, spCenterFront, spRadius);
}
