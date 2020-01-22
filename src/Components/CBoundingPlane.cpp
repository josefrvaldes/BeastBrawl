#include "CBoundingPlane.h"
#include <math.h>
#include <iostream>
#include "CBoundingSphere.h"
#include <math.h>

#include <limits>
typedef std::numeric_limits< double > dbl;


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
      distance{(dot(normalizedNormal, a_))}      // calculamos la distancia del plano a su normal normalizada, la necesitamos 
{
    m_compType = CompType::CompBoundingPlane;
    equationPlane = equationPlane3Points(a,b,c); 
}

IntersectData CBoundingPlane::IntersectSphere(const CBoundingSphere &other){
    float distanceFromSpCenter = fabs(dot(normalizedNormal, other.center) - distance);
    // cout << "Distance from sphere center " << distanceFromSpCenter << endl;
    float distanceFromSphere = distanceFromSpCenter - other.radius;
    bool intersectsInfinitePlane = distanceFromSphere < 0;
    if(intersectsInfinitePlane){
        vec3 centerOnPlane = IntersectPoint(*(&other.center));
        if( !membershipPoint(*(&centerOnPlane)) ){
            // No estamos dentro del plano finito por lo que devolvemos false
            //std::cout << "DIIIIIIIIIIOOOOOOOOOOOOOOOSSSSS DIOOOOOOOOOOOOOSSSSSSSSSSSSSS DIOSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;
            //std::cout << "DIIIIIIIIIIOOOOOOOOOOOOOOOSSSSS DIOOOOOOOOOOOOOSSSSSSSSSSSSSS DIOSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;
            //std::cout << "DIIIIIIIIIIOOOOOOOOOOOOOOOSSSSS DIOOOOOOOOOOOOOSSSSSSSSSSSSSS DIOSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;
            //std::cout << "DIIIIIIIIIIOOOOOOOOOOOOOOOSSSSS DIOOOOOOOOOOOOOSSSSSSSSSSSSSS DIOSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;
            //std::cout << "DIIIIIIIIIIOOOOOOOOOOOOOOOSSSSS DIOOOOOOOOOOOOOSSSSSSSSSSSSSS DIOSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << std::endl;
            return IntersectData(false, normalizedNormal * distanceFromSphere);
        }
    }
    //std::cout << "estamos dentro del plano beibe, todo normal " << std::endl;
    return IntersectData(intersectsInfinitePlane, normalizedNormal * distanceFromSphere);
}

vec3 CBoundingPlane::IntersectPoint(const vec3 &point) const{
    // calculamos el punto virtual que contendria el centro de la esfera en el plano
    float valorPuntoFinal = (equationPlane.x*point.x)+(equationPlane.y*point.y)+(equationPlane.z*point.z)+equationPlane.w; // -1 -9 + 8 + 1 = -1
    float valorPlanoFinal = (equationPlane.x*equationPlane.x)+(equationPlane.y*equationPlane.y)+(equationPlane.z*equationPlane.z); // 4 +1 + 9 = 14
    float interseccion = (-1*(valorPuntoFinal))/valorPlanoFinal; // el valor final de A` es (1/14)
    float puntoXFinal = point.x + (interseccion*equationPlane.x);
    float puntoYFinal = point.y + (interseccion*equationPlane.y);
    float puntoZFinal = point.z + (interseccion*equationPlane.z);
    //std::cout << "LA SOLUCUON ES:        " << puntoXFinal << " , " << puntoYFinal << " , " << puntoZFinal << std::endl;
    //std::cout << "Y EL CENTRO ES:        " << point.x << " , " << point.y << " , " << point.z << std::endl;

    return vec3(puntoXFinal,puntoYFinal,puntoZFinal);
}

bool CBoundingPlane::membershipPoint(const vec3 &point) const{
    // comprobamos posicion correcta de la X
    if(a.x < c.x){
        if(point.x < a.x || point.x > c.x){ // significa que estamos fuera del rango de la X
            if(a.x == c.x){
                if( round(point.x) != round(a.x) )
                    return false;
            }else
                return false; 
        }
    }else{
        if(point.x > a.x || point.x < c.x){ // significa que estamos fuera del rango de la X
            if(a.x == c.x){
                //cout.precision(dbl::max_digits10);
                if( round(point.x) != round(a.x) ){
                    return false;
                }
            }else
                return false; 
        }
    }
    // comprobamos la posicion correcta de la Y
    if(a.y < c.y){
        if(point.y < a.y || point.y > c.y){ // significa que estamos fuera del rango de Y
            if(a.y == c.y){
                if( round(point.y) != round(a.y) )
                    return false;
            }else
                return false; 
        }
    }else{
        if(point.y > a.y || point.y < c.y) {// significa que estamos fuera del rango de Y
            if(a.y == c.y){
                if( round(point.y) != round(a.y) )
                    return false;
            }else
                return false; 
        }      
    }
    // comprobamos la posicion correcta de Z
    if(a.z < c.z){
        if(point.z < a.z || point.z > c.z){
            if(a.z == c.z){
                if( round(point.z) != round(a.z) )
                    return false;
            }else
                return false; 
        }
    }else{
        if(point.z > a.z || point.z < c.z){
            if(a.z == c.z){
                if( round(point.z) != round(a.z) ){
                    //cout.precision(dbl::max_digits10);
                    return false;
                }
            }else
                return false; 
        }
    }
    // en cualquier otro caso el punto se encuentra dentro del plano
    return true;
}

vec4 CBoundingPlane::equationPlane3Points(const vec3 &a, const vec3 &b, const vec3 &c) const{

    float mat00 = -1*a.x; // realmente es X - A.X
    float mat01 = -1*a.y; // realmente es Y - A.Y
    float mat02 = -1*a.z; // realmente es Z - A.Z

    float mat10 = b.x - a.x; 
    float mat11 = b.y - a.y; 
    float mat12 = b.z - a.z; 

    float mat20 = c.x - a.x; 
    float mat21 = c.y - a.y; 
    float mat22 = c.z - a.z; 

    float planeX = (mat11*mat22) - (mat12*mat21);
    float planeY = (mat20*mat12) - (mat10*mat22);
    float planeZ = (mat10*mat21) - (mat11*mat20);
    float planeD = ((mat00*mat11*mat22)+(mat01*mat20*mat12)+(mat02*mat10*mat21)) 
                    - ((mat02*mat11*mat20)+(mat01*mat10*mat22)+(mat00*mat21*mat12));

    std::cout << "LA ECUACION GENERAL DEL PLANTO ES: " << planeX << "x " << planeY << "y " << planeZ << "z " << planeD << std::endl;

    return vec4(planeX,planeY,planeZ,planeD);
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
            glm::vec3 target(puntoEnPlano + normalizedNormal * avoidDistance);        // target para evitar el muro
            
            return IntersectData(true, target, distance);                            // devolvemos en un vec3 el punto en el que colisiona
        }
    } 
    //std::cout << "No colisiona" << std::endl;
    return IntersectData(false, vec3(0,0,0)); 
}