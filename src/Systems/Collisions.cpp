#include "Collisions.h"
#include "../Entities/PowerUp.h"
#include "../Entities/BoxPowerUp.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Entities/Totem.h"
#include "../Components/CTotem.h"
#include "../Components/CShield.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"

Collisions::Collisions(){

}


// comprueba la interseccion entre dos entidades
bool Collisions::Intersects(Entity* entity1,Entity* entity2){
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

    if( (e1MinX <= e2MaxX && e1MaxX >= e2MinX) &&
        (e1MinY <= e2MaxY && e1MaxY >= e2MinY) &&
        (e1MinZ <= e2MaxZ && e1MaxZ >= e2MinZ) ){
           //cout << "chocan\n";
           return true;
    }


    return false;
}


void Collisions::IntersectPlayerPowerUps(Car* carPlayer, ManPowerUp* manPowerUps, ManNavMesh* manNavMesh){
    for(shared_ptr<Entity> actualPowerUp : manPowerUps->GetEntities()){
        //auto cPowerUp = static_cast<CPowerUp*>(actualPowerUp->GetComponent(CompType::PowerUpComp).get());
        //if(cPowerUp->effectActive == true){                                                                 // SI HACE DANYO
            if(Intersects(carPlayer, actualPowerUp.get())){   //TRUE
                // debemos eliminar el powerUp y hacer danyo al jugador
                DataMap dataCollisonCarPowerUp;                                                                           
                dataCollisonCarPowerUp["PowerUp"] = actualPowerUp;              // nos guardamos el puntero para eliminar el powerUp                           
                EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_ENTITY_POWERUP, dataCollisonCarPowerUp}); 

                // comprobamos si el coche tenia escudo y el totem.. ya que debe de soltarlo
                auto cShield = static_cast<CShield*>(carPlayer->GetComponent(CompType::ShieldComp).get());
                if(cShield->activePowerUp==false && static_cast<CTotem*>(carPlayer->GetComponent(CompType::TotemComp).get())->active){  // TRUE
                    auto dataTransformableCar = static_cast<CTransformable*>(carPlayer->GetComponent(CompType::TransformableComp).get());
                    DataMap dataTransfCar;     
                    Entity* carEntity = carPlayer;                                                               
                    dataTransfCar["TransfCarPos"] = dataTransformableCar;  
                    dataTransfCar["car"] = carEntity; 
                    dataTransfCar["manNavMesh"] = manNavMesh;
                    EventManager::GetInstance().AddEventMulti(Event{EventType::DROP_TOTEM, dataTransfCar});  
                }
            }
        //}
    }
}

void Collisions::IntersectsCarsPowerUps(ManCar* manCars, ManPowerUp* manPowerUps, ManNavMesh* manNavMesh){
    for(shared_ptr<Entity> actualCar : manCars->GetEntitiesAI()){   
        for(shared_ptr<Entity> actualPowerUp : manPowerUps->GetEntities()){
            //auto cPowerUp = static_cast<CPowerUp*>(actualPowerUp->GetComponent(CompType::PowerUpComp).get());
            //if(cPowerUp->effectActive == true){                                                                 // SI HACE DANYO
                if(Intersects(actualCar.get(), actualPowerUp.get())){   //TRUE
                    // debemos eliminar el powerUp y hacer danyo al jugador
                    DataMap dataCollisonCarPowerUp;                                                                           
                    dataCollisonCarPowerUp["PowerUp"] = actualPowerUp;              // nos guardamos el puntero para eliminar el powerUp
                    dataCollisonCarPowerUp["carAI"] = actualCar.get();              // nos guardamos el puntero al coche                              
                    EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_ENTITY_AI_POWERUP, dataCollisonCarPowerUp}); 

                    // comprobamos si el coche tenia escudo y el totem.. ya que debe de soltarlo
                    auto cShield = static_cast<CShield*>(actualCar.get()->GetComponent(CompType::ShieldComp).get());
                    if(cShield->activePowerUp==false && static_cast<CTotem*>(actualCar.get()->GetComponent(CompType::TotemComp).get())->active){  // TRUE
                        auto dataTransformableCar = static_cast<CTransformable*>(actualCar.get()->GetComponent(CompType::TransformableComp).get());
                        DataMap dataTransfCar;                                                                    
                        dataTransfCar["TransfCarPos"] = dataTransformableCar;  
                        dataTransfCar["car"] = actualCar.get(); 
                        dataTransfCar["manNavMesh"] = manNavMesh;
                        EventManager::GetInstance().AddEventMulti(Event{EventType::DROP_TOTEM, dataTransfCar});  
                    } 
                }
            //}
        }
    }
}


void Collisions::IntersectPlayerTotem(Car* carPlayer, ManTotem* manTotem){

    for(shared_ptr<Entity> actualTotem : manTotem->GetEntities()){                                                       // SI HACE DANYO
        if(Intersects(carPlayer, actualTotem.get())){   //TRUE
            // debemos coger el TOTEM
            DataMap dataCollisonTotem;                                                                           
            dataCollisonTotem["Totem"] = actualTotem;              // nos guardamos el puntero para eliminar el powerUp                                             
            EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_PLAYER_TOTEM, dataCollisonTotem});
        }
    }
}


void Collisions::IntersectCarsTotem(ManCar* manCars, ManTotem* manTotem){

    for(shared_ptr<Entity> actualCar : manCars->GetEntitiesAI()){   
        for(shared_ptr<Entity> actualTotem : manTotem->GetEntities()){                                                       // SI HACE DANYO
            if(Intersects(actualCar.get(), actualTotem.get())){   //TRUE
                // debemos coger el TOTEM
                DataMap dataCollisonTotem;                                                                           
                dataCollisonTotem["Totem"] = actualTotem;              // nos guardamos el puntero para eliminar el powerUp  
                dataCollisonTotem["actualCar"] = actualCar.get();                                           
                EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_AI_TOTEM, dataCollisonTotem});
            }
        }
    }
}




void Collisions::IntersectPlayerBoxPowerUp(Car* carPlayer, ManBoxPowerUp* manBoxPowerUp){
    auto cPowerUpCar = static_cast<CPowerUp*>(carPlayer->GetComponent(CompType::PowerUpComp).get());                            // debemos acceder al componente PowerUpComp
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){                                                                         // solo si no tenemos powerUp podemos coger uno
        for(shared_ptr<Entity> actualBoxPowerUp: manBoxPowerUp->GetEntities()){                                                 // recorremos los powerUps
            auto cBoxPowerUp = static_cast<CBoxPowerUp*>(actualBoxPowerUp.get()->GetComponent(CompType::BoxPowerUpComp).get()); // debemos acceder al componente BoxPowerUp
            if(cBoxPowerUp->active == true){                                                                                    // Vemos si efectivamente esta activo o no, para poder cogerlo
                if( Intersects(carPlayer, actualBoxPowerUp.get()) ){                                                            // Finalmente comprobamos las colisiones entre el coche y el powerUp
                    DataMap dataCollisonCarBoxPowerUp;                                                                          // Mejor definirlo en el .h
                    dataCollisonCarBoxPowerUp["BoxPowerUpComp"] = cBoxPowerUp;                                                  // necesitamos el componente
                    dataCollisonCarBoxPowerUp["actualBox"] = actualBoxPowerUp;                                                  // y tambien la caja actual (para eliminarla de irrlicht)
                    EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_BOX_POWERUP, dataCollisonCarBoxPowerUp});                      // llamamos al evento --- COMO ODIO QUE SE LLAME ADD Y NO TARGET
                }
            }
        }
    }
}


void Collisions::IntersectCarsBoxPowerUp(ManCar* manCars, ManBoxPowerUp* manBoxPowerUp){
    for(shared_ptr<Entity> actualCar : manCars->GetEntitiesAI()){   
        auto cPowerUpCar = static_cast<CPowerUp*>(actualCar.get()->GetComponent(CompType::PowerUpComp).get());                        
        if(cPowerUpCar->typePowerUp == typeCPowerUp::None){                                                                         
            for(shared_ptr<Entity> actualBoxPowerUp: manBoxPowerUp->GetEntities()){                                                 
                auto cBoxPowerUp = static_cast<CBoxPowerUp*>(actualBoxPowerUp.get()->GetComponent(CompType::BoxPowerUpComp).get());

                // Vemos si efectivamente esta activo o no, para poder cogerlo
                if(cBoxPowerUp->active == true){                                                                                   
                    if( Intersects(actualCar.get(), actualBoxPowerUp.get()) ){                                                            
                        DataMap dataCollisonCarBoxPowerUp;                                                                          
                        dataCollisonCarBoxPowerUp["BoxPowerUpComp"] = cBoxPowerUp;                                                 
                        dataCollisonCarBoxPowerUp["actualBox"] = actualBoxPowerUp;
                        dataCollisonCarBoxPowerUp["actualCar"] = actualCar.get();                                              
                        EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_AI_BOX_POWERUP, dataCollisonCarBoxPowerUp});                     
                    }
                }
            }
        }
    }
}