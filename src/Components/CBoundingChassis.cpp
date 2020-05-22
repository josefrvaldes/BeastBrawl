/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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
