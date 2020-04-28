
#include "SystemVision.h"

#include "../Components/CBrainAI.h"
#include "../Components/CBoundingOBB.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../Components/CBrainAI.h"
#include "../Components/CTotem.h"

#include "../Entities/Entity.h"
#include "../Entities/CarAI.h"
// #include "../Entities/Car.h"
// #include "../Entities/CarHuman.h"

#include "../Managers/ManCar.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManBoundingOBB.h"


SystemVision::SystemVision(){
    clPhysics = make_unique<CLPhysics>();
}

void SystemVision::AddManager(Manager &m) {
    managers.push_back(&m);
}


void SystemVision::update(CarAI* actualCar){
    auto cBrainAI = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());

    cBrainAI->CleanVisionRange();

    SaveCarInVision(actualCar, cBrainAI, static_cast<ManCar*>(managers[0]), static_cast<ManBoundingOBB*>(managers[7]));
    SaveBoxPowerUpInVision(actualCar, cBrainAI, static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManBoundingOBB*>(managers[7]));
    SaveTotemInVision(actualCar, cBrainAI, static_cast<ManTotem*>(managers[3]), static_cast<ManBoundingOBB*>(managers[7]));

    // auto cBrainAII = static_cast<CBrainAI*>((static_cast<ManCar*>(managers[0]))->GetCar()->GetComponent(CompType::BrainAIComp).get());
    // cBrainAII->CleanVisionRange();
    // SaveCarInVision((static_cast<ManCar*>(managers[0]))->GetCar().get(), cBrainAII, static_cast<ManCar*>(managers[0]), static_cast<ManBoundingOBB*>(managers[7]));
    // SaveBoxPowerUpInVision((static_cast<ManCar*>(managers[0]))->GetCar().get(), cBrainAII, static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManBoundingOBB*>(managers[7]));
    // SaveTotemInVision((static_cast<ManCar*>(managers[0]))->GetCar().get(), cBrainAII, static_cast<ManTotem*>(managers[3]), static_cast<ManBoundingOBB*>(managers[7]));
    //cout << "NumCars: " << cBrainAII->carInVision.size() << "\n";
    //cout << "NumBox: " << cBrainAII->boxInVision.size() << "\n";
    //if(cBrainAII->totemInVision!=nullptr)
    //    cout << "Totem: 1\n";
}



void SystemVision::SaveCarInVision(Entity* actualCar, CBrainAI* cBrainAI, ManCar* manCar, ManBoundingOBB* manBoundingOBB){
    for(const auto& carVision : manCar->GetEntities()){
        auto cTransCar = static_cast<CTransformable*>(carVision->GetComponent(CompType::TransformableComp).get());
        if(actualCar!=carVision.get() && EntityInVisionRange(actualCar, cTransCar->position, cBrainAI->fov)){
            auto cTransCar = static_cast<CTransformable*>(carVision->GetComponent(CompType::TransformableComp).get());
            if(!IsOcludedOrFar(actualCar, cBrainAI, cTransCar->position, manBoundingOBB))
                cBrainAI->carInVision.emplace_back(carVision.get());
        }
    }
}

void SystemVision::SaveBoxPowerUpInVision(Entity* actualCar, CBrainAI* cBrainAI, ManBoxPowerUp* manBoxPowerUp, ManBoundingOBB* manBoundingOBB){
    for(const auto& boxVision : manBoxPowerUp->GetEntities()){
        auto cTransBox = static_cast<CTransformable*>(boxVision->GetComponent(CompType::TransformableComp).get());
        auto cBoxPowerUp = static_cast<CBoxPowerUp*>(boxVision->GetComponent(CompType::BoxPowerUpComp).get());
        if(!cBoxPowerUp->active)
            continue;
        if(EntityInVisionRange(actualCar, cTransBox->position, cBrainAI->fov)){
            if(!IsOcludedOrFar(actualCar, cBrainAI, cTransBox->position, manBoundingOBB))
                cBrainAI->boxInVision.emplace_back(boxVision.get());
        }
    }
}

void SystemVision::SaveTotemInVision(Entity* actualCar, CBrainAI* cBrainAI, ManTotem* manTotem, ManBoundingOBB* manBoundingOBB){
    auto cTotem = static_cast<CTotem*>(manTotem->GetEntities()[0]->GetComponent(CompType::TotemComp).get());
    if(cTotem->active == true){
        auto cTransTotem = static_cast<CTransformable*>(manTotem->GetEntities()[0]->GetComponent(CompType::TransformableComp).get());
        if(EntityInVisionRange(actualCar, cTransTotem->position, cBrainAI->fov)){
            if(!IsOcludedOrFar(actualCar, cBrainAI, cTransTotem->position, manBoundingOBB))
                cBrainAI->totemInVision = manTotem->GetEntities()[0].get();
        }
    }
}



// comprueba lanzando un rayo si se encuentra ocluido o si se encuentra muy lejos
bool SystemVision::IsOcludedOrFar(Entity* actualCar, CBrainAI* cBrainAI, const glm::vec3& posEntity, ManBoundingOBB* manBoundingOBB) const{
    auto cTransCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar*>(actualCar->GetComponent(CompType::CarComp).get());
    float distanceToTarget = glm::distance(cTransCar->position, posEntity);
    IntersectData intersData{0, glm::vec3(0,0,0)};

    // comprobamos si se encuentra muy lejos para verlo
    if(cBrainAI->distanceVision < distanceToTarget)
        return true;    // se encuentra lejos

    // genera el vector normal de movimiento
    glm::vec3 velocityVector = CalculateVectorVelocity(*cCar, *cTransCar);
    float vectorDistance = sqrt(velocityVector.x*velocityVector.x + velocityVector.z*velocityVector.z);
    glm::vec3 vectorVelocityN = glm::vec3( velocityVector.x*(1/vectorDistance) , 0.0 ,velocityVector.z*(1/vectorDistance));

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


glm::vec3 SystemVision::CalculateVectorVelocity(CCar &cCar, CTransformable &transformableCar) const{
    if(cCar.speed==0) cCar.speed=0.1;
    float angleRotation = (transformableCar.rotation.y * PI) / 180.0;
    float posXSiguiente = transformableCar.position.x - cos(angleRotation) * cCar.speed;
    float posZSiguiente = transformableCar.position.z + sin(angleRotation) * cCar.speed;
    return glm::vec3(posXSiguiente - transformableCar.position.x , 0.0, posZSiguiente - transformableCar.position.z);
}



// comprueba si se encuentra en su campo de vision
bool SystemVision::EntityInVisionRange(Entity* actualCar, const glm::vec3& posEntity, int rangeVision) const{
    float seeCar = false;
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
    float offset = valueAtan2 - cTransformableActual->rotation.y;
    if(offset<=rangeVision && offset>=-rangeVision){
        seeCar = true;
    }
/*
    if (cTransformableActual->rotation.y - rangeVision >= 0 && cTransformableActual->rotation.y + rangeVision < 360) {
        if (cTransformableActual->rotation.y - rangeVision < valueAtan2 && cTransformableActual->rotation.y + rangeVision > valueAtan2) {
            seeCar = true;
        }
    } else {  // coge el angulo 0 de por medio
        float rotMin = cTransformableActual->rotation.y - rangeVision;
        float rotMax = cTransformableActual->rotation.y + rangeVision;
        if (cTransformableActual->rotation.y - rangeVision < 0)
            rotMin += 360;
        if (cTransformableActual->rotation.y + rangeVision >= 360)
            rotMax -= 360;
        if (rotMin < valueAtan2 || rotMax > valueAtan2) {
            seeCar = true;
        }
    }*/
    return seeCar;
}



/* Nota: dado que en nuestro juego es raro que se descarte por distancia
    se va a discriminar por esta despues de comprobar si se encuentra en vision.
    En caso de otro juego seria mucho mas optimo primero descartar por distancia/octree/navmesh */