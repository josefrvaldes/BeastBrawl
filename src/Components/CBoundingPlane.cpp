#include "CBoundingPlane.h"
#include <math.h>
#include <iostream>
#include "CBoundingSphere.h"

using namespace glm;
using namespace std;

// https://www.youtube.com/watch?v=5BJs02PaSok&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=5
// CBoundingPlane CBoundingPlane::Normalize() const {
//     float magnitude = glm::length(normal);
//     return CBoundingPlane(normal / magnitude, distance / magnitude);
// }

CBoundingPlane::CBoundingPlane(const vec3 &a_, const vec3 &b_, const vec3 &c_, const vec3 &d_)
    : a{a_},
      b{b_},
      c{c_},
      d{d_},
      normal{(cross(b - a, c - a))},             // calculamos su normal sin normalizar // para el cálculo de la normal: https://stackoverflow.com/a/1966605/4657738
      normalizedNormal{normalize(normal)},       // calculamos su normal normalizada
      distance{(dot(normalizedNormal, a_))}  // calculamos la distancia del plano a su normal normalizada, la necesitamos
{
    m_compType = CompType::CompBoundingPlane;
}

IntersectData CBoundingPlane::IntersectSphere(const CBoundingSphere &other) const {
    float distanceFromSpCenter = fabs(dot(normalizedNormal, other.center) - distance);
    // cout << "Distance from sphere center " << distanceFromSpCenter << endl;
    float distanceFromSphere = distanceFromSpCenter - other.radius;
    bool intersectsInfinitePlane = distanceFromSphere < 0;
    return IntersectData(intersectsInfinitePlane, normalizedNormal * distanceFromSphere);
}


IntersectData CBoundingPlane::IntersectRay(const vec3 &posRayOrigin, const vec3 &rayNormalNormalized) const{
    float denom = dot(normalizedNormal, rayNormalNormalized);
    //std::cout << "Resultado: " << denom << std::endl;
    if (denom < -0.00001) {                                                              // para evitar que el rayo sea paralelo al plano
        vec3 p0l0 = a - posRayOrigin;
        
        float t = dot(p0l0, normalizedNormal) / denom;
        glm::vec3 puntoEnPlano(posRayOrigin + rayNormalNormalized * t);                  // punto en el que colisiona
        
        if (t>=0) {
            // Comprobamos si ese punto se encuentra dentro del plano definido
            if(d.x < b.x){
                if(puntoEnPlano.x < d.x || puntoEnPlano.x > b.x)
                    return IntersectData(false, vec3(0,0,0));
            }else{
                if(puntoEnPlano.x < b.x || puntoEnPlano.x > d.x)
                    return IntersectData(false, vec3(0,0,0));
            }

            if(d.z < b.z){
                if(puntoEnPlano.z < d.z || puntoEnPlano.z > b.z)
                    return IntersectData(false, vec3(0,0,0));
            }else{
                if(puntoEnPlano.z < b.z || puntoEnPlano.z > d.z)
                    return IntersectData(false, vec3(0,0,0));
            }

            if(d.y < b.y){
                if(puntoEnPlano.y < d.y || puntoEnPlano.y > b.y)
                    return IntersectData(false, vec3(0,0,0));
            }else{
                if(puntoEnPlano.y < b.y || puntoEnPlano.y > d.y)
                    return IntersectData(false, vec3(0,0,0));
            }

            glm::vec3 vectorDistance(puntoEnPlano-posRayOrigin);
            float distance = sqrt(vectorDistance.x*vectorDistance.x + vectorDistance.y*vectorDistance.y + vectorDistance.z*vectorDistance.z);

            float avoidDistance = 100.0;
            glm::vec3 target(puntoEnPlano + normalizedNormal * avoidDistance);          // target para evitar el muro
            
            return IntersectData(true, target, distance);  // devolvemos en un vec3 el punto en el que colisiona
        }
    } 
    //std::cout << "No colisiona" << std::endl;
    return IntersectData(false, vec3(0,0,0)); 



}