#include "Collisions.h"
#include "../Entities/PowerUp.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Components/CTotem.h"
#include "../Components/CShield.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"

Collisions::Collisions(){

}

bool Collisions::Intersects(Entity* entity1,Entity* entity2){

    /*
    return (a.minX <= b.maxX && a.maxX >= b.minX) &&
         (a.minY <= b.maxY && a.maxY >= b.minY) &&
         (a.minZ <= b.maxZ && a.maxZ >= b.minZ);

    */
    auto cTransformableE1 = static_cast<CTransformable*>(entity1->GetComponent(CompType::TransformableComp).get());
    auto cDimensionsE1 = static_cast<CDimensions*>(entity1->GetComponent(CompType::DimensionsComp).get());

    auto cTransformableE2 = static_cast<CTransformable*>(entity2->GetComponent(CompType::TransformableComp).get());
    auto cDimensionsE2 = static_cast<CDimensions*>(entity2->GetComponent(CompType::DimensionsComp).get());

    float e1MinX = cTransformableE1->position.x - cDimensionsE1->width/2;
    float e1MinY = cTransformableE1->position.y - cDimensionsE1->height/2;
    float e1MinZ = cTransformableE1->position.z - cDimensionsE1->depth/2;
    float e1MaxX = cTransformableE1->position.x + cDimensionsE1->width/2;
    float e1MaxY = cTransformableE1->position.y + cDimensionsE1->height/2;
    float e1MaxZ = cTransformableE1->position.z + cDimensionsE1->depth/2;

    float e2MinX = cTransformableE2->position.x - cDimensionsE2->width/2;
    float e2MinY = cTransformableE2->position.y - cDimensionsE2->height/2;
    float e2MinZ = cTransformableE2->position.z - cDimensionsE2->depth/2;
    float e2MaxX = cTransformableE2->position.x + cDimensionsE2->width/2;
    float e2MaxY = cTransformableE2->position.y + cDimensionsE2->height/2;
    float e2MaxZ = cTransformableE2->position.z + cDimensionsE2->depth/2;

    if(e1MinX <= e2MaxX && e1MaxX >= e2MinX &&
       e1MinY <= e2MaxY && e1MaxY >= e2MinY &&
       e1MinZ <= e2MaxZ && e1MaxZ >= e2MinZ){
           //cout << "chocan\n";
           return true;
    }

    return false;
}


void Collisions::IntersectPlayerPowerUps(ManCar* manCars, ManPowerUp* manPowerUps){

    for(shared_ptr<Entity> actualPowerUp : manPowerUps->GetEntities()){
        auto cPowerUp = static_cast<CPowerUp*>(actualPowerUp->GetComponent(CompType::PowerUpComp).get());
        if(cPowerUp->effectActive == true){                                                                 // SI HACE DANYO
            if(Intersects(manCars->GetCar().get(), actualPowerUp.get())){   //TRUE
                // debemos eliminar el powerUp y hacer danyo al jugador
                shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                DataMap dataCollisonCarPowerUp;                                                                           
                dataCollisonCarPowerUp["PowerUp"] = actualPowerUp;              // nos guardamos el puntero para eliminar el powerUp                           
                eventManager->AddEventMulti(Event{EventType::COLLISION_ENTITY_POWERUP, dataCollisonCarPowerUp}); 

                // comprobamos si el coche tenia escudo y el totem.. ya que debe de soltarlo
                auto cShield = static_cast<CShield*>(manCars->GetCar().get()->GetComponent(CompType::ShieldComp).get());
                if(cShield->activePowerUp==false && static_cast<CTotem*>(manCars->GetCar().get()->GetComponent(CompType::TotemComp).get())->active){  // TRUE
                    auto dataTransformableCar = static_cast<CTransformable*>(manCars->GetCar().get()->GetComponent(CompType::TransformableComp).get());
                    DataMap dataTransfCar;                                                                    
                    dataTransfCar["TransfCarPos"] = dataTransformableCar;  
                    eventManager->AddEventMulti(Event{EventType::DROP_TOTEM, dataTransfCar});  
                }
            }
        }
    }
}




void Collisions::IntersectsCarsPowerUps(ManCar* manCars, ManPowerUp* manPowerUps){

    for(shared_ptr<Entity> actualCar : manCars->GetEntitiesAI()){   
        for(shared_ptr<Entity> actualPowerUp : manPowerUps->GetEntities()){
            auto cPowerUp = static_cast<CPowerUp*>(actualPowerUp->GetComponent(CompType::PowerUpComp).get());
            if(cPowerUp->effectActive == true){                                                                 // SI HACE DANYO
                if(Intersects(actualCar.get(), actualPowerUp.get())){   //TRUE
                    // debemos eliminar el powerUp y hacer danyo al jugador
                    shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                    DataMap dataCollisonCarPowerUp;                                                                           
                    dataCollisonCarPowerUp["PowerUp"] = actualPowerUp;              // nos guardamos el puntero para eliminar el powerUp
                    dataCollisonCarPowerUp["carAI"] = actualCar.get();              // nos guardamos el puntero al coche                              
                    eventManager->AddEventMulti(Event{EventType::COLLISION_ENTITY_AI_POWERUP, dataCollisonCarPowerUp}); 
                    std::cout << "Soy la IA y me han PEGADOOOOO" << std::endl;
                    // comprobamos si el coche tenia escudo y el totem.. ya que debe de soltarlo
                    //auto cShield = static_cast<CShield*>(actualCar.get()->GetComponent(CompType::ShieldComp).get());
                    //if(cShield->activePowerUp==false && static_cast<CTotem*>(actualCar.get()->GetComponent(CompType::TotemComp).get())->active){  // TRUE
                    //    auto dataTransformableCar = static_cast<CTransformable*>(actualCar.get()->GetComponent(CompType::TransformableComp).get());
                    //    DataMap dataTransfCar;                                                                    
                    //    dataTransfCar["TransfCarPos"] = dataTransformableCar;  
                    //    eventManager->AddEventMulti(Event{EventType::DROP_TOTEM, dataTransfCar});  
                    //}
                }
            }
        }
    }

}