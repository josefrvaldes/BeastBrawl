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
 
 
 #include "Collisions.h"
#include <Entities/Car.h>
#include <Entities/CarHuman.h>
#include <Entities/CarAI.h>
#include <Components/CTotem.h>
#include <Components/CShield.h>

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


/*void Collisions::IntersectPlayerPowerUps(Car* carPlayer, ManPowerUp* manPowerUps, ManNavMesh* manNavMesh){
    for(auto& actualPowerUp : manPowerUps->GetEntities()){                                                            // SI HACE DANYO
        if(Intersects(carPlayer, actualPowerUp.get())){   //TRUE
            // debemos eliminar el powerUp y hacer danyo al jugador
            shared_ptr<DataMap> dataCollisonCarPowerUp = make_shared<DataMap>();                                                                       
            (*dataCollisonCarPowerUp)[POWER_UP] = actualPowerUp;              // nos guardamos el puntero para eliminar el powerUp                           
            EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_ENTITY_POWERUP, dataCollisonCarPowerUp}); 
            // comprobamos si el coche tenia escudo y el totem.. ya que debe de soltarlo
            auto cShield = static_cast<CShield*>(carPlayer->GetComponent(CompType::ShieldComp).get());
            if(cShield->activePowerUp==false && static_cast<CTotem*>(carPlayer->GetComponent(CompType::TotemComp).get())->active){  // TRUE
                auto dataTransformableCar = static_cast<CTransformable*>(carPlayer->GetComponent(CompType::TransformableComp).get());
                shared_ptr<DataMap> dataTransfCar = make_shared<DataMap>();                                                                       
                     
                Entity* carEntity = carPlayer;                                                               
                (*dataTransfCar)[CAR_TRANSFORMABLE] = dataTransformableCar;  
                (*dataTransfCar)[ACTUAL_CAR] = carEntity; 
                (*dataTransfCar)[MAN_NAVMESH] = manNavMesh;
                EventManager::GetInstance().AddEventMulti(Event{EventType::DROP_TOTEM, dataTransfCar});  
            }
        }
    }
}*/

/*void Collisions::IntersectsCarsPowerUps(ManCar* manCars, ManPowerUp* manPowerUps, ManNavMesh* manNavMesh){
    for(const auto& actualCar : manCars->GetEntities()){
        if(actualCar.get() != manCars->GetCar().get()){
            for(auto& actualPowerUp : manPowerUps->GetEntities()){                                                               // SI HACE DANYO
                if(Intersects(actualCar.get(), actualPowerUp.get())){   //TRUE
                    // debemos eliminar el powerUp y hacer danyo al jugador
                    shared_ptr<DataMap> dataCollisonCarPowerUp = make_shared<DataMap>();                                                                       

                    (*dataCollisonCarPowerUp)[POWER_UP] = actualPowerUp;              // nos guardamos el puntero para eliminar el powerUp
                    (*dataCollisonCarPowerUp)[CAR_AI] = actualCar.get();              // nos guardamos el puntero al coche                              
                    EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_CAR_POWERUP, dataCollisonCarPowerUp}); 
                    // comprobamos si el coche tenia escudo y el totem.. ya que debe de soltarlo
                    auto cShield = static_cast<CShield*>(actualCar.get()->GetComponent(CompType::ShieldComp).get());
                    if(cShield->activePowerUp==false && static_cast<CTotem*>(actualCar.get()->GetComponent(CompType::TotemComp).get())->active){  // TRUE
                        auto dataTransformableCar = static_cast<CTransformable*>(actualCar.get()->GetComponent(CompType::TransformableComp).get());
                        shared_ptr<DataMap> dataTransfCar = make_shared<DataMap>();                                                                    
                        (*dataTransfCar)[CAR_TRANSFORMABLE] = dataTransformableCar;  
                        (*dataTransfCar)[ACTUAL_CAR] = actualCar.get(); 
                        (*dataTransfCar)[MAN_NAVMESH] = manNavMesh;
                        EventManager::GetInstance().AddEventMulti(Event{EventType::DROP_TOTEM, dataTransfCar});  
                    } 
                }
            }
        }
    }
}*/

/**
 * @Deprecated
 * Este método se borrará en próximas iteraciones porque ya no se usa.
 * Ahora se usa IntersectCarsTotem
 */
void Collisions::IntersectPlayerTotem(Car* carPlayer, ManTotem* manTotem){

    for(auto& currentTotem : manTotem->GetEntities()){   // SI HACE DANYO
        if(Intersects(carPlayer, currentTotem.get())){   //TRUE
            // debemos coger el TOTEM
            shared_ptr<DataMap> dataCollisionTotem = make_shared<DataMap>();                                                                         
            (*dataCollisionTotem)[TOTEM] = currentTotem;              // nos guardamos el puntero para eliminar el powerUp                                             
            EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_PLAYER_TOTEM, dataCollisionTotem});
        }
    }
}


void Collisions::IntersectCarsTotem(ManCar* manCars, ManTotem* manTotem){
    for(const auto& currentCar : manCars->GetEntities()){
        for(shared_ptr<Entity> currentTotem : manTotem->GetEntities()){ // SI HACE DANYO
            if(Intersects(currentCar.get(), currentTotem.get())){   //TRUE
                // debemos coger el TOTEM
                shared_ptr<DataMap> dataCollisionTotem = make_shared<DataMap>();                                                                         

                (*dataCollisionTotem)[TOTEM] = currentTotem;              // nos guardamos el puntero para eliminar el powerUp  
                (*dataCollisionTotem)[ACTUAL_CAR] = currentCar.get();                                           
                EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_PLAYER_TOTEM, dataCollisionTotem});
                return; // con este return nos aseguramos de que solo 1 car pueda coger el totem a la vez, aunque colisionen varios
            }
        }
    }
}





void Collisions::IntersectPlayerBoxPowerUp(Car* carPlayer, ManBoxPowerUp* manBoxPowerUp){
    auto cPowerUpCar = static_cast<CPowerUp*>(carPlayer->GetComponent(CompType::PowerUpComp).get());                            // debemos acceder al componente PowerUpComp                                                                    // solo si no tenemos powerUp podemos coger uno
    for(auto& actualBoxPowerUp: manBoxPowerUp->GetEntities()){                                                 // recorremos los powerUps
        auto cBoxPowerUp = static_cast<CBoxPowerUp*>(actualBoxPowerUp->GetComponent(CompType::BoxPowerUpComp).get()); // debemos acceder al componente BoxPowerUp
        if(cBoxPowerUp->active == true){ 
            if(cPowerUpCar->typePowerUp == typeCPowerUp::None){                                                                                   // Vemos si efectivamente esta activo o no, para poder cogerlo
                if( Intersects(carPlayer, actualBoxPowerUp.get()) ){                                                            // Finalmente comprobamos las colisiones entre el coche y el powerUp
                    shared_ptr<DataMap> dataCollisonCarBoxPowerUp = make_shared<DataMap>();                                                                          // Mejor definirlo en el .h
                    (*dataCollisonCarBoxPowerUp)[BOX_POWER_UP_COMPONENT] = cBoxPowerUp;                                                  // necesitamos el componente
                    (*dataCollisonCarBoxPowerUp)[ACTUAL_BOX] = actualBoxPowerUp; 
                    // Lanzaremos este evento cuando colisionemos con una caja y no tengamos ya PowerUp       
                    EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_BOX_POWERUP, dataCollisonCarBoxPowerUp});                      // llamamos al evento --- COMO ODIO QUE SE LLAME ADD Y NO TARGET
                }
            }else{
                if( Intersects(carPlayer, actualBoxPowerUp.get()) ){                                                            
                    shared_ptr<DataMap> dataCollisonCarBoxPowerUp = make_shared<DataMap>();                                                                          // Mejor definirlo en el .h
                                                                        
                    (*dataCollisonCarBoxPowerUp)[BOX_POWER_UP_COMPONENT] = cBoxPowerUp;                                                 
                    (*dataCollisonCarBoxPowerUp)[ACTUAL_BOX] = actualBoxPowerUp; 
                    // Lanzaremos este evento cuando colisionemos con una caja y tengamos ya PowerUp                                        
                    EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_BOX_WITH_POWERUP, dataCollisonCarBoxPowerUp});                     
                }    
            }
        }
    }
}


void Collisions::IntersectCarsBoxPowerUp(ManCar* manCars, ManBoxPowerUp* manBoxPowerUp){
    for(const auto& actualCar : manCars->GetEntities()){
        if(actualCar.get() != manCars->GetCar().get()){
            auto cPowerUpCar = static_cast<CPowerUp*>(actualCar.get()->GetComponent(CompType::PowerUpComp).get());                                                                                              
            for(auto& actualBoxPowerUp: manBoxPowerUp->GetEntities()){                                                 
                auto cBoxPowerUp = static_cast<CBoxPowerUp*>(actualBoxPowerUp->GetComponent(CompType::BoxPowerUpComp).get());
                // Vemos si efectivamente esta activo o no, para poder cogerlo
                if(cBoxPowerUp->active == true){
                    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){                                                                                   
                        if( Intersects(actualCar.get(), actualBoxPowerUp.get()) ){                                                            
                            shared_ptr<DataMap> dataCollisonCarBoxPowerUp = make_shared<DataMap>();                                                                          // Mejor definirlo en el .h

                            (*dataCollisonCarBoxPowerUp)[BOX_POWER_UP_COMPONENT] = cBoxPowerUp;                                                 
                            (*dataCollisonCarBoxPowerUp)[ACTUAL_BOX] = actualBoxPowerUp;
                            (*dataCollisonCarBoxPowerUp)[ACTUAL_CAR] = actualCar.get();   
                            // Lanzaremos este evento cuando colisionemos con una caja y no tengamos ya PowerUp                                             
                            EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_AI_BOX_POWERUP, dataCollisonCarBoxPowerUp});                     
                        }
                    }else{
                        if( Intersects(actualCar.get(), actualBoxPowerUp.get()) ){                                                            
                            shared_ptr<DataMap> dataCollisonCarBoxPowerUp = make_shared<DataMap>();                                                                          // Mejor definirlo en el .h                                                                        
                            (*dataCollisonCarBoxPowerUp)[BOX_POWER_UP_COMPONENT] = cBoxPowerUp;                                                 
                            (*dataCollisonCarBoxPowerUp)[ACTUAL_BOX] = actualBoxPowerUp; 
                            // Lanzaremos este evento cuando colisionemos con una caja y tengamos ya PowerUp                                          
                            EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_BOX_WITH_POWERUP, dataCollisonCarBoxPowerUp});                     
                        }          
                    }
                }
            }
        }
    }
}