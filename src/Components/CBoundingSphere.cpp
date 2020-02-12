#include "CBoundingSphere.h"
#include "CTransformable.h"


#include <math.h>

CBoundingSphere::CBoundingSphere(const vec3 &_center)
    : center{_center}, radius{DEFAULT_SPHERE_RADIUS} {
    m_compType = CompType::CompBoundingSphere;
    //equationSphere = equationSphereCenterRadius(_center, DEFAULT_SPHERE_RADIUS);
    }
CBoundingSphere::CBoundingSphere(const vec3 &_center, float _radius)
    : center{_center}, radius{_radius} {
    m_compType = CompType::CompBoundingSphere;
    //equationSphere = equationSphereCenterRadius(_center, _radius);
}



/*
vec4 CBoundingSphere::equationSphereCenterRadius(const vec3 &center, const float &radius) const{
    return vec4( (-1*pow(center.x,2)) , (-1*pow(center.y,2)) , (-1*pow(center.z,2)) , pow(radius,2) );
}
*/

// https://www.youtube.com/watch?v=1l-k8c7NpQM&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=3
IntersectData CBoundingSphere::IntersectSphere(const CBoundingSphere &other) const {
    float radiusDistance = radius + other.radius;  // esto es la distancia máxima que tiene que haber entre los dos centros si se están tocando
    vec3 direction = other.center - center; // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    //vec3 vecCenterDistance = (other.center - center);  // distancia entre los centros
    float centerDistance = glm::length(direction); // longitud del vector
    direction /= centerDistance; // normalizamos la dirección

    float distance = centerDistance - radiusDistance;  // distancia que están intersectando
    bool intersects = distance < 0;
    return IntersectData(intersects, distance * direction);
}






// https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
IntersectData CBoundingSphere::IntersectRay(const CTransformable &transCarRay, const vec3 &normalRay) const{

    vec3 vecDiferenceM = vec3(transCarRay.position.x , transCarRay.position.y, transCarRay.position.z) - center;
    // Producto escalar
    float b = vecDiferenceM.x * normalRay.x + 0.0 + vecDiferenceM.z * normalRay.z;
    float c = vecDiferenceM.x * vecDiferenceM.x + 0.0 + vecDiferenceM.z * vecDiferenceM.z - (radius+radius+radius) * (radius+radius+radius);

    if (c > 0.0f && b > 0.0f) return IntersectData(false, vec3(0,0,0), -1); 
    float discr = b*b - c; 

    if (discr < 0.0f) return IntersectData(false, vec3(0,0,0), -1); 

    // Ray now found to intersect sphere, compute smallest distance value of intersection distancia desde la que intersecta
    float distance = -b - sqrt(discr); 

    // If t is negative, ray started inside sphere so clamp t to zero 
    if (distance < 0.0f) distance = 0.0f; 
    // punto que punto corta el rayo la esfera
    vec3 q = vec3(transCarRay.position.x , transCarRay.position.y, transCarRay.position.z) + distance * normalRay; 
    //std::cout << "Veo obstaculo: " << t << std::endl;


    // calcular normal
    vec3 vecNormal = q - center;
    float distanceVecNormal = sqrt(vecNormal.x*vecNormal.x + 0 + vecNormal.z*vecNormal.z);
    vec3 vecNormalNormalized = vec3( vecNormal.x*(1/distanceVecNormal) , 0.0 ,vecNormal.z*(1/distanceVecNormal)) ;

    float distanceFromCollision = 40.0;
    vec3 target = q + vec3(vecNormalNormalized.x * distanceFromCollision, 0.0, vecNormalNormalized.z * distanceFromCollision);

    return IntersectData(true, target, distance);
}