#include "SteeringBehaviours.h"

#include "../Managers/ManCar.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Entities/Entity.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"

#define PI 3.141592

SteeringBehaviours::SteeringBehaviours(){

}




void SteeringBehaviours::Update(ManCar* m_manCar, ManBoxPowerUp* m_manBoxPowerUp) const{

    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_manCar->GetEntitiesAI()[0]->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_manCar->GetEntitiesAI()[0]->GetComponent(CompType::CarComp).get());
    if(cCar->speed==0) cCar->speed=0.1;
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;
    glm::vec2 vectorVelocity = glm::vec2(posXSiguiente - cTransformable->position.x , posZSiguiente - cTransformable->position.z );


    
    
    // Seek
    //auto cTransformableTarget = static_cast<CTransformable*>(m_manCar->GetCar()->GetComponent(CompType::TransformableComp).get());
    //glm::vec3 posTarget = glm::vec3(cTransformableTarget->position.x, cTransformableTarget->position.y, cTransformableTarget->position.z);
    //glm::vec2 vectorForce = Seek(m_manCar->GetEntitiesAI()[0].get(), posTarget, vectorVelocity);

    // Pursue
    glm::vec2 vectorForce = PursuePowerUp(m_manCar->GetEntitiesAI()[0].get(), m_manCar->GetCar().get(), vectorVelocity);

    // Obstacle avoidance
    //glm::vec2 vectorForceAvoid = ObstacleAvoidance(m_manCar->GetEntitiesAI()[0].get(), m_manBoxPowerUp, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateTransformable(cCar, cTransformable, angle);
}


void SteeringBehaviours::UpdateTransformable(CCar* m_cCar, CTransformable* m_cTransformableCar, float angle) const{
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
    
    m_cCar->speed += m_cCar->acceleration;
    if (m_cCar->speed > m_cCar->maxSpeed/2.0) {
        m_cCar->speed = m_cCar->maxSpeed/2.0;  
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
    
    //  tiempo extra para calcular ya que al ser una esfera tarda un tiempo adicional en llegar le PowerUp To-Do: Mejorar el calculo de este tiempo
    glm::vec2 vectorToForce = glm::vec2(posTarget.x - cTransformable->position.x , posTarget.z - cTransformable->position.z );
    glm::vec2 vectorToTarget = glm::vec2(cTransformableTarget->position.x - cTransformable->position.x , cTransformableTarget->position.z - cTransformable->position.z );
    float angle = CalculateAngle(vectorToTarget, vectorToForce, cTransformable->rotation.y);
    if(angle<0) angle*= (-1);
    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * 0.016 * (predictionTime+ angle*0.020)*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * 0.016 * (predictionTime+ angle*0.020)*60;
    //std::cout << predictionTime << "Argulo: " << angle*0.01 << std::endl;

    //float angle2 = angle + cTransformable->rotation.y;
    //if(angle2<0)
    //    angle2 += 360.0;
    //else if(angle2 >= 360)
    //    angle2 -= 360.0;
    //float angleRotation2 = (angle2 * PI) / 180.0;
    //float posPUx = cTransformable->position.x - cos(angleRotation2) * 510.0 * 0.016 * predictionTime*60;
    //float posPUz = cTransformable->position.z + sin(angleRotation2) * 510.0 * 0.016 * predictionTime*60;

    //float vetorDistancePUx = (posTarget.x - posPUx);
    //float vetorDistancePUz = (posTarget.z - posPUx);
    //float distancePU = sqrt(vetorDistancePUx*vetorDistancePUx + vetorDistancePUz*vetorDistancePUz);
    //float predictionTimeExtra = distancePU / 510.0;

    //posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * 0.016 * (predictionTime+predictionTimeExtra)*60;
    //posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * 0.016 * (predictionTime+predictionTimeExtra)*60;
    //std::cout << distance << std::endl;

    return Seek(m_originCar, posTarget, m_velocityVector);
}



glm::vec2 SteeringBehaviours::ObstacleAvoidance(Entity* m_Car, ManBoxPowerUp* m_manBoxPowerUp, const glm::vec2& m_velocityVector) const{
    auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    glm::vec2 vectorForce;

    // Normalizar vector velocidad
    float vectorDistance = sqrt(m_velocityVector.x*m_velocityVector.x + m_velocityVector.y*m_velocityVector.y);
    glm::vec2 vectorVelocityN = glm::vec2( m_velocityVector.x*(1/vectorDistance) , m_velocityVector.y*(1/vectorDistance)) ;
    vectorVelocityN *= 100.0;
    //std::cout << vectorVelocityN.x << "  " << vectorVelocityN.y << std::endl;

    // Calcular colisiones con paredes

    // Obtener vector de fuerza para irse de la pared

    return vectorForce;
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