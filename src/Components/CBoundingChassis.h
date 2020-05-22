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
 
 
 #pragma once

#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"

#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include <vector>
#include "Component.h"
#include <memory>



using namespace glm;
using namespace std;

class CBoundingSphere;
class IntersectData;
struct CCar;
struct CTransformable;
struct CBoundingPlane;
struct CBoundingCilindre;

class CBoundingChassis : public Component {
   public:
    CBoundingChassis(const vec3 &spCB, const vec3 &spCF, const float &spR, const float &spRFloor ,const float spDistaC);

    shared_ptr<CBoundingSphere> sphereBehind;
    shared_ptr<CBoundingSphere> sphereFront;
    shared_ptr<CBoundingCilindre> cilindre;
};