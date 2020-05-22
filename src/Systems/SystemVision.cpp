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
 
 
 
#include "SystemVision.h"


#include "../Managers/ManCar.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManBoundingOBB.h"
#include "../Managers/ManBoundingWall.h"
#include "../Managers/ManBoundingGround.h"

#include "../Entities/Entity.h"
#include "../Entities/CarAI.h"
#include "../Entities/CarHuman.h"

#include "../Components/CBrainAI.h"
#include "../Components/CBoundingOBB.h"
#include "../Components/CBoundingPlane.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../Components/CBrainAI.h"
#include "../Components/CTotem.h"

#include "../Facade/Render/RenderFacadeManager.h"



SystemVision::SystemVision(CLPhysics *clPhysics_) : clPhysics{clPhysics_} {
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
}




void SystemVision::SaveCarInVision(Entity* actualCar, CBrainAI* cBrainAI, ManCar* manCar, ManBoundingWall* manBoundingWall, 
                                    ManBoundingOBB* manBoundingOBB, ManBoundingGround* manBoundingGround){
    for(const auto& carVision : manCar->GetEntities()){
        auto cTransCar = static_cast<CTransformable*>(carVision->GetComponent(CompType::TransformableComp).get());
        if(actualCar!=carVision.get() && EntityInVisionRange(actualCar, cTransCar->position, cBrainAI->fov)){
            auto cTransCar = static_cast<CTransformable*>(carVision->GetComponent(CompType::TransformableComp).get());
            if(!IsFar(actualCar, cBrainAI, cTransCar->position) && !CollideWithOBB(actualCar, cTransCar->position, manBoundingOBB)
            && !CollideWithGround(actualCar, cTransCar->position, manBoundingGround) && !CollideWithWall(actualCar, cTransCar->position, manBoundingWall))
                cBrainAI->carInVision.emplace_back(carVision.get());
        }
    }
}

void SystemVision::SaveBoxPowerUpInVision(Entity* actualCar, CBrainAI* cBrainAI, ManBoxPowerUp* manBoxPowerUp, ManBoundingWall* manBoundingWall, 
                                        ManBoundingOBB* manBoundingOBB, ManBoundingGround* manBoundingGround){
    for(const auto& boxVision : manBoxPowerUp->GetEntities()){
        auto cTransBox = static_cast<CTransformable*>(boxVision->GetComponent(CompType::TransformableComp).get());
        auto cBoxPowerUp = static_cast<CBoxPowerUp*>(boxVision->GetComponent(CompType::BoxPowerUpComp).get());
        if(!cBoxPowerUp->active)
            continue;
        if(EntityInVisionRange(actualCar, cTransBox->position, cBrainAI->fov)){
            if(!IsFar(actualCar, cBrainAI, cTransBox->position) && !CollideWithOBB(actualCar, cTransBox->position, manBoundingOBB)
            && !CollideWithGround(actualCar, cTransBox->position, manBoundingGround) && !CollideWithWall(actualCar, cTransBox->position, manBoundingWall))
                cBrainAI->boxInVision.emplace_back(boxVision.get());
        }
    }
}

void SystemVision::SaveTotemInVision(Entity* actualCar, CBrainAI* cBrainAI, ManTotem* manTotem, ManBoundingWall* manBoundingWall, 
                                    ManBoundingOBB* manBoundingOBB, ManBoundingGround* manBoundingGround){
    auto cTotem = static_cast<CTotem*>(manTotem->GetEntities()[0]->GetComponent(CompType::TotemComp).get());
    if(cTotem->active == true){
        auto cTransTotem = static_cast<CTransformable*>(manTotem->GetEntities()[0]->GetComponent(CompType::TransformableComp).get());
        if(EntityInVisionRange(actualCar, cTransTotem->position, cBrainAI->fov)){
            if(!IsFar(actualCar, cBrainAI, cTransTotem->position) && !CollideWithOBB(actualCar, cTransTotem->position, manBoundingOBB)
            && !CollideWithGround(actualCar, cTransTotem->position, manBoundingGround) && !CollideWithWall(actualCar, cTransTotem->position, manBoundingWall))
                cBrainAI->totemInVision = manTotem->GetEntities()[0].get();
        }
    }
}



// comprueba lanzando un rayo si se encuentra muy lejos
bool SystemVision::IsFar(Entity* actualCar, CBrainAI* cBrainAI, const glm::vec3& posEntity) const{
    auto cTransCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float distanceToTarget = glm::distance(cTransCar->position, posEntity);

    if(cBrainAI->distanceVision < distanceToTarget)
        return true;    // se encuentra lejos
    return false;
}



// comprueba si se encuentra en su campo de vision
bool SystemVision::EntityInVisionRange(Entity* actualCar, const glm::vec3& posEntity, int visionRange) const{
    // calcular un desplazamiento para ser en tercera persona
    auto cTransformableActual = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float posXActualCar = cTransformableActual->position.x + 40 * cos(glm::radians(cTransformableActual->rotation.y));
    float posZActualCar = cTransformableActual->position.z - 40 * sin(glm::radians(cTransformableActual->rotation.y));

    // vector between actualCar and otherCar
    float vetorToEntityX = (posEntity.x - posXActualCar);
    float vetorToEntityZ = (posEntity.z - posZActualCar);

    // calculate position rotated of otherCar atan2
    float valueAtan2 = glm::degrees( atan2(vetorToEntityZ, vetorToEntityX) );
    valueAtan2 = 180.0 - valueAtan2;  // se le restan ya que el eje empieza en el lado contrario
    if (valueAtan2 < 0)
        valueAtan2 += 360;


    //compare with actualCar actualRotation
    float rotMin = cTransformableActual->rotation.y - visionRange;
    if (rotMin < 0) rotMin += 360;
    float rotMax = cTransformableActual->rotation.y + visionRange;
    if (rotMax >= 360) rotMax -= 360;

    if(rotMin<rotMax){
        if(rotMin<valueAtan2 && rotMax>valueAtan2)
            return true;
    }else{
        if(rotMin<valueAtan2 || rotMax>valueAtan2)
            return true;
    }

    return false;
}



bool SystemVision::EntityInVisionHuman(const glm::vec3& posEntity) const{
    float visionRange = renderEngine->FacadeGetFovActualCamera();
    glm::vec3 targetCamera = renderEngine->FacadeGetTargetActualCamera();
    glm::vec3 posCamera = renderEngine->FacadeGetPositionActualCamera();
    targetCamera.z = targetCamera.z * (-1); // la Z esta invertida en el motor
    posCamera.z = posCamera.z * (-1);

    float vectorToTargetX = targetCamera.x - posCamera.x;
    float vectorToTargetZ = targetCamera.z - posCamera.z;
    float valueCentralDegree = glm::degrees( atan2(vectorToTargetZ, vectorToTargetX) );
    if (valueCentralDegree < 0) valueCentralDegree += 360;

    float vectorToEntityX = posEntity.x - posCamera.x;
    float vectorToEntityZ = posEntity.z - posCamera.z;
    float valueDegreeEntity = glm::degrees( atan2(vectorToEntityZ, vectorToEntityX) );
    if (valueDegreeEntity < 0) valueDegreeEntity += 360;


    //compare with actualCar actualRotation
    float rotMin = valueCentralDegree - visionRange;
    if (rotMin < 0) rotMin += 360;
    float rotMax = valueCentralDegree + visionRange;
    if (rotMax >= 360) rotMax -= 360;

    if(rotMin<rotMax){
        if(rotMin<valueDegreeEntity && rotMax>valueDegreeEntity)
            return true;
    }else{
        if(rotMin<valueDegreeEntity || rotMax>valueDegreeEntity)
            return true;
    }
    return false;
}


// devuelve los grados que separan una entidad de tu eje central de vision
float SystemVision::DegreesFromCenter(Entity* actualCar, Entity* targetCar) const{
    auto cTransCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cTransTargetCar = static_cast<CTransformable*>(targetCar->GetComponent(CompType::TransformableComp).get());

    // calcular vector al target
    float vetorTargetX = (cTransTargetCar->position.x - cTransCar->position.x );
    float vetorTargetZ = (cTransTargetCar->position.z - cTransCar->position.z);
    
    // calcular un desplazamiento para ser en tercera persona
    float posXCar = cTransCar->position.x + 38.9 * cos(glm::radians(cTransCar->rotation.y));
    float posZCar = cTransCar->position.z - 38.9 * sin(glm::radians(cTransCar->rotation.y));
    // se calcula el vector entre el siguiente punto y y el punto actual del coche
    float xCoche = (posXCar - cTransCar->position.x );
    float zCoche = (posZCar - cTransCar->position.z);

    // se calcula el angulo entre los dos vectores
    float numerador = xCoche*vetorTargetX + zCoche*vetorTargetZ;
    float denominador = sqrt(xCoche*xCoche + zCoche*zCoche) * sqrt(vetorTargetX*vetorTargetX + vetorTargetZ*vetorTargetZ);
    float angle = 0.0;
    if(denominador!=0)
        angle = acos(numerador/denominador);
    angle = glm::degrees(angle);

    angle = (angle - 180.0) * (-1);
    if(angle<0) angle+=360;
    

    return angle;
}

// calcula el coche mas centrado al que lanzarle la telebanana
Car* SystemVision::CalculateCentredCar(Entity* actualCar, int visionRange, ManCar* manCar, 
        ManBoundingWall* manBoundingWall, ManBoundingOBB* manBoundingOBB, ManBoundingGround* manBoundingGround) const{
    Entity* objectiveCar = nullptr;
    float minDegrees = visionRange+1.0;
    float degreesFromCenter;
    //cout << "Corre platano\n";
    for(const auto& car : manCar->GetEntities()){
        if(car.get()!=actualCar){
            degreesFromCenter = DegreesFromCenter(actualCar, car.get());
            //cout << degreesFromCenter << "\n";
            if(minDegrees>degreesFromCenter){
                auto cTransTarget = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
                if(!CollideWithWall(actualCar,cTransTarget->position,manBoundingWall) && !CollideWithOBB(actualCar,cTransTarget->position,manBoundingOBB) 
                && !CollideWithGround(actualCar,cTransTarget->position,manBoundingGround)){
                    minDegrees = degreesFromCenter;
                    objectiveCar = car.get();
                    //cout << "Objetivo encontrado\n";

                }
            }
        }
    }

    return static_cast<Car*>(objectiveCar);
}


// comprueba si los muros ocluyen el objetivo
bool SystemVision::CollideWithWall(Entity* actualCar, const glm::vec3& posTarget, ManBoundingWall* manBoundingWall) const{
    auto cTransCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float distanceToTarget = glm::distance(cTransCar->position, posTarget);
    IntersectData intersData{0, glm::vec3(0,0,0)};

    // genera el vector normal de movimiento
    glm::vec3 targetVector = posTarget - cTransCar->position;
    glm::vec3 vectorVelocityN = glm::normalize(targetVector);

    // comprueba las colisiones
    for(const auto& obstacle : manBoundingWall->GetEntities()){
        auto cBoundPlane = static_cast<CBoundingPlane*>(obstacle->GetComponent(CompType::CompBoundingPlane).get());
        intersData = clPhysics->HandleCollisionsRayWithPlane(*cTransCar, vectorVelocityN, *cBoundPlane);

        if(intersData.intersects == false) 
            continue;       // no se ocluye por este muro
        if(intersData.distance < distanceToTarget)
            return true;    // se encuentra ocluido
    }
    return false;
}

// comprueba si los OBBs ocluyen el objetivo
bool SystemVision::CollideWithOBB(Entity* actualCar, const glm::vec3& posTarget, ManBoundingOBB* manBoundingOBB) const{
    auto cTransCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float distanceToTarget = glm::distance(cTransCar->position, posTarget);
    IntersectData intersData{0, glm::vec3(0,0,0)};

    // genera el vector normal de movimiento
    glm::vec3 targetVector = posTarget - cTransCar->position;
    glm::vec3 vectorVelocityN = glm::normalize(targetVector);

    // comprueba las colisiones
    for(const auto& obstacle : manBoundingOBB->GetEntities()){
        auto cBoundOBBObject = static_cast<CBoundingOBB*>(obstacle->GetComponent(CompType::CompBoundingOBB).get());
        intersData = clPhysics->HandleCollisionsRayWithOBB(*cTransCar, vectorVelocityN, *cBoundOBBObject);

        if(intersData.intersects == false) 
            continue;       // no se ocluye por este muro
        if(intersData.distance < distanceToTarget)
            return true;    // se encuentra ocluido
    }
    return false;
}


// comprueba si los suelos ocluyen el objetivo
bool SystemVision::CollideWithGround(Entity* actualCar, const glm::vec3& posTarget, ManBoundingGround* manBoundingGround) const{
    auto cTransCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    CTransformable cTransCar2 = *cTransCar;
    cTransCar2.position.y += 20;
    glm::vec3 posTarget2 = glm::vec3(posTarget.x, posTarget.y+20, posTarget.z);
    float distanceToTarget = glm::distance(cTransCar2.position, posTarget2);
    IntersectData intersData{0, glm::vec3(0,0,0)};

    if(cTransCar2.position.y+30 < posTarget2.y){    // se comprueba para ser en la direccion correcta
        glm::vec3 aux = cTransCar2.position;
        cTransCar2.position = posTarget2;
        posTarget2 = aux;
    }

    // genera el vector normal de movimiento
    glm::vec3 targetVector = posTarget2 - cTransCar2.position;
    glm::vec3 vectorVelocityN = glm::normalize(targetVector);

    // comprueba las colisiones
    for(const auto& obstacle : manBoundingGround->GetEntities()){
        auto cBoundPlane = static_cast<CBoundingPlane*>(obstacle->GetComponent(CompType::CompBoundingPlane).get());
        intersData = clPhysics->HandleCollisionsRayWithPlane(cTransCar2, vectorVelocityN, *cBoundPlane);

        if(intersData.intersects == false) 
            continue;       // no se ocluye por este muro
        if(intersData.distance < distanceToTarget)
            return true;    // se encuentra ocluido
    }
    return false;
}



/* Nota: dado que en nuestro juego es raro que se descarte por distancia
    se va a discriminar por esta despues de comprobar si se encuentra en vision.
    En caso de otro juego seria mucho mas optimo primero descartar por distancia/octree/navmesh */