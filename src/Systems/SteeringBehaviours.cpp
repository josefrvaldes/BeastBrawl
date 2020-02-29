#include "SteeringBehaviours.h"

#include <Managers/ManCar.h>
#include <Managers/ManBoxPowerUp.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManBoundingWall.h>
#include <Entities/Car.h>
#include <Entities/CarHuman.h>
#include <Entities/CarAI.h>
#include <Components/CNitro.h>
#include <Components/CCar.h>
#include <Components/CBoundingSphere.h>
#include <Components/CBoundingPlane.h>
#include <Components/CBoundingRay.h>
#include "../Constants.h"



#define PI 3.141592

// TODO: YA esta en el stareInGame hay que quitarlo el "clPhysics = make_unique<CLPhysics>();"
SteeringBehaviours::SteeringBehaviours(){
    clPhysics = make_unique<CLPhysics>();
}



void SteeringBehaviours::UpdateSeek(Entity* m_actualCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(m_actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(m_actualCar->GetComponent(CompType::CarComp).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    //Seek
    auto cPosDestination = static_cast<CPosDestination*>(m_actualCar->GetComponent(CompType::PosDestination).get());
    glm::vec3 posTarget = glm::vec3(cPosDestination->position.x, cPosDestination->position.y, cPosDestination->position.z);
    glm::vec2 vectorForce = Seek(m_actualCar, posTarget, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateAngleRotation(cCar, angle);
    UpdateSpeed(cCar, cNitro);
    UpdatePosition(cCar, cTransformable);
}



void SteeringBehaviours::UpdateArrive(Entity* m_actualCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(m_actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(m_actualCar->GetComponent(CompType::CarComp).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    //Arrive
    auto cPosDestination = static_cast<CPosDestination*>(m_actualCar->GetComponent(CompType::PosDestination).get());
    glm::vec3 posTarget = glm::vec3(cPosDestination->position.x, cPosDestination->position.y, cPosDestination->position.z);
    glm::vec2 vectorForce = Arrive(m_actualCar, posTarget, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateAngleRotation(cCar, angle);
    UpdateSpeed(cCar, cNitro);
    UpdatePosition(cCar, cTransformable);
}



// devuelve el angulo que le falta para girar para llegar al punto optimo
float SteeringBehaviours::UpdatePursuePowerUp(Entity* m_actualCar, Entity* m_targetCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(m_actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(m_actualCar->GetComponent(CompType::CarComp).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    // Pursue
    glm::vec2 vectorForce = PursuePowerUp(m_actualCar, m_targetCar, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateAngleRotation(cCar, angle);
    UpdateSpeed(cCar, cNitro);
    UpdatePosition(cCar, cTransformable);

    return angle;
}




bool SteeringBehaviours::UpdateObstacleAvoidance(Entity* m_Car, ManCar* m_manCar) const{
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_Car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(m_Car->GetComponent(CompType::NitroComp).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    // Obstacle avoidance
    glm::vec2 vectorForceAvoid = ObstacleAvoidance(m_Car, m_manCar, vectorVelocity);

    if(vectorForceAvoid.x != 0.0 || vectorForceAvoid.y != 0.0 ){
        //std::cout << "Se viene choque" << std::endl;
        float angle = CalculateAngle(vectorVelocity, vectorForceAvoid, cTransformable->rotation.y);
        UpdateAngleRotation(cCar, angle);
        UpdateSpeedAvoidance(cCar, cNitro);
        UpdatePosition(cCar, cTransformable);
        return true;
    }
    return false;
}


bool SteeringBehaviours::UpdateWallAvoidance(Entity* m_Car,  ManBoundingWall* m_manBoundingWall) const{
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(m_Car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(m_Car->GetComponent(CompType::NitroComp).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    // wall avoidance
    glm::vec2 vectorForceAvoid = WallAvoidance(m_Car, m_manBoundingWall, vectorVelocity);

    if(vectorForceAvoid.x != 0.0 || vectorForceAvoid.y != 0.0 ){
        float angle = CalculateAngle(vectorVelocity, vectorForceAvoid, cTransformable->rotation.y);
        UpdateAngleRotation(cCar, angle);
        UpdateSpeedAvoidance(cCar, cNitro);
        UpdatePosition(cCar, cTransformable);
        return true;
    }
    return false;
}



// Actualiza el angulo de rotacion
void SteeringBehaviours::UpdateAngleRotation(CCar* m_cCar, float angle) const{
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
}
// Actualiza la velocidad
void SteeringBehaviours::UpdateSpeed(CCar* m_cCar, CNitro* m_cNitro) const{
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
}
void SteeringBehaviours::UpdateSpeedAvoidance(CCar* m_cCar, CNitro* m_cNitro) const{
    if(m_cNitro->activePowerUp == false && m_cCar->speed > m_cCar->maxSpeed*0.35){
        if (m_cCar->speed > m_cCar->maxSpeed) {
            m_cCar->speed -= m_cCar->acceleration*4.0;
            if(m_cCar->speed < m_cCar->maxSpeed)
                m_cCar->speed = m_cCar->maxSpeed;
        }
    }else if(m_cNitro->activePowerUp == false && m_cCar->speed <= m_cCar->maxSpeed*0.35){
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
}
// Actualiza la posicion
void SteeringBehaviours::UpdatePosition(CCar* m_cCar, CTransformable* m_cTransformableCar) const{
    // calculamos las posiciones
    float angleRotation = (m_cTransformableCar->rotation.y * PI) / 180.0;
    m_cTransformableCar->position.x -= cos(angleRotation) * m_cCar->speed * Constants::DELTA_TIME;
    m_cTransformableCar->position.z += sin(angleRotation) * m_cCar->speed * Constants::DELTA_TIME;
    if(m_cCar->wheelRotation != 0){
        m_cTransformableCar->rotation.y += m_cCar->wheelRotation * 0.20;
        if(m_cTransformableCar->rotation.y>=360.0)
            m_cTransformableCar->rotation.y -= 360.0;
        else if(m_cTransformableCar->rotation.y < 0.0)
            m_cTransformableCar->rotation.y += 360.0;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// STEERING BEHAVIORS ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// se obitene un vector de fuerzas que persigue
glm::vec2 SteeringBehaviours::Seek(Entity* m_originCar, const glm::vec3& m_posTargetCar, const glm::vec2& m_velocityVector) const{
    glm::vec2 vectorForce;
    auto cTransformable = static_cast<CTransformable*>(m_originCar->GetComponent(CompType::TransformableComp).get());

    // Velocidad deseada
    float vetorToTargetX = (m_posTargetCar.x - cTransformable->position.x);
    float vetorToTargetZ = (m_posTargetCar.z - cTransformable->position.z);

    vectorForce.x = vetorToTargetX;
    vectorForce.y = vetorToTargetZ;

    return vectorForce;
}



// se obitene un vector de fuerzas que persigue y disminuye su velocidad al acercarse al objetivo
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



// calcula el punto que crees que va a estar el objetivo y se lo pasa a seek
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
    if(cCar->speed <= distance/3){  // 60 frames -> 1 segundo,    
        predictionTime = 3;
    }else{
        predictionTime = distance / cCar->speed;
    }
    
    // calcular punto al que va a predecir
    float angleRotation = (cTransformableTarget->rotation.y * PI) / 180.0;
    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * predictionTime*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * predictionTime*60;

    return Seek(m_originCar, posTarget, m_velocityVector);
}



// calcula el punto que crees que va a estar el objetivo y se lo pasa a seek
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
    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * predictionTime*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * predictionTime*60;
    //std::cout << "Actual pos: ( " << cTransformableTarget->position.x << " , " << cTransformableTarget->position.z << " )    Predicted: ( " << posTarget.x << " , " << posTarget.z << " )" << std::endl;
    
    //  tiempo extra para calcular ya que al ser una esfera tarda un tiempo adicional en llegar le PowerUp
    glm::vec2 vectorToForce = glm::vec2(posTarget.x - cTransformable->position.x , posTarget.z - cTransformable->position.z );
    glm::vec2 vectorToTarget = glm::vec2(cTransformableTarget->position.x - cTransformable->position.x , cTransformableTarget->position.z - cTransformable->position.z );
    float angle = CalculateAngle(vectorToTarget, vectorToForce, cTransformable->rotation.y);

    float angle2 = angle + cTransformable->rotation.y;
    if(angle2<0)
        angle2 += 360.0;
    else if(angle2 >= 360)
        angle2 -= 360.0;
    float angleRotation2 = (angle2 * PI) / 180.0;
    float posPUx = cTransformable->position.x - cos(angleRotation2) * 510.0 * Constants::DELTA_TIME * predictionTime*60;
    float posPUz = cTransformable->position.z + sin(angleRotation2) * 510.0 * Constants::DELTA_TIME * predictionTime*60;

    float vetorDistancePUx = (posTarget.x - posPUx);
    float vetorDistancePUz = (posTarget.z - posPUz);
    float distancePU = sqrt(vetorDistancePUx*vetorDistancePUx + vetorDistancePUz*vetorDistancePUz);
    float predictionTimeExtra = distancePU / 510.0;

    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * (predictionTime+predictionTimeExtra)*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * (predictionTime+predictionTimeExtra)*60;
    //std::cout << distance << std::endl;

    return Seek(m_originCar, posTarget, m_velocityVector);
}



// obtiene un vector fuerza en caso de ir a colisionar contra un objeto con bounding esferica
glm::vec2 SteeringBehaviours::ObstacleAvoidance(Entity* m_Car, ManCar* m_manCar, const glm::vec2& m_velocityVector) const{
    auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cRay = static_cast<CBoundingRay*>(m_Car->GetComponent(CompType::CompBoundingRay).get());
    glm::vec2 vectorForce = glm::vec2(0.0, 0.0);
    float distance = 999999999.0;
    float finalDistance = 99999999.0;
    glm::vec2 vectorForceAvoid;
    Entity* actualObstacle = nullptr;

    for(const auto& obstacle : m_manCar->GetEntities()){
        //auto cPowerUp = static_cast<CPowerUp*>(obstacle->GetComponent(CompType::PowerUpComp).get());
        if(obstacle.get()!=m_Car && CollisionRaySphere(m_Car, obstacle.get(), m_velocityVector, distance, vectorForceAvoid)==true){
            if(distance < finalDistance && distance < cCar->speed*0.4+cRay->baseDistanceSphere && distance > 0){
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
                actualObstacle= obstacle.get();
            }
        }
    }

    // modifica el vectorForce
    AvoidVibration(m_Car, actualObstacle, m_velocityVector, vectorForce);

    return vectorForce;
}


// obtiene un vector fuerza en caso de ir a colisionar contra un muro con bounding de plano
glm::vec2 SteeringBehaviours::WallAvoidance(Entity* m_Car, ManBoundingWall* m_manBoundingWall, const glm::vec2& m_velocityVector) const{
    auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cRay = static_cast<CBoundingRay*>(m_Car->GetComponent(CompType::CompBoundingRay).get());
    glm::vec2 vectorForce = glm::vec2(0.0, 0.0);
    glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
    float distance = 999999999.0;
    float finalDistance = 99999998.0;
    glm::vec2 vectorForceAvoid;
    Entity* actualObstacle = nullptr;

    for(const auto& obstacle : m_manBoundingWall->GetEntities()){
        if(CollisionRayPlane(m_Car, obstacle.get(), m_velocityVector, distance, vectorForceAvoid, target)==true){
            if(distance < finalDistance && distance < cCar->speed*0.4+cRay->baseDistancePlane && distance > 0){
                //std::cout << "Collisiona" << std::endl;
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
                actualObstacle = obstacle.get();
            }
        }
    }

    // modifica el vectorForce
    AvoidTrapCorner(m_Car, actualObstacle, m_velocityVector, target, vectorForce);

    return vectorForce;
}



bool SteeringBehaviours::CollisionRaySphere(Entity* m_Car, Entity* m_object, const glm::vec2& m_velocityVector, float& distance, glm::vec2& vectorForceAvoid) const{
    //auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cTransformableCar = static_cast<CTransformable*>(m_Car->GetComponent(CompType::TransformableComp).get());
    auto cTransformableObject = static_cast<CTransformable*>(m_object->GetComponent(CompType::TransformableComp).get());
    auto cSphereObject = static_cast<CBoundingSphere*>(m_object->GetComponent(CompType::CompBoundingSphere).get());

    // Normalizar vector velocidad del coche
    float vectorDistance = sqrt(m_velocityVector.x*m_velocityVector.x + m_velocityVector.y*m_velocityVector.y);
    glm::vec3 vectorVelocityN = glm::vec3( m_velocityVector.x*(1/vectorDistance) , 0.0 ,m_velocityVector.y*(1/vectorDistance));

    IntersectData intersData = clPhysics->HandleCollisionsRayWithSpheres(*cTransformableCar, *cTransformableObject, *cSphereObject, vectorVelocityN);
    if(intersData.intersects == false) return false;
    if(CollisionFaceToFace(m_Car, m_object) == false) return false;
    distance = intersData.distance;
    vectorForceAvoid = Seek(m_Car, intersData.direction, m_velocityVector); // aunque ponga direction es el target

    return true;
}



bool SteeringBehaviours::CollisionRayPlane(Entity* m_Car, Entity* m_object, const glm::vec2& m_velocityVector, float& distance, glm::vec2& vectorForceAvoid, glm::vec3& target) const{
    //auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cTransformableCar = static_cast<CTransformable*>(m_Car->GetComponent(CompType::TransformableComp).get());
    auto cBoundPlaneObject = static_cast<CBoundingPlane*>(m_object->GetComponent(CompType::CompBoundingPlane).get());

    // Normalizar vector velocidad del coche
    float vectorDistance = sqrt(m_velocityVector.x*m_velocityVector.x + m_velocityVector.y*m_velocityVector.y);
    glm::vec3 vectorVelocityN = glm::vec3( m_velocityVector.x*(1/vectorDistance) , 0.0 ,m_velocityVector.y*(1/vectorDistance)) ;

    IntersectData intersData = clPhysics->HandleCollisionsRayWithPlane(*cTransformableCar, vectorVelocityN, *cBoundPlaneObject);
    if(intersData.intersects == false) return false;
    distance = intersData.distance;
    target = intersData.direction;
    vectorForceAvoid = Seek(m_Car, intersData.direction, m_velocityVector); // aunque ponga direction es el target

    return true;
}



// trata de evitar quedarse atrapado en la esquina devolviendo el vector al que debe moverse el coche
void SteeringBehaviours::AvoidTrapCorner(Entity* m_Car, Entity *actualObstacle, const glm::vec2& m_velocityVector, const glm::vec3& target, glm::vec2& vectorForce) const{
    auto cRay = static_cast<CBoundingRay*>(m_Car->GetComponent(CompType::CompBoundingRay).get());
    // evitar quedar atrapado en la esquina
    if(actualObstacle == nullptr){                     // no colisiona contra nada
        if(cRay->previousPlane != nullptr && cRay->iteratorSamePlane < cRay->maxItSamePlane){
            cRay->iteratorSamePlane++;
            vectorForce = Seek(m_Car, cRay->target, m_velocityVector);
            
        }else if(cRay->iteratorSamePlane >= cRay->maxItSamePlane){
            cRay->iteratorSamePlane = 0;
            cRay->previousPlane = nullptr; 
        }
    }else{                                          // colisiona contra un muro
        auto planeObstacle = static_cast<CBoundingPlane*>(actualObstacle->GetComponent(CompType::CompBoundingPlane).get());                           
        if(planeObstacle != cRay->previousPlane){
            if(cRay->previousPlane != nullptr && cRay->iteratorSamePlane < cRay->maxItSamePlane){
                cRay->iteratorSamePlane++;
                vectorForce = Seek(m_Car, cRay->target, m_velocityVector);
            }else if(cRay->iteratorSamePlane >= cRay->maxItSamePlane || cRay->previousPlane == nullptr){
                cRay->iteratorSamePlane = 0;
                cRay->previousPlane = planeObstacle;
                cRay->target = target;
            }
        }else if(cRay->iteratorSamePlane == 0){
            cRay->target = target;
        }
    }
}


// trata de evitar la vibracion al evitar un coche
void SteeringBehaviours::AvoidVibration(Entity* m_Car, Entity *actualObstacle, const glm::vec2& m_velocityVector, glm::vec2& vectorForce) const{
    auto cRay = static_cast<CBoundingRay*>(m_Car->GetComponent(CompType::CompBoundingRay).get());
    // evitar vibracion
    if(actualObstacle == nullptr){                      // no colisiona contra nada
        if(cRay->previousSphere != nullptr && cRay->iteratorSameSphere < cRay->maxItSameSphere){
            cRay->iteratorSameSphere++;
            vectorForce = cRay->vectorForce;
        }else if(cRay->iteratorSameSphere >= 10){
            cRay->iteratorSameSphere = 0;
            cRay->previousSphere = nullptr;
        }
    }else{                                              // colisiona contra un muro
        auto sphereObstacle = static_cast<CBoundingSphere*>(actualObstacle->GetComponent(CompType::CompBoundingSphere).get());
        if(sphereObstacle != cRay->previousSphere){
            if(cRay->previousSphere != nullptr && cRay->iteratorSameSphere < cRay->maxItSameSphere){
                cRay->iteratorSameSphere++;
                vectorForce = cRay->vectorForce;
            }else if(cRay->iteratorSameSphere >= 10 || cRay->previousSphere == nullptr){
                cRay->iteratorSameSphere = 0;
                cRay->previousSphere = sphereObstacle;
                cRay->vectorForce = vectorForce;
            }
        }else if(cRay->iteratorSamePlane == 0){
            cRay->vectorForce = vectorForce;
        }
    }
}


// comprobamos si dos coches van a colisionar de frente para aplicarle la evasion
bool SteeringBehaviours::CollisionFaceToFace(Entity* m_Car, Entity *m_object) const{
    auto cTransCar = static_cast<CTransformable*>(m_Car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(m_Car->GetComponent(CompType::CarComp).get());
    auto cTransObject = static_cast<CTransformable*>(m_object->GetComponent(CompType::TransformableComp).get());
    auto cObject = static_cast<CCar*>(m_object->GetComponent(CompType::CarComp).get());
    float carSpeed = cCar->speed;
    float objectSpeed = cObject->speed;

    if(carSpeed==0) carSpeed=0.1;
    if(objectSpeed==0) objectSpeed =0.1;

    float angleRotationCar = (cTransCar->rotation.y * PI) / 180.0;
    glm::vec2 carVector(cos(angleRotationCar) * carSpeed, sin(angleRotationCar) * carSpeed);

    float angleRotationObject = (cTransObject->rotation.y * PI) / 180.0;
    glm::vec2 objectVector(cos(angleRotationObject) * objectSpeed, sin(angleRotationObject) * objectSpeed);

    // se calcula el angulo entre los dos vectores
    float numerador = carVector.x*objectVector.x + carVector.y*objectVector.y;
    float denominador = sqrt(carVector.x*carVector.x + carVector.y*carVector.y) * sqrt(objectVector.x*objectVector.x + objectVector.y*objectVector.y);
    float angle = 0.0;
    if(denominador!=0)
        angle = acos(numerador/denominador);
    angle = (angle * 180) / PI;

    if(angle<0) angle*=(-1);

    if(angle > 120.0)
        return true;
    else
        return false;
}



glm::vec2 SteeringBehaviours::CalculateVectorVelocity(CCar &m_cCar, CTransformable &transformableCar) const{
    if(m_cCar.speed==0) m_cCar.speed=0.1;
    float angleRotation = (transformableCar.rotation.y * PI) / 180.0;
    float posXSiguiente = transformableCar.position.x - cos(angleRotation) * m_cCar.speed;
    float posZSiguiente = transformableCar.position.z + sin(angleRotation) * m_cCar.speed;
    return glm::vec2(posXSiguiente - transformableCar.position.x , posZSiguiente - transformableCar.position.z );
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







/*
// http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
    glm::vec3 ray_origin(cTransformableCar->position.x, cTransformableCar->position.y, cTransformableCar->position.z);
    glm::vec3 ray_direction = vectorVelocityN;
    float intersection_distance = -1; // Output of TestRayOBBIntersection()
	glm::vec3 aabb_min(-1, -1, -1);
	glm::vec3 aabb_max( 1, 1, 1);

    glm::quat orientations = glm::quat(glm::vec3(cTransformableObject->rotation.x, cTransformableObject->rotation.y, cTransformableObject->rotation.z));

    glm::mat4 RotationMatrix = glm::toMat4(orientations);
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(cTransformableObject->position.x, cTransformableObject->position.y, cTransformableObject->position.z));
	glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix;

    if ( TestRayOBBIntersection( ray_origin, ray_direction, aabb_min, aabb_max, ModelMatrix, intersection_distance) ){
        std::cout << "HAY COLISION: " << intersection_distance << std::endl;
		//std::ostringstream oss;
		//oss << "mesh " << i;
		//message = oss.str();
		//break;
	}else{
        std::cout << "No HAY: " << std::endl;
    }
bool  SteeringBehaviours::TestRayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction, glm::vec3 aabb_min, glm::vec3 aabb_max, glm::mat4 ModelMatrix, float& intersection_distance) const{
	// Intersection method from Real-Time Rendering and Essential Mathematics for Games
	
	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	glm::vec3 delta = OBBposition_worldspace - ray_origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_direction, xaxis);

		if ( fabs(f) > 0.001f ){ // Standard case

			float t1 = (e+aabb_min.x)/f; // Intersection with the "left" plane
			float t2 = (e+aabb_max.x)/f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2
			if (t1>t2){
				float w=t1;t1=t2;t2=w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if ( t2 < tMax )
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if ( t1 > tMin )
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin )
				return false;

		}else{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if(-e+aabb_min.x > 0.0f || -e+aabb_max.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray_direction, yaxis);

		if ( fabs(f) > 0.001f ){

			float t1 = (e+aabb_min.y)/f;
			float t2 = (e+aabb_max.y)/f;

			if (t1>t2){float w=t1;t1=t2;t2=w;}

			if ( t2 < tMax )
				tMax = t2;
			if ( t1 > tMin )
				tMin = t1;
			if (tMin > tMax)
				return false;

		}else{
			if(-e+aabb_min.y > 0.0f || -e+aabb_max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(ray_direction, zaxis);

		if ( fabs(f) > 0.001f ){

			float t1 = (e+aabb_min.z)/f;
			float t2 = (e+aabb_max.z)/f;

			if (t1>t2){float w=t1;t1=t2;t2=w;}

			if ( t2 < tMax )
				tMax = t2;
			if ( t1 > tMin )
				tMin = t1;
			if (tMin > tMax)
				return false;

		}else{
			if(-e+aabb_min.z > 0.0f || -e+aabb_max.z < 0.0f)
				return false;
		}
	}

	intersection_distance = tMin;
	return true;
}
*/