#include "SteeringBehaviours.h"

#include <Managers/ManBoxPowerUp.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManBoundingWall.h>
#include <Managers/ManBoundingOBB.h>
#include <Managers/ManNavMesh.h>
#include <Entities/Car.h>
#include <Entities/CarHuman.h>
#include <Entities/CarAI.h>
#include <Entities/NavMesh.h>
#include <Components/CNitro.h>
#include <Components/CCar.h>
#include <Components/CBrainAI.h>
#include <Components/CExternalForce.h>
#include <Components/CBoundingSphere.h>
#include <Components/CBoundingPlane.h>
#include <Components/CBoundingOBB.h>
#include <Components/CBoundingRay.h>
#include <Components/CCurrentNavMesh.h>
#include "../Constants.h"
#include "../Systems/Utils.h"
#include <math.h>



SteeringBehaviours::SteeringBehaviours(){
}


// comprueba si es se le puede disparar
bool SteeringBehaviours::IsTargeteable(Entity* actualCar, Entity* targetCar, ManBoundingWall* manBoundingWall) const{
    auto cTransCar1 = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cTransCar2 = static_cast<CTransformable*>(targetCar->GetComponent(CompType::TransformableComp).get());
    glm::vec3 vectorToCar(cTransCar2->position.x - cTransCar1->position.x , cTransCar2->position.y - cTransCar1->position.y, cTransCar2->position.z - cTransCar1->position.z );

    // Normalizar vector
    float vectorDistance = sqrt(vectorToCar.x*vectorToCar.x + vectorToCar.y*vectorToCar.y + vectorToCar.z*vectorToCar.z);
    glm::vec3 vectorToCarNormalized = glm::vec3( vectorToCar.x*(1/vectorDistance) , vectorToCar.y*(1/vectorDistance) ,vectorToCar.z*(1/vectorDistance)) ;

    for(const auto& obstacle : manBoundingWall->GetEntities()){
        auto cBoundPlaneObject = static_cast<CBoundingPlane*>(obstacle->GetComponent(CompType::CompBoundingPlane).get());
        IntersectData intersData = clPhysics->HandleCollisionsRayWithPlane(*cTransCar1, vectorToCarNormalized, *cBoundPlaneObject);
        if(intersData.intersects == true) return false;
    }
    return true;
}




void SteeringBehaviours::UpdateSeek(Entity* actualCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cExternalForce = static_cast<CExternalForce*>(actualCar->GetComponent(CompType::CompExternalForce).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    //Seek
    auto cPosDestination = static_cast<CPosDestination*>(actualCar->GetComponent(CompType::PosDestination).get());
    glm::vec3 posTarget = glm::vec3(cPosDestination->position.x, cPosDestination->position.y, cPosDestination->position.z);
    glm::vec2 vectorForce = Seek(actualCar, posTarget, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateAngleRotation(cCar, angle);
    UpdateSpeed(cCar, cNitro);
    UpdatePosition(cCar, cTransformable, cExternalForce);
}



void SteeringBehaviours::UpdateArrive(Entity* actualCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cExternalForce = static_cast<CExternalForce*>(actualCar->GetComponent(CompType::CompExternalForce).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    //Arrive
    auto cPosDestination = static_cast<CPosDestination*>(actualCar->GetComponent(CompType::PosDestination).get());
    glm::vec3 posTarget = glm::vec3(cPosDestination->position.x, cPosDestination->position.y, cPosDestination->position.z);
    glm::vec2 vectorForce = Arrive(actualCar, posTarget, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateAngleRotation(cCar, angle);
    UpdateSpeed(cCar, cNitro);
    UpdatePosition(cCar, cTransformable, cExternalForce);
}



// devuelve el angulo que le falta para girar para llegar al punto optimo
float SteeringBehaviours::UpdatePursuePowerUp(Entity* actualCar, Entity* targetCar){
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cNitro = static_cast<CNitro *>(actualCar->GetComponent(CompType::NitroComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cExternalForce = static_cast<CExternalForce*>(actualCar->GetComponent(CompType::CompExternalForce).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    // Pursue
    glm::vec2 vectorForce = PursuePowerUp(actualCar, targetCar, vectorVelocity);

    float angle = CalculateAngle(vectorVelocity, vectorForce, cTransformable->rotation.y);
    UpdateAngleRotation(cCar, angle);
    UpdateSpeed(cCar, cNitro);
    UpdatePosition(cCar, cTransformable, cExternalForce);

    return angle;
}




bool SteeringBehaviours::UpdateCarAvoidance(Entity* actualCar) const{
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(actualCar->GetComponent(CompType::NitroComp).get());
    auto cExternalForce = static_cast<CExternalForce*>(actualCar->GetComponent(CompType::CompExternalForce).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    // Obstacle avoidance
    glm::vec2 vectorForceAvoid = CarAvoidance(actualCar, vectorVelocity);

    if(vectorForceAvoid.x != 0.0 || vectorForceAvoid.y != 0.0 ){
        //std::cout << "Se viene choque" << std::endl;
        float angle = CalculateAngle(vectorVelocity, vectorForceAvoid, cTransformable->rotation.y);
        UpdateAngleRotation(cCar, angle);
        UpdateSpeedAvoidance(cCar, cNitro);
        UpdatePosition(cCar, cTransformable, cExternalForce);
        return true;
    }
    return false;
}



bool SteeringBehaviours::UpdateWallAvoidance(Entity* actualCar,  ManBoundingWall* manBoundingWall, ManBoundingOBB* manBoundingOBB) const{
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(actualCar->GetComponent(CompType::NitroComp).get());
    auto cExternalForce = static_cast<CExternalForce*>(actualCar->GetComponent(CompType::CompExternalForce).get());
    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);

    // wall avoidance
    glm::vec2 vectorForceAvoid = WallAvoidance(actualCar, manBoundingWall, manBoundingOBB, vectorVelocity);

    if(vectorForceAvoid.x != 0.0 || vectorForceAvoid.y != 0.0 ){
        float angle = CalculateAngle(vectorVelocity, vectorForceAvoid, cTransformable->rotation.y);
        UpdateAngleRotation(cCar, angle);
        UpdateSpeedAvoidance(cCar, cNitro);
        UpdatePosition(cCar, cTransformable, cExternalForce);
        return true;
    }
    return false;
}


bool SteeringBehaviours::UpdateObstacleAvoidance(Entity* actualCar, ManBoundingOBB* manBoundingOBB) const{
    // se calcula el vector al siguiente punto al que avanzara el coche
    auto cTransformable = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(actualCar->GetComponent(CompType::NitroComp).get());
    auto cExternalForce = static_cast<CExternalForce*>(actualCar->GetComponent(CompType::CompExternalForce).get());
    std::vector<glm::vec2> vectorRay = CalculateRayVector(*cCar, *cTransformable);

    // Obstacle avoidance
    glm::vec2 vectorForceAvoid = ObstacleAvoidance(actualCar, manBoundingOBB, vectorRay);
    if(vectorForceAvoid.x != 0.0 || vectorForceAvoid.y != 0.0 ){
        float angle = CalculateAngle(vectorRay[0], vectorForceAvoid, cTransformable->rotation.y);
        UpdateAngleRotation(cCar, angle);
        UpdateSpeedAvoidance(cCar, cNitro);
        UpdatePosition(cCar, cTransformable, cExternalForce);
        return true;
    }
    return false;
}


// el coche esta pensando por lo que no realiza ninguna accion
void SteeringBehaviours::UpdateThink(Entity* actualCar, ManNavMesh* manNavMesh){
    auto cTransformable = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(actualCar->GetComponent(CompType::NitroComp).get());
    auto cExternalForce = static_cast<CExternalForce*>(actualCar->GetComponent(CompType::CompExternalForce).get());

    glm::vec3 posToThink = CalculateCenterNavMesh(actualCar, manNavMesh);

    if(!cNitro->activePowerUp){
        cCar->speed -= (cCar->friction+cCar->friction);
        if (cCar->speed <= 0) {
            cCar->speed = 0;
        }
    }else{
        cCar->speed += cNitro->nitroAcceleration - (cCar->friction+cCar->friction);
        if(cCar->speed > cNitro->nitroMaxSpeed){
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }

    glm::vec2 vectorVelocity = CalculateVectorVelocity(*cCar, *cTransformable);
    glm::vec2 vectorTarget = glm::vec2(posToThink.x-cTransformable->position.x, posToThink.z-cTransformable->position.z);
    float angle = CalculateAngle(vectorVelocity, vectorTarget, cTransformable->rotation.y);
    if(angle<20 && angle>-20)
        angle = 0;

    UpdateAngleRotation(cCar, angle);
    UpdatePosition(cCar, cTransformable, cExternalForce);
}




// Actualiza el angulo de rotacion
void SteeringBehaviours::UpdateAngleRotation(CCar* cCar, float angle) const{
    if(angle>0.1)
        cCar->wheelRotation += 4*cCar->incrementWheelRotation;
    else if(angle<-0.1)
        cCar->wheelRotation -= 4*cCar->incrementWheelRotation;
    else
        cCar->wheelRotation = 0;

    if(cCar->wheelRotation > cCar->maxWheelRotation*1.5) 
        cCar->wheelRotation = cCar->maxWheelRotation*1.5;
    else if(cCar->wheelRotation < -cCar->maxWheelRotation*1.5)
        cCar->wheelRotation = -cCar->maxWheelRotation*1.5;

}
// Actualiza la velocidad
void SteeringBehaviours::UpdateSpeed(CCar* cCar, CNitro* cNitro) const{
    if(cNitro->activePowerUp == false){
        cCar->speed += cCar->acceleration;
        if (cCar->speed > cCar->maxSpeed*0.85) {
            cCar->speed -= cCar->acceleration*4.0;
            if(cCar->speed < cCar->maxSpeed*0.85)
                cCar->speed = cCar->maxSpeed*0.85;
        }
    }else{
        cCar->speed += cNitro->nitroAcceleration;
        if(cCar->speed > cNitro->nitroMaxSpeed){
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}
// Actualiza la velocidad en caso de estar evadiendo
void SteeringBehaviours::UpdateSpeedAvoidance(CCar* cCar, CNitro* cNitro) const{
    if(cNitro->activePowerUp == false && cCar->speed > cCar->maxSpeed*0.35){
        if (cCar->speed > cCar->maxSpeed) {
            cCar->speed -= cCar->acceleration*4.0;
            if(cCar->speed < cCar->maxSpeed)
                cCar->speed = cCar->maxSpeed;
        }
    }else if(cNitro->activePowerUp == false && cCar->speed <= cCar->maxSpeed*0.35){
        cCar->speed += cCar->acceleration;
        if (cCar->speed > cCar->maxSpeed*0.85) {
            cCar->speed -= cCar->acceleration*4.0;
            if(cCar->speed < cCar->maxSpeed*0.85)
                cCar->speed = cCar->maxSpeed*0.85;
        }
    }else{
        cCar->speed += cNitro->nitroAcceleration;
        if(cCar->speed > cNitro->nitroMaxSpeed){
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}
// Actualiza la posicion
void SteeringBehaviours::UpdatePosition(CCar* cCar, CTransformable* cTransformableCar, CExternalForce* cExternalForce) const{
    // calculamos las posiciones
    float angleRotation = glm::radians(cTransformableCar->rotation.y);
    glm::vec2 carForce(-cos(angleRotation)*cCar->speed , sin(angleRotation)*cCar->speed);  // Fijarse en el menos de la X
    glm::vec2 finalForce = ApplyExternalForce(cCar, cExternalForce, carForce);

    cTransformableCar->position.x += finalForce.x * Constants::DELTA_TIME;
    cTransformableCar->position.z += finalForce.y * Constants::DELTA_TIME;

    if(cCar->wheelRotation != 0){
        cTransformableCar->rotation.y += cCar->wheelRotation * 0.20;
        cTransformableCar->rotation.y = Utils::GetAdjustedDegrees(cTransformableCar->rotation.y);
    }
}



// aplicamos al movimiento del coche el desplazamiento en caso de que alguien lo empuje
glm::vec2 SteeringBehaviours::ApplyExternalForce(CCar *cCar, CExternalForce *externalForce, const glm::vec2& carForce) const{
    glm::vec2 finalForce(carForce);
    if(externalForce->force > 0){
        glm::vec2 collisionForce(externalForce->dirExternalForce.x*externalForce->force, externalForce->dirExternalForce.z*externalForce->force);
        //float angleForces = glm::degrees(atan2(collisionForce.y, collisionForce.x)) - glm::degrees(atan2(carForce.y, carForce.x));
        //angleForces = Utils::GetAdjustedDegrees(angleForces);
        finalForce.x = carForce.x + collisionForce.x;
        finalForce.y = carForce.y + collisionForce.y;
        externalForce->force -= externalForce->friction;
    }else{
        externalForce->force = 0.0;
    }
    return finalForce;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// STEERING BEHAVIORS ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// se obitene un vector de fuerzas que persigue
glm::vec2 SteeringBehaviours::Seek(Entity* originCar, const glm::vec3& posTargetCar, const glm::vec2& velocityVector) const{
    glm::vec2 vectorForce;
    auto cTransformable = static_cast<CTransformable*>(originCar->GetComponent(CompType::TransformableComp).get());

    // Velocidad deseada
    float vetorToTargetX = (posTargetCar.x - cTransformable->position.x);
    float vetorToTargetZ = (posTargetCar.z - cTransformable->position.z);

    vectorForce.x = vetorToTargetX;
    vectorForce.y = vetorToTargetZ;

    return vectorForce;
}



// se obitene un vector de fuerzas que persigue y disminuye su velocidad al acercarse al objetivo
glm::vec2 SteeringBehaviours::Arrive(Entity* originCar, const glm::vec3& posTargetCar, const glm::vec2& velocityVector) const{
    glm::vec2 vectorForce;
    auto cTransformable = static_cast<CTransformable*>(originCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(originCar->GetComponent(CompType::CarComp).get());

    // Velocidad deseada
    float vetorToTargetX = (posTargetCar.x - cTransformable->position.x);
    float vetorToTargetZ = (posTargetCar.z - cTransformable->position.z);

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
glm::vec2 SteeringBehaviours::Pursue(Entity* originCar, Entity* targetCar, const glm::vec2& velocityVector) const{
    auto cTransformableTarget = static_cast<CTransformable*>(targetCar->GetComponent(CompType::TransformableComp).get());
    auto cTransformable = static_cast<CTransformable*>(originCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(originCar->GetComponent(CompType::CarComp).get());
    auto cCarTarget = static_cast<CCar*>(targetCar->GetComponent(CompType::CarComp).get());
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
    float angleRotation = glm::radians(cTransformableTarget->rotation.y);
    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * predictionTime*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * predictionTime*60;

    return Seek(originCar, posTarget, velocityVector);
}



// calcula el punto que crees que va a estar el objetivo y se lo pasa a seek
glm::vec2 SteeringBehaviours::PursuePowerUp(Entity* originCar, Entity* targetCar, const glm::vec2& velocityVector) const{
    auto cTransformableTarget = static_cast<CTransformable*>(targetCar->GetComponent(CompType::TransformableComp).get());
    auto cTransformable = static_cast<CTransformable*>(originCar->GetComponent(CompType::TransformableComp).get());
    auto cCarTarget = static_cast<CCar*>(targetCar->GetComponent(CompType::CarComp).get());
    glm::vec3 posTarget;
    posTarget.y = 0.0;

    // Calcular distancia
    float vetorWaypointX = (cTransformableTarget->position.x - cTransformable->position.x);
    float vetorWaypointZ = (cTransformableTarget->position.z - cTransformable->position.z);
    float distance = sqrt(vetorWaypointX*vetorWaypointX + vetorWaypointZ*vetorWaypointZ);

    // calcular tiempo que se tarda v = e / t
    float predictionTime = 0.0; // in seconds
    //if(Constants::MELON_MOLON_SPEED <= distance/3){  // 60 frames -> 1 segundo,    
    //    predictionTime = 3;
    //}else{
        predictionTime = distance / Constants::MELON_MOLON_SPEED;      // Constants::MELON_MOLON_SPEED -> velocidad melon molon
    //}
    
    // calcular punto al que va a predecir
    float angleRotation = glm::radians(cTransformableTarget->rotation.y);
    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * predictionTime*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * predictionTime*60;
    
    //  tiempo extra para calcular ya que al ser una esfera tarda un tiempo adicional en llegar le PowerUp
    glm::vec2 vectorToForce = glm::vec2(posTarget.x - cTransformable->position.x , posTarget.z - cTransformable->position.z );
    glm::vec2 vectorToTarget = glm::vec2(cTransformableTarget->position.x - cTransformable->position.x , cTransformableTarget->position.z - cTransformable->position.z );
    float angle = CalculateAngle(vectorToTarget, vectorToForce, cTransformable->rotation.y);

    float angle2 = angle + cTransformable->rotation.y;
    angle2 = Utils::GetAdjustedDegrees(angle2);

    float angleRotation2 = glm::radians(angle2);
    float posPUx = cTransformable->position.x - cos(angleRotation2) * Constants::MELON_MOLON_SPEED * Constants::DELTA_TIME * predictionTime*60;
    float posPUz = cTransformable->position.z + sin(angleRotation2) * Constants::MELON_MOLON_SPEED * Constants::DELTA_TIME * predictionTime*60;

    float vetorDistancePUx = (posTarget.x - posPUx);
    float vetorDistancePUz = (posTarget.z - posPUz);
    float distancePU = sqrt(vetorDistancePUx*vetorDistancePUx + vetorDistancePUz*vetorDistancePUz);
    float predictionTimeExtra = distancePU / Constants::MELON_MOLON_SPEED;

    posTarget.x = cTransformableTarget->position.x - cos(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * (predictionTime+predictionTimeExtra)*60;
    posTarget.z = cTransformableTarget->position.z + sin(angleRotation) * cCarTarget->speed * Constants::DELTA_TIME * (predictionTime+predictionTimeExtra)*60;

    return Seek(originCar, posTarget, velocityVector);
}



// obtiene un vector fuerza en caso de ir a colisionar contra un objeto con bounding esferica
glm::vec2 SteeringBehaviours::CarAvoidance(Entity* actualCar, const glm::vec2& velocityVector) const{
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cBrainAI = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
    auto cRay = static_cast<CBoundingRay*>(actualCar->GetComponent(CompType::CompBoundingRay).get());
    glm::vec2 vectorForce = glm::vec2(0.0, 0.0);
    float distance = 99999.0;
    float finalDistance = 9999.0;
    glm::vec2 vectorForceAvoid;
    Entity* actualObstacle = nullptr;

    for(const auto& obstacleCar : cBrainAI->carInVision){
        //auto cPowerUp = static_cast<CPowerUp*>(obstacleCar->GetComponent(CompType::PowerUpComp).get());
        if(obstacleCar!=actualCar && CollisionRaySphere(actualCar, obstacleCar, velocityVector, distance, vectorForceAvoid)==true){
            if(distance < finalDistance && distance < cCar->speed*0.4+cRay->baseDistanceSphere && distance > 0){
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
                actualObstacle = obstacleCar;
            }
        }
    }

    // modifica el vectorForce
    AvoidVibration(actualCar, actualObstacle, velocityVector, vectorForce);

    return vectorForce;
}


// obtiene un vector fuerza en caso de ir a colisionar contra un muro con bounding de plano
glm::vec2 SteeringBehaviours::WallAvoidance(Entity* actualCar, ManBoundingWall* manBoundingWall, ManBoundingOBB* manBoundingOBB, const glm::vec2& velocityVector) const{
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cRay = static_cast<CBoundingRay*>(actualCar->GetComponent(CompType::CompBoundingRay).get());
    glm::vec2 vectorForce = glm::vec2(0.0, 0.0);
    glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
    float distance = 99999.0;
    float finalDistance = 9998.0;
    glm::vec2 vectorForceAvoid;
    Entity* actualObstacle = nullptr;

    // colisiones con paredes sueltas
    for(const auto& obstacle : manBoundingWall->GetEntities()){
        if(CollisionRayPlane(actualCar, obstacle.get(), velocityVector, distance, vectorForceAvoid, target)==true){
            if(distance < finalDistance && distance < cCar->speed*0.4+cRay->baseDistancePlane && distance > 0){
                //std::cout << "Collisiona" << std::endl;
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
                actualObstacle = obstacle.get();
            }
        }
    }

    // colisiones con OBB
    for(const auto& obstacleOBB : manBoundingOBB->GetEntities()){
        if(CollisionRayPlane(actualCar, obstacleOBB.get(), velocityVector, distance, vectorForceAvoid, target)==true){
            if(distance < finalDistance && distance < cCar->speed*0.4+cRay->baseDistancePlane && distance > 0){
                //std::cout << "Collisiona" << std::endl;
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
                actualObstacle = obstacleOBB.get();
            }
        }
    }

    // modifica el vectorForce
    AvoidTrapCorner(actualCar, actualObstacle, velocityVector, target, vectorForce);

    return vectorForce;
}


// calcula el vector resultante para evadir un OBB en caso de colisionar
glm::vec2 SteeringBehaviours::ObstacleAvoidance(Entity* actualCar, ManBoundingOBB* manBoundingOBB, const std::vector<glm::vec2>& vectorRay) const{
    //auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cRay = static_cast<CBoundingRay*>(actualCar->GetComponent(CompType::CompBoundingRay).get());
    glm::vec2 vectorForce = glm::vec2(0.0, 0.0);
    glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
    float distance = 99999.0;
    float finalDistance = 9998.0;
    glm::vec2 vectorForceAvoid;

    // colisiones con OBB
    for(const auto& obstacleOBB : manBoundingOBB->GetEntities()){
        if(CollisionRayPlane(actualCar, obstacleOBB.get(), vectorRay[0], distance, vectorForceAvoid, target)){
            if(distance < finalDistance && distance < cRay->baseDistanceOBB && distance > 0){
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
            }
        }else if(CollisionRayPlane(actualCar, obstacleOBB.get(), vectorRay[1], distance, vectorForceAvoid, target)){
            if(distance < finalDistance && distance < cRay->baseDistanceOBB && distance > 0){
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
            }
        }else if(CollisionRayPlane(actualCar, obstacleOBB.get(), vectorRay[2], distance, vectorForceAvoid, target)){
            if(distance < finalDistance && distance < cRay->baseDistanceOBB && distance > 0){
                finalDistance = distance;
                vectorForce = vectorForceAvoid;
            }
        }
    }

    return vectorForce;
}


// comprueba si se colisiona frontalmente con una esfera
bool SteeringBehaviours::CollisionRaySphere(Entity* actualCar, Entity* object, const glm::vec2& velocityVector, float& distance, glm::vec2& vectorForceAvoid) const{
    //auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cTransformableCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cTransformableObject = static_cast<CTransformable*>(object->GetComponent(CompType::TransformableComp).get());
    auto cSphereObject = static_cast<CBoundingSphere*>(object->GetComponent(CompType::CompBoundingSphere).get());

    // Normalizar vector velocidad del coche
    float vectorDistance = sqrt(velocityVector.x*velocityVector.x + velocityVector.y*velocityVector.y);
    glm::vec3 vectorVelocityN = glm::vec3( velocityVector.x*(1/vectorDistance) , 0.0 ,velocityVector.y*(1/vectorDistance));

    IntersectData intersData = clPhysics->HandleCollisionsRayWithSpheres(*cTransformableCar, *cTransformableObject, *cSphereObject, vectorVelocityN);
    if(intersData.intersects == false) return false;
    if(CollisionFaceToFace(actualCar, object) == false) return false;
    distance = intersData.distance;
    vectorForceAvoid = Seek(actualCar, intersData.direction, velocityVector); // aunque ponga direction es el target

    return true;
}



// comprueba la colision de rayo con plano
bool SteeringBehaviours::CollisionRayPlane(Entity* actualCar, Entity* object, const glm::vec2& velocityVector, float& distance, glm::vec2& vectorForceAvoid, glm::vec3& target) const{
    auto cTransformableCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    // Normalizar vector velocidad del coche
    float vectorDistance = sqrt(velocityVector.x*velocityVector.x + velocityVector.y*velocityVector.y);
    glm::vec3 vectorVelocityN = glm::vec3( velocityVector.x*(1/vectorDistance) , 0.0 ,velocityVector.y*(1/vectorDistance)) ;
    IntersectData intersData{0, glm::vec3(0,0,0)};

    if(object->HasComponent(CompType::CompBoundingPlane) && !object->HasComponent(CompType::CompBoundingOBB)){
        auto cBoundPlaneObject = static_cast<CBoundingPlane*>(object->GetComponent(CompType::CompBoundingPlane).get());
        intersData = clPhysics->HandleCollisionsRayWithPlane(*cTransformableCar, vectorVelocityN, *cBoundPlaneObject);
    }else if(object->HasComponent(CompType::CompBoundingOBB)){
        auto cBoundOBBObject = static_cast<CBoundingOBB*>(object->GetComponent(CompType::CompBoundingOBB).get());
        intersData = clPhysics->HandleCollisionsRayWithOBB(*cTransformableCar, vectorVelocityN, *cBoundOBBObject);
    }

    if(intersData.intersects == false) return false;
    distance = intersData.distance;
    target = intersData.direction;
    vectorForceAvoid = Seek(actualCar, intersData.direction, velocityVector); // aunque ponga direction es el target
    return true;
}



// trata de evitar quedarse atrapado en la esquina devolviendo el vector al que debe moverse el coche
void SteeringBehaviours::AvoidTrapCorner(Entity* actualCar, Entity *actualObstacle, const glm::vec2& velocityVector, const glm::vec3& target, glm::vec2& vectorForce) const{
    auto cRay = static_cast<CBoundingRay*>(actualCar->GetComponent(CompType::CompBoundingRay).get());
    // evitar quedar atrapado en la esquina
    if(actualObstacle == nullptr){                     // no colisiona contra nada
        if(cRay->previousIdCollided != nullptr && cRay->iteratorSamePlane < cRay->maxItSamePlane){
            cRay->iteratorSamePlane++;
            vectorForce = Seek(actualCar, cRay->target, velocityVector);
            
        }else if(cRay->iteratorSamePlane >= cRay->maxItSamePlane){
            cRay->iteratorSamePlane = 0;
            cRay->previousIdCollided = nullptr; 
            cRay->prevToPrevIdCollided = nullptr; 
        }
    }else{                                          // colisiona contra un muro
        auto planeIdObstacle = static_cast<CId*>(actualObstacle->GetComponent(CompType::IdComp).get());                           
        if(planeIdObstacle != cRay->previousIdCollided && planeIdObstacle!=cRay->prevToPrevIdCollided){
            if(cRay->previousIdCollided != nullptr && cRay->iteratorSamePlane < cRay->maxItSamePlane){
                cRay->iteratorSamePlane++;
                vectorForce = Seek(actualCar, cRay->target, velocityVector);
            }else if(cRay->iteratorSamePlane >= cRay->maxItSamePlane || cRay->previousIdCollided == nullptr){
                cRay->iteratorSamePlane = 0;
                cRay->prevToPrevIdCollided = cRay->previousIdCollided;
                cRay->previousIdCollided = planeIdObstacle;
                cRay->target = target;
            }
        }else if(planeIdObstacle==cRay->prevToPrevIdCollided){ // si se queda atrapado en un bucle tratamos de salir fijando durante mas tiempo
            cRay->iteratorSamePlane++;
            vectorForce = Seek(actualCar, cRay->target, velocityVector);
            if(cRay->iteratorSamePlane>cRay->maxItDistinctPlane){
                cRay->previousIdCollided = nullptr; 
                cRay->prevToPrevIdCollided = nullptr; 
            }
        }else if(cRay->iteratorSamePlane == 0){
            cRay->target = target;
        }
    }
}


// trata de evitar la vibracion al evitar un coche
void SteeringBehaviours::AvoidVibration(Entity* actualCar, Entity *actualObstacle, const glm::vec2& velocityVector, glm::vec2& vectorForce) const{
    auto cRay = static_cast<CBoundingRay*>(actualCar->GetComponent(CompType::CompBoundingRay).get());
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
bool SteeringBehaviours::CollisionFaceToFace(Entity* actualCar, Entity *object) const{
    auto cTransCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    auto cTransObject = static_cast<CTransformable*>(object->GetComponent(CompType::TransformableComp).get());
    auto cObject = static_cast<CCar*>(object->GetComponent(CompType::CarComp).get());
    float carSpeed = cCar->speed;
    float objectSpeed = cObject->speed;

    if(carSpeed==0) carSpeed=0.1;
    if(objectSpeed==0) objectSpeed =0.1;

    float angleRotationCar = glm::radians(cTransCar->rotation.y);
    glm::vec2 carVector(cos(angleRotationCar) * carSpeed, sin(angleRotationCar) * carSpeed);

    float angleRotationObject = glm::radians(cTransObject->rotation.y);
    glm::vec2 objectVector(cos(angleRotationObject) * objectSpeed, sin(angleRotationObject) * objectSpeed);

    // se calcula el angulo entre los dos vectores
    float numerador = carVector.x*objectVector.x + carVector.y*objectVector.y;
    float denominador = sqrt(carVector.x*carVector.x + carVector.y*carVector.y) * sqrt(objectVector.x*objectVector.x + objectVector.y*objectVector.y);
    float angle = 0.0;
    if(denominador!=0)
        angle = acos(numerador/denominador);
    angle = glm::degrees(angle);

    if(angle<0) angle*=(-1);

    if(angle > 120.0)
        return true;
    else
        return false;
}



glm::vec2 SteeringBehaviours::CalculateVectorVelocity(CCar &cCar, CTransformable &transformableCar) const{
    if(cCar.speed==0) cCar.speed=0.1;
    float angleRotation = glm::radians(transformableCar.rotation.y);
    float posXSiguiente = transformableCar.position.x - cos(angleRotation) * cCar.speed;
    float posZSiguiente = transformableCar.position.z + sin(angleRotation) * cCar.speed;
    return glm::vec2(posXSiguiente - transformableCar.position.x , posZSiguiente - transformableCar.position.z );
}

std::vector<glm::vec2> SteeringBehaviours::CalculateRayVector(CCar &cCar, CTransformable &transformableCar) const{
    std::vector<glm::vec2> rays;
    if(cCar.speed==0) cCar.speed=0.1;
    float posXSiguienteRay1 = transformableCar.position.x - cos( glm::radians(transformableCar.rotation.y) ) * cCar.speed;
    float posZSiguienteRay1 = transformableCar.position.z + sin( glm::radians(transformableCar.rotation.y) ) * cCar.speed;
    float posXSiguienteRay2 = transformableCar.position.x - cos( glm::radians(Utils::GetAdjustedDegrees(transformableCar.rotation.y+40)) ) * cCar.speed;
    float posZSiguienteRay2 = transformableCar.position.z + sin( glm::radians(Utils::GetAdjustedDegrees(transformableCar.rotation.y+40)) ) * cCar.speed;
    float posXSiguienteRay3 = transformableCar.position.x - cos( glm::radians(Utils::GetAdjustedDegrees(transformableCar.rotation.y-40)) ) * cCar.speed;
    float posZSiguienteRay3 = transformableCar.position.z + sin( glm::radians(Utils::GetAdjustedDegrees(transformableCar.rotation.y-40)) ) * cCar.speed;

    rays.emplace_back(glm::vec2(posXSiguienteRay1 - transformableCar.position.x , posZSiguienteRay1 - transformableCar.position.z ));
    rays.emplace_back(glm::vec2(posXSiguienteRay2 - transformableCar.position.x , posZSiguienteRay2 - transformableCar.position.z ));
    rays.emplace_back(glm::vec2(posXSiguienteRay3 - transformableCar.position.x , posZSiguienteRay3 - transformableCar.position.z ));
    return rays;
}


// devuelve un angulo entre -180 y 180 para saber el sentido de giro
float SteeringBehaviours::CalculateAngle(const glm::vec2& originVec, const glm::vec2& destinyVec, float rotationY) const{
    // se calcula el angulo entre los dos vectores
    float numerador = originVec.x*destinyVec.x + originVec.y*destinyVec.y;
    float denominador = sqrt(originVec.x*originVec.x + originVec.y*originVec.y) * sqrt(destinyVec.x*destinyVec.x + destinyVec.y*destinyVec.y);
    float angle2 = 0.0;
    if(denominador!=0)
        angle2 = acos(numerador/denominador);
    angle2 = glm::degrees(angle2);

    // calcular si tiene que girar a la izquierda o derecha
    float valueAtan2 = glm::degrees( atan2(destinyVec.y,destinyVec.x) );
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
        if(rotationY>oppositeAngleAtan2 && rotationY<valueAtan2)
            finalAngle = angle2;
        else
            finalAngle = angle2 * (-1);
    }else{
        if(rotationY>valueAtan2 && rotationY<oppositeAngleAtan2)
            finalAngle = angle2 * (-1);
        else
            finalAngle = angle2;
    }

    return finalAngle;
}


// devuelve la posicion central del navmesh
glm::vec3 SteeringBehaviours::CalculateCenterNavMesh(Entity* actualCar, ManNavMesh* manNavMesh) const{
    auto cCurrNavMesh = static_cast<CCurrentNavMesh*>(actualCar->GetComponent(CompType::CurrentNavMeshComp).get());
    Entity* navMesh = nullptr;
    uint16_t i = 0;
    for(const auto& actualNM : manNavMesh->GetEntities()){
        if(cCurrNavMesh->currentNavMesh == i){
            navMesh = actualNM.get();
            break;
        }
        i++;
    }

    if(navMesh){
        auto cTransNM = static_cast<CTransformable*>(navMesh->GetComponent(CompType::TransformableComp).get());
        return cTransNM->position;
    }else{
        return glm::vec3(0.0);
    }
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