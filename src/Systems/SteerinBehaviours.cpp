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




void SteeringBehaviours::Update(ManCar* m_manCar){

    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_manCar->GetEntitiesAI()[0]->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_manCar->GetEntitiesAI()[0]->GetComponent(CompType::CarComp).get());
    if(cCar->speed==0) cCar->speed=0.1;
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;
    glm::vec2 vectorVelocity = glm::vec2(posXSiguiente - cTransformable->position.x , posZSiguiente - cTransformable->position.z );


    // Seek
    glm::vec2 vectorForceSeek = Seek(m_manCar->GetEntitiesAI()[0].get(), m_manCar->GetCar().get(), vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForceSeek, cTransformable->rotation.y);

    UpdateTransformable(cTransformable, angle);
}


void SteeringBehaviours::UpdateTransformable(CTransformable* m_cTransformableCar, float angle){

}




// se obitene un vector de fuerzas que persigue
glm::vec2 SteeringBehaviours::Seek(Entity* m_originCar, Entity* m_destinyCar, glm::vec2 m_velocityVector){
    glm::vec2 vectorForce;
    vectorForce.x = 0;
    vectorForce.y = 0;
    auto cTransformable = static_cast<CTransformable*>(m_originCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_originCar->GetComponent(CompType::CarComp).get());

    // Velocidad deseada
    auto cTransformableOther = static_cast<CTransformable*>(m_destinyCar->GetComponent(CompType::TransformableComp).get());
    float vetorWaypointX = (cTransformableOther->position.x - cTransformable->position.x);
    float vetorWaypointZ = (cTransformableOther->position.z - cTransformable->position.z);
        //Normalizar
    float vector = sqrt(vetorWaypointX*vetorWaypointX + vetorWaypointZ*vetorWaypointZ);
    glm::vec2 desiredVelocity = glm::vec2( vetorWaypointX*(1/vector) , vetorWaypointZ*(1/vector)) * cCar->maxSpeed;

    vectorForce = desiredVelocity - m_velocityVector;

    return vectorForce;
}






float SteeringBehaviours::CalculateAngle(glm::vec2 m_originVec, glm::vec2 m_destinyVec, float m_rotationY){
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