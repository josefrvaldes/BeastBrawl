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

CBoundingOBB::CBoundingOBB(const vector<glm::vec3> &vertices, const vector<glm::vec3> &centersMass_){
    m_compType = CompType::CompBoundingOBB;
    //center = centerMass;
    //std::cout << "EL CENTRO DE MASAS (EN X) DEL BOUNDING OBB ES: " << center.x << endl;
    // vamos a crear los planos que serviran para crear el CBoundingOBB
    for(long unsigned int i=0; i<vertices.size(); i=i+4){
        //std::cout << " La x del vertice es: " << vertices[i].x << endl;
        planes.push_back( make_shared<CBoundingPlane>(vertices[i], vertices[i+1], vertices[i+2], vertices[i+3]) );
    }
    for(long unsigned int i=0; i<centersMass_.size(); i++){
        //std::cout << " La x del vertice es: " << vertices[i].x << endl;
        centersMass.push_back(centersMass_[i]);
    }
    //cout << " ---------------------------------------   FIN   ------------------------------------------------------" << endl;
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
        //cout << "hay interseccion con 1 plano..." << endl;
        return intersectsPlane[0];
    }
    else if(intersectsPlane.size() > 1){
        //cout <<"HAY INTERSECCION CON 2, EN ESPECIFICO CON EL: ";
        //return intersectsPlane[0];

        // necesitamos decidir cual es centro de masas que mas nos conviene, respecto a nuestra posicion
        auto centerNear = centersMass[0];
        float distanceNear = EuclideanDis(other.center, centersMass[0]);
        for(long unsigned int i=1; i<centersMass.size(); i++){
            // nos quedamos con aquel que tenga la distancia mas corta
            if(distanceNear > EuclideanDis(other.center, centersMass[i])){
                centerNear = centersMass[i];
                distanceNear = EuclideanDis(other.center, centersMass[i]);
            }
        }
        //cout << "la DISTANCIA EEEEES: " << distanceNear << endl;
        //cout << "el centro con el que colisionamos se encuentra en: ( " << centerNear.x << " , " << centerNear.y << " , " << centerNear.z << " )" << endl;
        //std::cout << "DE UN OBB COLISIONAMOS CON MAS DE 1 PLANO BEIBEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
        for(long unsigned int i=0; i<intersectsPlane.size(); i++){
            glm::vec3 vecDir = glm::vec3(centerNear.x-other.center.x, centerNear.y-other.center.y, centerNear.z-other.center.z);
            IntersectData intersDataSphereRay = planes[intersectsPlane[i].posEntity]->IntersectRay(other.center , normalize(vecDir));
            // De normal el rayo lanzado solo debe de colisionar con 1 plano, sera por el que realmente entra.
            // de momento lo que haremos es colisionar con ese plano a ver que pasa
            if(intersDataSphereRay.intersects){
                //cout << i << endl;
                return intersectsPlane[i];
            }
        }
        // hay que crear una esfera virtual de centro, centro del OBB y de radio = |dis(centro, puntoCol)|
    }
    return IntersectData(false, -1 ,glm::vec3(0.0,0.0,0.0));
}

IntersectData CBoundingOBB::IntersectRay(const glm::vec3 &posRayOrigin, const glm::vec3 &rayNormalNormalized) const{
    //return IntersectData(false,vec3(0.0,0.0,0.0));
    for(long unsigned int i=0; i<planes.size(); i++){
        IntersectData intersData = planes[i].get()->IntersectRay(posRayOrigin, rayNormalNormalized);
        if (intersData.intersects) {
            return IntersectData(true, i, intersData.direction, intersData.distance);
        }
    }
    // si no intersecta con ninguno no hago nada
    return IntersectData(false, glm::vec3(0.0,0.0,0.0));
}

double CBoundingOBB::EuclideanDis(const glm::vec3 &p1, const glm::vec3 &p2) const{
    return sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2));
}