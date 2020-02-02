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

CBoundingOBB::CBoundingOBB(const vector<vec3> &vertices){
    m_compType = CompType::CompBoundingOBB;
    // vamos a crear los planos que serviran para crear el CBoundingOBB
    for(long unsigned int i=0; i<vertices.size(); i=i+4){
        std::cout << " La x del vertice es: " << vertices[i].x << endl;
        planes.push_back( make_shared<CBoundingPlane>(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]) );
    }
    cout << " ---------------------------------------   FIN   ------------------------------------------------------" << endl;
}

// TODO: Actualmente todos los planos se tratan como finitos en los que respecta a las colisiones con las esferas
IntersectData CBoundingOBB::IntersectSphere(const CBoundingSphere &other, const CTransformable &trCar, const CCar &ccarCar){
    uint16_t count = 0;
    for(uint16_t i=0; i<planes.size(); i++){
        IntersectData intersData = planes[i].get()->IntersectSphere(other, trCar, ccarCar);
        if (intersData.intersects) {
            count++;
        }
    }
    if(count > 1){
        std::cout << "DE UN OBB COLISIONAMOS CON MAS DE 1 PLANO BEIBEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
    }
    return IntersectData(false,vec3(0.0,0.0,0.0));
}

IntersectData CBoundingOBB::IntersectRay(const vec3 &posRayOrigin, const vec3 &rayNormalNormalized) const{
    return IntersectData(false,vec3(0.0,0.0,0.0));
}