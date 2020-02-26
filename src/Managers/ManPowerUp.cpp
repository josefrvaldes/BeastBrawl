#include "ManPowerUp.h"

#include <iostream>
#include <Entities/PowerUp.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Components/CDimensions.h>
#include "../Components/CBoundingSphere.h"
#include "../Components/CType.h"

class Position;
using namespace std;


ManPowerUp::ManPowerUp() {
    SubscribeToEvents();
}


ManPowerUp::~ManPowerUp() {
    cout << "Llamando al destructor de ManPowerUps" << endl;
    entities.clear();
    entities.shrink_to_fit();
}


// Lugar en el que se crean los power ups

void ManPowerUp::CreatePowerUp(DataMap* d) {

    typeCPowerUp type = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);
    //cout << "el tipo de powerUp que recibimos es el: " << int(type) << endl;

    CTransformable *transforSalida = any_cast<CTransformable *>((*d)[CAR_EXIT_POSITION]);
    CDimensions *dimensionsCarSalida = any_cast<CDimensions *>((*d)[CAR_EXIT_DIMENSION]);
    CTransformable *transforPerse;
    if(d->count(CAR_FOLLOW_POSITION) > 0){
        transforPerse = any_cast<CTransformable *>((*d)[CAR_FOLLOW_POSITION]);
    }else
        transforPerse = nullptr;

    // calculamos la posicion en la que debe salir el powerUp:
    int medidaPowerUp = 25;
    float posX = 0, posZ = 0;

   float angleRotation = (transforSalida->rotation.y * 3.141592) / 180.0;
   if(type == typeCPowerUp::PudinDeFrambuesa){
        posX = transforSalida->position.x - cos(angleRotation)* (-1*((dimensionsCarSalida->width/2)+medidaPowerUp));
        posZ = transforSalida->position.z + sin(angleRotation)* (-1*((dimensionsCarSalida->depth/2)+medidaPowerUp));
   }else{
        posX = transforSalida->position.x - cos(angleRotation) * ((dimensionsCarSalida->width/2)+medidaPowerUp);
        posZ = transforSalida->position.z + sin(angleRotation) * ((dimensionsCarSalida->depth/2)+medidaPowerUp);    
   }

    vec3 positionPowerUp = vec3(posX,transforSalida->position.y+10,posZ);

    shared_ptr<PowerUp> powerUp = make_shared<PowerUp>(positionPowerUp, transforSalida->rotation, type, transforPerse);
    auto cTypePU = static_cast<CType*>(powerUp->GetComponent(CompType::TypeComp).get())->type;
    if( int(cTypePU) >= 0 && int(cTypePU) < 50 ){
        //std::cout << "Las dimensiones del coche son x:" << dimensionsCarSalida->width << " y:" << dimensionsCarSalida->height << " z:" << dimensionsCarSalida->depth << std::endl;
        
        entities.push_back(powerUp);

        auto renderFacadeManager = RenderFacadeManager::GetInstance();
        auto renderEngine = renderFacadeManager->GetRenderFacade();
        renderEngine->FacadeAddObject(powerUp.get());


        auto cDimensions = static_cast<CDimensions*>(powerUp->GetComponent(CompType::DimensionsComp).get());
        auto radioSphere = ((cDimensions->width/2)+(cDimensions->depth/2))/2;
        shared_ptr<CBoundingSphere> cBoundingSphere = make_shared<CBoundingSphere>(positionPowerUp, radioSphere);
        powerUp->AddComponent(cBoundingSphere);

        //renderEngine->FacadeAddSphereOnObject(powerUp.get());

        //Cuando creamos el powerUp, ponemos su tiempo inicial de inactivadad --> para no danyarnos a nostros mismos
        static_cast<CPowerUp*>(powerUp->GetComponent(CompType::PowerUpComp).get())->timeStart = system_clock::now();
    }else{
        cout << "el type powerUp es: " << int(cTypePU) << endl;
        cout << "ESTO NO DEBERIA DE PASAR NUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNCA JODER" << endl;
        cout << "SIIIIIIIIIIII TE PASA ESTOOOOOOOOOOOOOOOOOOOOOO HABLAAAAAAAAAAAAAAAR CON CARLOOOOOOOOOOOOOOOOOOOOOSSSSSSS" << endl;
    }
}


// TO-DO ELIMINARLO TODO AL MISMO TIEMPO ANTES DE RENDERIZAR 
void ManPowerUp::DeletePowerUp(DataMap* d){
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    for(long unsigned int i=0; i< entities.size(); ++i){
        if(entities[i] == any_cast<shared_ptr<Entity>>((*d)[POWER_UP])){
            renderEngine->DeleteEntity(entities[i].get());
            entities.erase(entities.begin()+i);
        }
    }
}


// TO-DO : tener una variable de control para eliminar todas las cosas de los arrays a la vez CUIDADO CON ESOOOO
void ManPowerUp::SubscribeToEvents() {
    // lo ejecuta el coche al tirar power up
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::PowerUp_Create,
        bind(&ManPowerUp::CreatePowerUp, this, placeholders::_1),
        "CreatePowerUp"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_ENTITY_POWERUP,
        bind(&ManPowerUp::DeletePowerUp, this, placeholders::_1),
        "DeletePowerUp"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_ENTITY_AI_POWERUP,
        bind(&ManPowerUp::DeletePowerUp, this, placeholders::_1),
        "DeletePowerUp"));
}

