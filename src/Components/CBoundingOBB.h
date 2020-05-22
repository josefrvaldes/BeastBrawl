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
#include "../CLPhysics/IntersectData.h"

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

class CBoundingOBB : public Component {
   public:
    CBoundingOBB(const vector<glm::vec3> &, const vector<glm::vec3> &);

    // void Normalize();
    IntersectData IntersectSphere(const CBoundingSphere &sphere);

    IntersectData IntersectPlane(const CBoundingPlane &other) const;

    IntersectData IntersectRay(const glm::vec3 &posRayOrigin, const glm::vec3 &rayNormalNormalized) const;
    IntersectData IntersectRay2(const glm::vec3 &posRayOrigin, const glm::vec3 &rayNormalNormalized) const;

    vector<shared_ptr<CBoundingPlane>> GetPlanesOBB(){ return planes; };

    vector<shared_ptr<CBoundingPlane>> planes;
    vector<glm::vec3> centersMass;
    // Recordatorio, un plano tiene 4 vec3 (sus 4 vertices), su normal, su ecuacion general, y una distancia.

    private:
    double EuclideanDis(const glm::vec3 &p1, const glm::vec3 &p2) const;
};