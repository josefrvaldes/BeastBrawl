#include "CBoundingOBB.h"
#include <math.h>
#include <iostream>
#include "CBoundingSphere.h"
#include "CBoundingPlane.h"
#include <math.h>
#include <cmath>

#include <limits>
typedef std::numeric_limits< double > dbl;


using namespace glm;
using namespace std;

// https://www.youtube.com/watch?v=5BJs02PaSok&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=5
// CBoundingPlane CBoundingPlane::Normalize() const {
//     float magnitude = glm::length(normal);
//     return CBoundingPlane(normal / magnitude, distance / magnitude);
// }

CBoundingOBB::CBoundingOBB(const vector<vec3> &vertices, const vec3 &centerMass){
    m_compType = CompType::CompBoundingOBB;
    center = centerMass;
    //std::cout << "EL CENTRO DE MASAS (EN X) DEL BOUNDING OBB ES: " << center.x << endl;
    // vamos a crear los planos que serviran para crear el CBoundingOBB
    for(long unsigned int i=0; i<vertices.size(); i=i+4){
        //std::cout << " La x del vertice es: " << vertices[i].x << endl;
        planes.push_back( make_shared<CBoundingPlane>(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]) );
    }
    cout << " ---------------------------------------   FIN   ------------------------------------------------------" << endl;
}

// TODO: Actualmente todos los planos se tratan como finitos en los que respecta a las colisiones con las esferas
// TODO: el "vector<IntersectData>" va a producir alguna perdida o que ??????????? linea 36
IntersectData CBoundingOBB::IntersectSphere(const CBoundingSphere &other, const CTransformable &trCar, const CCar &ccarCar){
    vector<IntersectData> intersectsPlane;  // nos guardamos los datos d ela colision
    intersectsPlane.shrink_to_fit();
    for(long unsigned int i=0; i<planes.size(); i++){
        IntersectData intersData = planes[i].get()->IntersectSphere(other, trCar, ccarCar);
        if (intersData.intersects) {
            intersectsPlane.push_back(IntersectData(true, i, intersData.direction));
        }
    }
    if(intersectsPlane.size() == 1 ){
        return intersectsPlane[0];
    }
    else if(intersectsPlane.size() > 1){
        //std::cout << "DE UN OBB COLISIONAMOS CON MAS DE 1 PLANO BEIBEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
        for(long unsigned int i=0; i<intersectsPlane.size(); i++){
            vec3 vecDir = vec3(center.x-other.center.x, center.y-other.center.y, center.z-other.center.z);
            IntersectData intersDataSphereRay = planes[intersectsPlane[i].posEntity]->IntersectRay(other.center ,normalize(vecDir));
            // De normal el rayo lanzado solo debe de colisionar con 1 plano, sera por el que realmente entra.
            // de momento lo que haremos es colisionar con ese plano a ver que pasa
            if(intersDataSphereRay.intersects){
                return intersectsPlane[i];
            }
        }
        // hay que crear una esfera virtual de centro, centro del OBB y de radio = |dis(centro, puntoCol)|
    }
    return IntersectData(false, -1 ,vec3(0.0,0.0,0.0));
}

IntersectData CBoundingOBB::IntersectRay(const vec3 &posRayOrigin, const vec3 &rayNormalNormalized) const{
    //return IntersectData(false,vec3(0.0,0.0,0.0));
    for(long unsigned int i=0; i<planes.size(); i++){
        IntersectData intersData = planes[i].get()->IntersectRay(posRayOrigin, rayNormalNormalized);
        if (intersData.intersects) {
            return IntersectData(true, i, intersData.direction, intersData.distance);
        }
    }
    // si no intersecta con ninguno no hago nada
    return IntersectData(false, vec3(0.0,0.0,0.0));
}