#include "SteeringBehaviours.h"

#include "../Managers/ManCar.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Entities/Entity.h"
#include "../Entities/BoxPowerUp.h"
#include "../Components/CTransformable.h"
#include "../Components/CNitro.h"
#include "../Components/CPosDestination.h"
#include "../Components/CCar.h"


#define PI 3.141592

SteeringBehaviours::SteeringBehaviours(){
    
}



void SteeringBehaviours::UpdateSeek(Entity* m_actualCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(m_actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(m_actualCar->GetComponent(CompType::CarComp).get());
    if(cCar->speed==0) cCar->speed=0.1;
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;
    glm::vec2 vectorVelocity = glm::vec2(posXSiguiente - cTransformable->position.x , posZSiguiente - cTransformable->position.z );

    //Seek
    auto cPosDestination = static_cast<CPosDestination*>(m_actualCar->GetComponent(CompType::PosDestination).get());
    glm::vec3 posTarget = glm::vec3(cPosDestination->position.x, cPosDestination->position.y, cPosDestination->position.z);
    glm::vec2 vectorForce = Seek(m_actualCar, posTarget, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateTransformable(cCar, cTransformable, cNitro, angle);
}



void SteeringBehaviours::UpdateArrive(Entity* m_actualCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(m_actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(m_actualCar->GetComponent(CompType::CarComp).get());
    if(cCar->speed==0) cCar->speed=0.1;
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;
    glm::vec2 vectorVelocity = glm::vec2(posXSiguiente - cTransformable->position.x , posZSiguiente - cTransformable->position.z );

    //Seek
    auto cPosDestination = static_cast<CPosDestination*>(m_actualCar->GetComponent(CompType::PosDestination).get());
    glm::vec3 posTarget = glm::vec3(cPosDestination->position.x, cPosDestination->position.y, cPosDestination->position.z);
    glm::vec2 vectorForce = Arrive(m_actualCar, posTarget, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateTransformable(cCar, cTransformable, cNitro, angle);
}



// devuelve el angulo que le falta para girar para llegar al punto optimo
float SteeringBehaviours::UpdatePursuePowerUp(Entity* m_actualCar, Entity* m_targetCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(m_actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(m_actualCar->GetComponent(CompType::CarComp).get());
    if(cCar->speed==0) cCar->speed=0.1;
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;
    glm::vec2 vectorVelocity = glm::vec2(posXSiguiente - cTransformable->position.x , posZSiguiente - cTransformable->position.z );

    // Pursue
    glm::vec2 vectorForce = PursuePowerUp(m_actualCar, m_targetCar, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateTransformable(cCar, cTransformable, cNitro, angle);

    return angle;
}




bool SteeringBehaviours::UpdateObstacleAvoidance(Entity* m_Car, ManBoxPowerUp* m_manBoxPowerUp) const{
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_Car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(m_Car->GetComponent(CompType::NitroComp).get());
    if(cCar->speed==0) cCar->speed=0.1;
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;
    glm::vec2 vectorVelocity = glm::vec2(posXSiguiente - cTransformable->position.x , posZSiguiente - cTransformable->position.z );

    // Obstacle avoidance
    //auto cTransformable2 = static_cast<CTransformable*>(m_manCar->GetCar().get()->GetComponent(CompType::TransformableComp).get());
    //auto cCar2 = static_cast<CCar*>(m_manCar->GetCar().get()->GetComponent(CompType::CarComp).get());
    //if(cCar2->speed==0) cCar2->speed=0.1;
    //float angleRotation2 = (cTransformable2->rotation.y * PI) / 180.0;
    //float posXSiguiente2 = cTransformable2->position.x - cos(angleRotation2) * cCar2->speed;
    //float posZSiguiente2 = cTransformable2->position.z + sin(angleRotation2) * cCar2->speed;
    //glm::vec2 vectorVelocity2 = glm::vec2(posXSiguiente2 - cTransformable2->position.x , posZSiguiente2 - cTransformable2->position.z );
    glm::vec2 vectorForceAvoid = ObstacleAvoidance(m_Car, m_manBoxPowerUp, vectorVelocity);

    if(vectorForceAvoid.x != 0.0 || vectorForceAvoid.y != 0.0 ){
        std::cout << "Se viene choque" << std::endl;
        float angle = CalculateAngle(vectorVelocity, vectorForceAvoid, cTransformable->rotation.y);
        UpdateTransformable(cCar, cTransformable, cNitro, angle);
        return true;
    }
    return false;
}


void SteeringBehaviours::UpdateTransformable(CCar* m_cCar, CTransformable* m_cTransformableCar, CNitro* m_cNitro, float angle) const{
    // To-Do: Modificar estos angulos
    if(angle > m_cCar->maxWheelRotation*0.20) 
        m_cCar->wheelRotation = m_cCar->maxWheelRotation;
    else if(angle < -m_cCar->maxWheelRotation*0.20)
        m_cCar->wheelRotation = -m_cCar->maxWheelRotation;
    else if(angle>0.1)
        m_cCar->wheelRotation = angle/0.40;
    else if(angle<-0.1)
        m_cCar->wheelRotation = angle/0.40;
    else
        m_cCar->wheelRotation = 0;
    

    if(m_cNitro->activePowerUp == false){
        m_cCar->speed += m_cCar->acceleration;
        if (m_cCar->speed > m_cCar->maxSpeed*0.85) {
            m_cCar->speed -= m_cCar->acceleration*4.0;
            if(m_cCar->speed < m_cCar->maxSpeed*0.85)
                m_cCar->speed = m_cCar->maxSpeed*0.85;
        }
    }else{
        m_cCar->speed += m_cNitro->nitroAcceleration;
        if(m_cCar->speed > m_cNitro->nitroMaxSpeed){
            m_cCar->speed = m_cNitro->nitroMaxSpeed;
        }
    }

    // calculamos las posiciones
    float angleRotation = (m_cTransformableCar->rotation.y * PI) / 180.0;
    m_cTransformableCar->position.x -= cos(angleRotation) * m_cCar->speed * 0.016;
    m_cTransformableCar->position.z += sin(angleRotation) * m_cCar->speed * 0.016;
    if(m_cCar->wheelRotation != 0){
        m_cTransformableCar->rotation.y += m_cCar->wheelRotation * 0.20;
        if(m_cTransformableCar->rotation.y>=360.0)
            m_cTransformableCar->rotation.y -= 360.0;
        else if(m_cTransformableCar->rotation.y < 0.0)
            m_cTransformableCar->rotation.y += 360.0;
    }
}




// se obitene un vector de fuerzas que persigue
glm::vec2 SteeringBehaviours::Seek(Entity* m_originCar, const glm::vec3& m_posTargetCar, const glm::vec2& m_velocityVector) const{
    glm::vec2 vectorForce;
    auto cTransformable = static_cast<CTransformable*>(m_originCar->GetComponent(CompType::TransformableComp).get());
    //auto cCar = static_cast<CCar*>(m_originCar->GetComponent(CompType::CarComp).get());

    // Velocidad deseada
    float vetorToTargetX = (m_posTargetCar.x - cTransformable->position.x);
    float vetorToTargetZ = (m_posTargetCar.z - cTransformable->position.z);
        //Normalizar
    //float vectorDistance = sqrt(vetorToTargetX*vetorToTargetX + vetorToTargetZ*vetorToTargetZ);
    //glm::vec2 desiredVelocity = glm::vec2( vetorToTargetX*(1/vectorDistance) , vetorToTargetZ*(1/vectorDistance)) * cCar->maxSpeed;

    //vectorForce = desiredVelocity - m_velocityVector;
    vectorForce.x = vetorToTargetX;
    vectorForce.y = vetorToTargetZ;

    return vectorForce;
}



glm::vec2 SteeringBehaviours::Arrive(Entity* m_originCar, const glm::vec3& m_posTargetCar, const glm::vec2& m_velocityVector) const{
    glm::vec2 vectorForce;
    auto cTransformable = static_cast<CTransformable*>(m_originCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_originCar->GetComponent(CompType::CarComp).get());

    // Velocidad deseada
    float vetorToTargetX = (m_posTargetCar.x - cTransformable->position.x);
    float vetorToTargetZ = (m_posTargetCar.z - cTransformable->position.z);

    vectorForce.x = vetorToTargetX;
    vectorForce.y = vetorToTargetZ;

    // Calcular distancia
    float distance = sqrt(vetorToTargetX*vetorToTargetX + vetorToTargetZ*vetorToTargetZ);
    float slowRadius = 100.0;
    if(distance < slowRadius){
        cCar->speed = cCar->maxSpeed * distance / slowRadius + 60;
    }

    return vectorForce;
}



// calcula el punto que crees que va a estar y se lo pasa a seek
glm::vec2 SteeringBehaviours::Pursue(Entity* m_originCar, Entity* m_targetCar, const glm::vec2& m_velocityVector) const{
    auto cTransformableTarget = static_cast<CTransformable*>(m_targetCar->GetComponent(CompType::TransformableComp).get());
    auto cTransformable = static_cast<CTransformable*>(m_originCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_originCar->GetComponent(CompType::CarComp).get());
    auto cCarTarget = static_cast<CCar*>(m_targetCar->GetComponent(CompType::CarComp).get());
    glm::vec3 posTarget;
    posTarget.y = 0.0;

    // Calcular distancia
    float vetorToTargetX = (cTransformableTarget->position.x - cTransformable->position.x);
    float vetorToTargetZ = (cTransformableTarget->position.z - cTransformable->position.z);
    float distance = sqrt(vetorToTargetX*vetorToTargetX + vetorToTargetZ*vetorToTargetZ);

    // calcular tiempo que se tarda v = e / t
    float predictionTime = 0.0; // in seconds
    //if(cCar->speed <= distance/3){  // 60 frames -> 1 segundo,    
    //    predictionTime = 3;
    //}else{
        predictionTime = distance / cCar->speed;      // 510.0 -> velocidad melon molon
    //}
    
    // calcular punto al que va a predecir
    float angleRotation = (cTransformableTarget->rotation.y * PI) / 180.0;
    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * 0.016 * predictionTime*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * 0.016 * predictionTime*60;

    return Seek(m_originCar, posTarget, m_velocityVector);
}



// calcula el punto que crees que va a estar y se lo pasa a seek
glm::vec2 SteeringBehaviours::PursuePowerUp(Entity* m_originCar, Entity* m_targetCar, const glm::vec2& m_velocityVector) const{
    auto cTransformableTarget = static_cast<CTransformable*>(m_targetCar->GetComponent(CompType::TransformableComp).get());
    auto cTransformable = static_cast<CTransformable*>(m_originCar->GetComponent(CompType::TransformableComp).get());
    //auto cCar = static_cast<CCar*>(m_originCar->GetComponent(CompType::CarComp).get());
    auto cCarTarget = static_cast<CCar*>(m_targetCar->GetComponent(CompType::CarComp).get());
    glm::vec3 posTarget;
    posTarget.y = 0.0;

    // Calcular distancia
    float vetorWaypointX = (cTransformableTarget->position.x - cTransformable->position.x);
    float vetorWaypointZ = (cTransformableTarget->position.z - cTransformable->position.z);
    float distance = sqrt(vetorWaypointX*vetorWaypointX + vetorWaypointZ*vetorWaypointZ);

    // calcular tiempo que se tarda v = e / t
    float predictionTime = 0.0; // in seconds
    //if(510.0 <= distance/3){  // 60 frames -> 1 segundo,    
    //    predictionTime = 3;
    //}else{
        predictionTime = distance / 510.0;      // 510.0 -> velocidad melon molon
    //}
    
    // calcular punto al que va a predecir
    float angleRotation = (cTransformableTarget->rotation.y * PI) / 180.0;
    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * 0.016 * predictionTime*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * 0.016 * predictionTime*60;
    //std::cout << "Actual pos: ( " << cTransformableTarget->position.x << " , " << cTransformableTarget->position.z << " )    Predicted: ( " << posTarget.x << " , " << posTarget.z << " )" << std::endl;
    
    //  tiempo extra para calcular ya que al ser una esfera tarda un tiempo adicional en llegar le PowerUp
    glm::vec2 vectorToForce = glm::vec2(posTarget.x - cTransformable->position.x , posTarget.z - cTransformable->position.z );
    glm::vec2 vectorToTarget = glm::vec2(cTransformableTarget->position.x - cTransformable->position.x , cTransformableTarget->position.z - cTransformable->position.z );
    float angle = CalculateAngle(vectorToTarget, vectorToForce, cTransformable->rotation.y);
    //if(angle<0) angle*= (-1);
    //posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * 0.016 * (predictionTime+ angle*0.020)*60;
    //posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * 0.016 * (predictionTime+ angle*0.020)*60;
    //std::cout << predictionTime << "Argulo: " << angle*0.01 << std::endl;

    float angle2 = angle + cTransformable->rotation.y;
    if(angle2<0)
        angle2 += 360.0;
    else if(angle2 >= 360)
        angle2 -= 360.0;
    float angleRotation2 = (angle2 * PI) / 180.0;
    float posPUx = cTransformable->position.x - cos(angleRotation2) * 510.0 * 0.016 * predictionTime*60;
    float posPUz = cTransformable->position.z + sin(angleRotation2) * 510.0 * 0.016 * predictionTime*60;

    float vetorDistancePUx = (posTarget.x - posPUx);
    float vetorDistancePUz = (posTarget.z - posPUz);
    float distancePU = sqrt(vetorDistancePUx*vetorDistancePUx + vetorDistancePUz*vetorDistancePUz);
    float predictionTimeExtra = distancePU / 510.0;

    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * 0.016 * (predictionTime+predictionTimeExtra)*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * 0.016 * (predictionTime+predictionTimeExtra)*60;
    //std::cout << distance << std::endl;

    return Seek(m_originCar, posTarget, m_velocityVector);
}



glm::vec2 SteeringBehaviours::ObstacleAvoidance(Entity* m_Car, ManBoxPowerUp* m_manBoxPowerUp, const glm::vec2& m_velocityVector) const{
    auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    glm::vec2 vectorForce = glm::vec2(0.0, 0.0);
    float distance;
    float finalDistance = 9999999999;
    glm::vec2 vectorForceAvoid;

    for(std::shared_ptr<Entity> obstacle : m_manBoxPowerUp->GetEntities()){
        if(CollisionRaySphere(m_Car, obstacle.get(), m_velocityVector, distance, vectorForceAvoid)==true){
            if(distance < finalDistance && distance < cCar->speed*0.4+50 && distance > 0){
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
            }
        }
    }

    return vectorForce;
}


bool SteeringBehaviours::CollisionRaySphere(Entity* m_Car, Entity* m_object, const glm::vec2& m_velocityVector, float& distance, glm::vec2& vectorForceAvoid) const{
    //auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cTransformableCar = static_cast<CTransformable*>(m_Car->GetComponent(CompType::TransformableComp).get());

    // Normalizar vector velocidad
    float vectorDistance = sqrt(m_velocityVector.x*m_velocityVector.x + m_velocityVector.y*m_velocityVector.y);
    glm::vec3 vectorVelocityN = glm::vec3( m_velocityVector.x*(1/vectorDistance) , 0.0 ,m_velocityVector.y*(1/vectorDistance)) ;
    //vectorVelocityN *= 50.0;

    // Propiedades de la esfera en el objeto
    auto cTransformableBox = static_cast<CTransformable*>(m_object->GetComponent(CompType::TransformableComp).get());
    glm::vec3 sphereCenter = glm::vec3(cTransformableBox->position.x, 0.0, cTransformableBox->position.z);
    const float sphereRadius = 50.0;

    glm::vec3 vecDiferenceM = glm::vec3(cTransformableCar->position.x , 0.0, cTransformableCar->position.z) - sphereCenter;
    // Producto escalar
    float b = vecDiferenceM.x * vectorVelocityN.x + 0.0 + vecDiferenceM.z * vectorVelocityN.z;
    float c = vecDiferenceM.x * vecDiferenceM.x + 0.0 + vecDiferenceM.z * vecDiferenceM.z - sphereRadius * sphereRadius;

    if (c > 0.0f && b > 0.0f) return false; 
    float discr = b*b - c; 

    if (discr < 0.0f) return false; 

    // Ray now found to intersect sphere, compute smallest distance value of intersection distancia desde la que intersecta
    distance = -b - sqrt(discr); 

    // If t is negative, ray started inside sphere so clamp t to zero 
    if (distance < 0.0f) distance = 0.0f; 
    // punto donde corta
    glm::vec3 q = glm::vec3(cTransformableCar->position.x , 0.0, cTransformableCar->position.z) + distance * vectorVelocityN; 
    //std::cout << "Veo obstaculo: " << t << std::endl;


    // calcular normal
    glm::vec3 vecNormal = q - sphereCenter;
    float distanceVecNormal = sqrt(vecNormal.x*vecNormal.x + 0 + vecNormal.z*vecNormal.z);
    glm::vec3 vecNormalNormalized = glm::vec3( vecNormal.x*(1/distanceVecNormal) , 0.0 ,vecNormal.z*(1/distanceVecNormal)) ;

    float distanceFromCollision = 40.0;
    glm::vec3 target = q + glm::vec3(vecNormalNormalized.x * distanceFromCollision, 0.0, vecNormalNormalized.z * distanceFromCollision);
    vectorForceAvoid = Seek(m_Car, target, m_velocityVector);

    return true;

// URL: https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
// Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting, 
// returns t value of intersection and intersection point q 
//    int IntersectRaySphere(Point p, Vector d, Sphere s, float &t, Point &q){
//    Vector m = p - s.c; 
//    float b = Dot(m, d); 
//    float c = Dot(m, m) - s.r * s.r; 
//
//    // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0) 
//    if (c > 0.0f && b > 0.0f) return 0; 
//    float discr = b*b - c; 
//
//    // A negative discriminant corresponds to ray missing sphere 
//    if (discr < 0.0f) return 0; 
//
//    // Ray now found to intersect sphere, compute smallest t value of intersection
//    t = -b - Sqrt(discr); 
//
//    // If t is negative, ray started inside sphere so clamp t to zero 
//    if (t < 0.0f) t = 0.0f; 
//    q = p + t * d; 
//
//    return 1;
//    }

    // Obtener vector de fuerza para irse de la pared

}




// devuelve un angulo entre -180 y 180
float SteeringBehaviours::CalculateAngle(const glm::vec2& m_originVec, const glm::vec2& m_destinyVec, float m_rotationY) const{
    // se calcula el angulo entre los dos vectores
    float numerador = m_originVec.x*m_destinyVec.x + m_originVec.y*m_destinyVec.y;
    float denominador = sqrt(m_originVec.x*m_originVec.x + m_originVec.y*m_originVec.y) * sqrt(m_destinyVec.x*m_destinyVec.x + m_destinyVec.y*m_destinyVec.y);
    float angle2 = 0.0;
    if(denominador!=0)
        angle2 = acos(numerador/denominador);
    angle2 = (angle2 * 180) / PI;

    // calcular si tiene que girar a la izquierda o derecha
    float valueAtan2 = atan2(m_destinyVec.y,m_destinyVec.x)*180/PI;
    valueAtan2 = 180.0 - valueAtan2; // se le restan ya que el eje empieza en el lado contrario 
    if(valueAtan2<0)
        valueAtan2 += 360;

    ////calcular opuesto
    float oppositeAngleAtan2 = 0.0;
    if(valueAtan2<180)
        oppositeAngleAtan2 = valueAtan2 + 180;
    else
        oppositeAngleAtan2 = valueAtan2 - 180;
    
    // sentido de giro
    float finalAngle = 0.0;
    if(valueAtan2>=180){ // este if se hace para comprobar que se pueda hacer el && y no caiga en la parte que se reinician los grados
        if(m_rotationY>oppositeAngleAtan2 && m_rotationY<valueAtan2)
            finalAngle = angle2;
        else
            finalAngle = angle2 * (-1);
    }else{
        if(m_rotationY>valueAtan2 && m_rotationY<oppositeAngleAtan2)
            finalAngle = angle2 * (-1);
        else
            finalAngle = angle2;
    }

    return finalAngle;
}