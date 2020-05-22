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
 
 
 #include "CBoundingPlane.h"
#include "CBoundingSphere.h"
#include "CTransformable.h"
#include <cmath>
#include <iostream>
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
    equationPlane = glm::vec4(normal, -glm::dot(a_, normal));  // la ultima es como el valor "d" del plano
}

// TODO: Se va a tener en cuenta el angulo que formen????
IntersectData CBoundingPlane::IntersectSphere(const CBoundingSphere &other){
    float distanceFromSpCenter = fabs(dot(normalizedNormal, other.center) - distance);
    // cout << "Distance from sphere center " << distanceFromSpCenter << endl;
    float distanceFromSphere = distanceFromSpCenter - other.radius;
    bool intersectsInfinitePlane = distanceFromSphere < 0;
    if(intersectsInfinitePlane){
        //cout << "------------------------------------------------------------------------------------------" << endl;
        //vec3 vecDirCar = vec3(trCar.position.x-ccarCar.previousPos.x, trCar.position.y-ccarCar.previousPos.y, trCar.position.z-ccarCar.previousPos.z);
        
        //vec3 vecDirCar = CalculateVecDirCar(trCar);
//
        //double angle = Angle2Vectors(vecDirCar,normal);
        ////cout << " DEBERIA DE SER '122' YYYYY EEEEES:  " << angle << endl;
        //if(angle > 90 /*&& trCar.position != ccarCar.previousPos*/){
            
            vec3 pointM = vec3(0.0,0.0,0.0);
            vec3 pointN = vec3(0.0,0.0,0.0);
            intersectLineSphere(normalize(vec3(a.x-b.x, a.y-b.y, a.z-b.z)), other.center, other.radius, &pointM, &pointN );  // comprobamos en un eje del plano AB
            pointM = IntersectPoint(*(&pointM));
            pointN = IntersectPoint(*(&pointN));
            //cout << " AQUI YA FALLAN DOS BEIBE " << endl;
            if( !membershipPoint(*(&pointM)) && !membershipPoint(*(&pointN)) ){
                //cout << " AQUI YA FALLAN DOS BEIBE " << endl;
                intersectLineSphere(normalize(vec3(a.x-d.x, a.y-d.y, a.z-d.z)), other.center, other.radius, &pointM, &pointN ); // comprobamos el otro eje del plano AD
                pointM = IntersectPoint(*(&pointM));
                pointN = IntersectPoint(*(&pointN));
                if( !membershipPoint(*(&pointM)) && !membershipPoint(*(&pointN)) ){
                    //cout << "LAS 4 ESTAN FUERA" << endl;
                    return IntersectData(false, normalizedNormal * distanceFromSphere);
                }
            }
            /*
            vec3 centerOnPlane = IntersectPoint(*(&other.center));
            if( !membershipPoint(*(&centerOnPlane)) ){
                return IntersectData(false, normalizedNormal * distanceFromSphere);
            }
            */

        //}else{
        //    return IntersectData(false, normalizedNormal * distanceFromSphere);     
        //}

    }
    //std::cout << "estamos dentro del plano beibe, todo normal " << std::endl;
    return IntersectData(intersectsInfinitePlane, normalizedNormal * distanceFromSphere);
}


IntersectData CBoundingPlane::IntersectSphereCenter(const CBoundingSphere &other){
    float distanceFromSpCenter = fabs(dot(normalizedNormal, other.center) - distance);
    // cout << "Distance from sphere center " << distanceFromSpCenter << endl;
    float distanceFromSphere = distanceFromSpCenter - other.radius;
    bool intersectsInfinitePlane = distanceFromSphere < 0;
    if(intersectsInfinitePlane){
        //vec3 centerOnPlane = IntersectPoint(*(&other.center));
        IntersectData pointCollision = IntersectRay(other.center, vec3(0,-1,0));
        if( !pointCollision.intersects ){
            return IntersectData(false, normalizedNormal * distanceFromSphere);
        }
    }
    //std::cout << "estamos dentro del plano beibe, todo normal " << std::endl;
    return IntersectData(intersectsInfinitePlane, normalizedNormal * distanceFromSphere);
}


double CBoundingPlane::Angle2Vectors(const vec3 &a, const vec3 &b) const{
    vec3 aN = glm::normalize(a);
    vec3 bN = glm::normalize(b);

    double dot = glm::dot(aN,bN);
    // Force the dot product of the two input vectors to
    // fall within the domain for inverse cosine, which
    // is -1 <= x <= 1. This will prevent runtime
    // "domain error" math exceptions.
    dot = ( dot < -1.0 ? -1.0 : ( dot > 1.0 ? 1.0 : dot ) );

    double angleRad = acos( dot );
    // grados = radianes*(180/PI_)
    return angleRad*(180/M_PI);  
}



void CBoundingPlane::intersectLineSphere(const vec3 &vecLine,const vec3 &point, const float &radius, vec3 *returnM, vec3 *returnN) const{
    // datos:
    //cout << " recta landa X: " << vecLine.x << " recta landa Y: " << vecLine.y << " recta landa Z: " << vecLine.z << endl;
    //cout << " punto X: " << point.x << " punto Y: " << point.y << " punto Z: " << point.z << endl;
    //cout << "El radio es: " << radius << endl;
    //cout << " ---------------------------------------------------------------------------------------------------------------------------- " << endl;



    // el punto de l RECTA sera el centro de la esfera
    // el vector (landa) dee la recta sera el vecLine 
    // como el punto de la recta sera el centro se sustituyen todos los valores que no son landa
    float landa1 =  0.0;
    float landa2 =  0.0; 

    landa1 =  sqrt( pow(radius,2) / (pow(vecLine.x,2)+pow(vecLine.y,2)+pow(vecLine.z,2)) );  // raiz(3²) = 3 por eso ponemos el radio directamente
    landa2 =  -1*landa1;


    // sustituimos en la ecuacion de la recta1:
    returnM->x = point.x + (vecLine.x * landa1);
    returnM->y = point.y + (vecLine.y * landa1);
    returnM->z = point.z + (vecLine.z * landa1);
    // sustituimos en la ecuacion de la recta2:
    returnN->x = point.x + (vecLine.x * landa2);
    returnN->y = point.y + (vecLine.y * landa2);
    returnN->z = point.z + (vecLine.z * landa2);

}

vec3 CBoundingPlane::IntersectPoint(const vec3 &point) const{
    // calculamos el punto virtual que contendria el centro de la esfera en el plano
    double valorPuntoFinal = (equationPlane.x*point.x)+(equationPlane.y*point.y)+(equationPlane.z*point.z)+equationPlane.w; // -1 -9 + 8 + 1 = -1
    double valorPlanoFinal = (equationPlane.x*equationPlane.x)+(equationPlane.y*equationPlane.y)+(equationPlane.z*equationPlane.z); // 4 +1 + 9 = 14
    double interseccion = (-1*(valorPuntoFinal))/valorPlanoFinal; // el valor final de A` es (1/14)
    double puntoXFinal = point.x + (interseccion*equationPlane.x);
    double puntoYFinal = point.y + (interseccion*equationPlane.y);
    double puntoZFinal = point.z + (interseccion*equationPlane.z);
    //std::cout << "LA SOLUCUON ES:        " << puntoXFinal << " , " << puntoYFinal << " , " << puntoZFinal << std::endl;
    //std::cout << "Y EL CENTRO ES:        " << point.x << " , " << point.y << " , " << point.z << std::endl;

    return vec3(puntoXFinal,puntoYFinal,puntoZFinal);
}

bool CBoundingPlane::membershipPoint(const vec3 &point) const{
    // comprobamos posicion correcta de la X
       // cout.precision(dbl::max_digits10);
       //cout << "EL PUNTO ES X: " << point.x << " LA A.X: " << a.x << " LA C.X: " << c.x << endl;
       //cout << "EL PUNTO ES Y: " << point.y << " LA A.y: " << a.y << " LA C.y: " << c.y << endl;
       //cout << "EL PUNTO ES Z: " << point.z << " LA A.z: " << a.z << " LA C.z: " << c.z << endl;
    if(a.x < c.x){
        if(point.x < a.x || point.x > c.x){ // significa que estamos fuera del rango de la X
            if( (int)a.x == (int)c.x ){
                //cout << " Mi punto esta en: " << point.x << " y deberia estar en: " << a.x << endl;
                if( round(point.x) != round(a.x) ){
                    return false;
                }
            }else
                return false; 
        }
    }else{
        if(point.x > a.x || point.x < c.x){ // significa que estamos fuera del rango de la X
            if( (int)(a.x) == (int)(c.x) ){
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
            if( (int)(a.y) == (int)(c.y) ){
                if(round(point.y) != round(a.y)){
                    return false;
                }
            }else
                return false; 
        }
    }else{
        if(point.y > a.y || point.y < c.y) {// significa que estamos fuera del rango de Y
            if( (int)(a.y) == (int)(c.y) ){
                if( round(point.y) != round(a.y)){
                    return false;
                }
            }else
                return false; 
        }      
    }
    // comprobamos la posicion correcta de Z
    if(a.z < c.z){
        if(point.z < a.z || point.z > c.z){
            //cout.precision(dbl::max_digits10);
            //std::cout << "Point.z: " << point.z << " a.z: " << a.z << " c.z: " << c.z << std::endl;
            if( (int)(a.z) == (int)(c.z) ){
                if( round(point.z) != round(a.z)){
                    return false;
                }
            }else
                return false; 
        }
    }else{
        if(point.z > a.z || point.z < c.z){
            if( (int)(a.z) == (int)(c.z) ){
                if( round(point.z) != round(a.z)){
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
    /*
    double mat00 = -1*a.x; // realmente es X - A.X
    double mat01 = -1*a.y; // realmente es Y - A.Y
    double mat02 = -1*a.z; // realmente es Z - A.Z
    double mat10 = b.x - a.x; 
    double mat11 = b.y - a.y; 
    double mat12 = b.z - a.z; 
    double mat20 = c.x - a.x; 
    double mat21 = c.y - a.y; 
    double mat22 = c.z - a.z; 
    double planeX = (mat11*mat22) - (mat12*mat21);
    double planeY = (mat20*mat12) - (mat10*mat22);
    double planeZ = (mat10*mat21) - (mat11*mat20);
    double planeD = ((mat00*mat11*mat22)+(mat01*mat20*mat12)+(mat02*mat10*mat21)) 
                    - ((mat02*mat11*mat20)+(mat01*mat10*mat22)+(mat00*mat21*mat12));
    //d = -glm::dot(a, normal);
    std::cout << "LA ECUACION GENERAL DEL PLANTO ES: " << planeX << "x " << planeY << "y " << planeZ << "z " << planeD << std::endl;
    return vec4(planeX,planeY,planeZ,planeD);
    */
    auto d_ = -glm::dot(a, normal);
    //std::cout << "LA ECUACION GENERAL DEL PLANTO ES: " << normal.x << "x " <<normal.y << "y " << normal.z << "z " << d_ << std::endl;
    return vec4(normal.x,normal.y,normal.z,d_);
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

IntersectData CBoundingPlane::IntersectRay2(const vec3 &posRayOrigin, const vec3 &rayNormalNormalized) const{


    auto d_ = - dot(normalizedNormal, a);
    float denom = dot(normalizedNormal, rayNormalNormalized);
    // Prevent divide by zero:
    if (abs(denom) <= 1e-4f)
        return IntersectData(false, vec3(0,0,0));
    if (-denom <= 1e-4f)
        return IntersectData(false, vec3(0,0,0));
    float t = -(dot(normalizedNormal, posRayOrigin) + d_) / dot(normalizedNormal, rayNormalNormalized);
    if (t <= 1e-4)
        return IntersectData(false, vec3(0,0,0));


    glm::vec3 puntoEnPlano = posRayOrigin + t * rayNormalNormalized;
    // Comprobamos si ese punto se encuentra dentro del plano definido
    if(!membershipPoint(puntoEnPlano)){
        return IntersectData(false, vec3(0,0,0));
    }

    glm::vec3 vectorDistance(puntoEnPlano-posRayOrigin);
    float distance = sqrt(vectorDistance.x*vectorDistance.x + vectorDistance.y*vectorDistance.y + vectorDistance.z*vectorDistance.z);
    float avoidDistance = 100.0;
    glm::vec3 target(puntoEnPlano + normalizedNormal * avoidDistance);        // target para evitar el muro
        
    return IntersectData(true, target, distance);                            // devolvemos en un vec3 el punto en el que colisiona
}



vec3 CBoundingPlane::CalculateVecDirCar(const CTransformable &cTransformable) const{

   float angleRotation = (cTransformable.rotation.y * M_PI) / 180.0;
   float nextPosX    = cTransformable.position.x - cos(angleRotation) * 1;
   float nexPosZ     = cTransformable.position.z + sin(angleRotation) * 1;

   return vec3(nextPosX-cTransformable.position.x, 0, nexPosZ-cTransformable.position.z);

}