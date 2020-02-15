#include "CBoundingPlane.h"
#include "CBoundingCilindre.h"
#include "CBoundingSphere.h"
#include "CBoundingOBB.h"
#include <math.h>
#include <cmath>
#include <iostream>


#include <limits>
typedef std::numeric_limits< double > dbl;


using namespace glm;
using namespace std;

// https://www.youtube.com/watch?v=5BJs02PaSok&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=5
// CBoundingPlane CBoundingPlane::Normalize() const {
//     float magnitude = glm::length(normal);
//     return CBoundingPlane(normal / magnitude, distance / magnitude);
// }

CBoundingCilindre::CBoundingCilindre(const vec3 &extreme1_, const vec3 &extreme2_, float radius_)
    : extreme1{extreme1_},
      extreme2{extreme2_},
      radius{radius_}
{
    m_compType = CompType::CompBoundingClindre;
}


IntersectData CBoundingCilindre::IntersectSphere(const CBoundingSphere &other) const{
    // trataremos al cilindro como una recta que contiene infinitas esferas en todo su tramo
    // es por ello que calculamos el punto perpendicular a la recta que pasa por el centro de la esfera para saber si hay colision
    vec3 virtualSphereCenter = CalculateProyectPointOnRecta(other.center);
    // ponemos el radio como "virtual" aunque siemre es el mismo por legibilidad
    float virtualSphereRadius = radius;

    // YA LO TENEMOS TODO PARA COMPROBAR LA COLSION ENTRE ESFERAS Y CILINDROS UEEE
    float radiusDistance = virtualSphereRadius + other.radius; 
    vec3 direction = other.center - virtualSphereCenter; 
    float centerDistance = glm::length(direction); 
    direction /= centerDistance;

    float distance = centerDistance - radiusDistance; 
    bool intersects = distance < 0;

    // FINALMENTE DEBEMOS COMPROBAR SI ESTAMOS DENTRO DEL RADIO DE LOS DOS EXTREMOS
    if(intersects){
       if(EuclideanDis(extreme1,extreme2) > EuclideanDis(extreme1, virtualSphereCenter) && 
            EuclideanDis(extreme1,extreme2) > EuclideanDis(extreme2, virtualSphereCenter)){
                intersects = true;
        }else{
            intersects = false;
        }
    }


    return IntersectData(intersects, distance * direction, virtualSphereCenter, virtualSphereRadius);
}

double CBoundingCilindre::EuclideanDis(const vec3 &p1, const vec3 &p2) const{
    return sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2));
}


vec3 CBoundingCilindre::CalculateProyectPointOnRecta(const vec3 &point_) const{

    // Primero, obtencion de la recta parametrica -- compuesta por: vector director y un punto
    vec3 vecDir = vec3(extreme2.x-extreme1.x, extreme2.y-extreme1.y, extreme2.z-extreme1.z);
    // el punto puede ser cualquiera de los dos "extreme"
    vec3 pointRecta = extreme1;

    // Segundo, buscamos el plano perpendicular a nuestra recta que pasa por "point_"
    // para eso necesitamos el vector normal al plano (el vector director de la recta) y un punto --> point_
    // solo nos queda del plano la D : x + y + z + D = 0
    double pointD = (-1*point_.x*vecDir.x) + (-1*point_.y*vecDir.y) + (-1*point_.z*vecDir.z);

    // Tercero, calcular el punto de corte entre el plano y la recta
    // se nos quedara como incognita landa
    double partNum      = -1*( (pointRecta.x*vecDir.x)+(pointRecta.y*vecDir.y)+(pointRecta.z*vecDir.z)+pointD );
    double partLanda    = (vecDir.x*vecDir.x)+(vecDir.y*vecDir.y)+(vecDir.z*vecDir.z);
    double landa        = partNum/partLanda;

    // Cuarto, finalmente sustituimos nuestra landa en nuestra ecuacion de la recta
    double partX = pointRecta.x + (vecDir.x*landa);
    double partY = pointRecta.y + (vecDir.y*landa);
    double partZ = pointRecta.z + (vecDir.z*landa);

    return vec3(partX, partY, partZ);
}