#include "ManPowerUp.h"

#include <Components/CDimensions.h>
#include <Components/CIDOnline.h>
#include <Entities/PowerUp.h>
#include <Entities/Car.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Game.h>
#include <iostream>
#include "../Components/CBoundingSphere.h"
#include "../Components/COnline.h"
#include "../Components/CRemovableObject.h"
#include "../Components/CType.h"

class Position;
using namespace std;

// TODO: No es un to-do, pero quiero indicar que los powerUps tienen una reserva de 50
ManPowerUp::ManPowerUp(shared_ptr<ManCar> manCars_) : manCars{manCars_} {
    entities.reserve(50);
    SubscribeToEvents();
}

ManPowerUp::~ManPowerUp() {
    cout << "Llamando al destructor de ManPowerUps" << endl;
    entities.clear();
    entities.shrink_to_fit();
}

// Lugar en el que se crean los power ups

void ManPowerUp::NewPowerUpReceivedFromServer(DataMap *d) {
    int8_t auxTypePU = any_cast<int8_t>((*d)[TYPE_POWER_UP]);
    typeCPowerUp typePU = static_cast<typeCPowerUp>(auxTypePU);
    vec3 position = any_cast<vec3>((*d)[VEC3_POS]);
    vec3 rotation = any_cast<vec3>((*d)[VEC3_ROT]);
    uint16_t idPUOnline = any_cast<uint16_t>((*d)[ID_ONLINE]);
    
    // si hay alguien a quien perseguir...
    CTransformable *transforPerse = nullptr;
    if(d->count(ID_PURSUE) > 0) {
        uint16_t idToPursue = any_cast<uint16_t>((*d)[ID_PURSUE]);
        for(const auto& car : manCars->GetEntities()) {
            COnline *cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
            if(cOnline->idClient == idToPursue) {
                transforPerse = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
                break;
            }
        }
    }
    shared_ptr<PowerUp> powerUp = make_shared<PowerUp>(position, rotation, typePU, transforPerse);
    shared_ptr<CIDOnline> cidOnline = make_shared<CIDOnline>(idPUOnline, true);
    cidOnline->collided = true;
    powerUp->AddComponent(cidOnline); 
    MaterializePowerUp(powerUp);
}

void ManPowerUp::CreatePowerUp(DataMap *d) {
    typeCPowerUp type = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);
    cout << "el tipo de powerUp que recibimos es el: " << int(type) << endl;

    CTransformable *transforSalida = any_cast<CTransformable *>((*d)[CAR_EXIT_POSITION]);
    CDimensions *dimensionsCarSalida = any_cast<CDimensions *>((*d)[CAR_EXIT_DIMENSION]);
    CTransformable *transforPerse = nullptr;
    Car *carPerse = nullptr;
    if (d->count(CAR_FOLLOW) > 0) {
        carPerse = any_cast<Car *>((*d)[CAR_FOLLOW]);
        transforPerse = static_cast<CTransformable*>(carPerse->GetComponent(CompType::TransformableComp).get());
    } else
        transforPerse = nullptr;

    // calculamos la posicion en la que debe salir el powerUp:
    int medidaPowerUp = 25;
    float posX = 0, posZ = 0;

   float angleRotation = (transforSalida->rotation.y * glm::pi<float>() / 180.0);
    if (type == typeCPowerUp::PudinDeFrambuesa) {
        posX = transforSalida->position.x - cos(angleRotation) * (-1 * ((dimensionsCarSalida->width / 2) + medidaPowerUp));
        posZ = transforSalida->position.z + sin(angleRotation) * (-1 * ((dimensionsCarSalida->depth / 2) + medidaPowerUp));
    } else {
        posX = transforSalida->position.x - cos(angleRotation) * ((dimensionsCarSalida->width / 2) + medidaPowerUp);
        posZ = transforSalida->position.z + sin(angleRotation) * ((dimensionsCarSalida->depth / 2) + medidaPowerUp);
    }

    vec3 positionPowerUp = vec3(posX, transforSalida->position.y + 10, posZ);

    shared_ptr<PowerUp> powerUp = make_shared<PowerUp>(positionPowerUp, transforSalida->rotation, type, transforPerse);

    // ojo con esta linea, no borrar porque es necesaria aunque parezca que no lo es
    auto cTypePU = static_cast<CType *>(powerUp->GetComponent(CompType::TypeComp).get())->type;

    if (int(cTypePU) >= 0 && int(cTypePU) < 50) {
        
        // si no estamos en online, simplemente materializamos el PU
        if(systemOnline != nullptr) {
            // si es una telebanana, necesitamos saber el id del coche al que perseguir
            uint16_t idToPursue = -1;
            // si es telebanana, y por tanto hay alguien a quien perseguir...
            if(carPerse != nullptr) {
                COnline *cOnline = static_cast<COnline*>(carPerse->GetComponent(CompType::OnlineComp).get());
                // apuntamos el id de ese coche al que perseguir para enviarlo al online
                idToPursue = cOnline->idClient;
            }
            systemOnline->SendThrowPU(powerUp, idToPursue);
        }
        MaterializePowerUp(powerUp);

    } else {
        cout << "el type powerUp es: " << int(type) << endl;
        cout << "ESTO NO DEBERIA DE PASAR NUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNCA JODER" << endl;
        cout << "SIIIIIIIIIIII TE PASA ESTOOOOOOOOOOOOOOOOOOOOOO HABLAAAAAAAAAAAAAAAR CON CARLOOOOOOOOOOOOOOOOOOOOOSSSSSSS" << endl;
    }
}

void ManPowerUp::MaterializePowerUp(shared_ptr<PowerUp> powerUp) {
    entities.push_back(powerUp);

    auto cTransformable = static_cast<CTransformable *>(powerUp->GetComponent(CompType::TransformableComp).get());

    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngine->FacadeAddObject(powerUp.get());

    auto cDimensions = static_cast<CDimensions *>(powerUp->GetComponent(CompType::DimensionsComp).get());
    auto radioSphere = ((cDimensions->width / 2) + (cDimensions->depth / 2)) / 2;
    shared_ptr<CBoundingSphere> cBoundingSphere = make_shared<CBoundingSphere>(cTransformable->position, radioSphere);
    powerUp->AddComponent(cBoundingSphere);

    //renderEngine->FacadeAddSphereOnObject(powerUp.get());

    //Cuando creamos el powerUp, ponemos su tiempo inicial de inactivadad --> para no danyarnos a nostros mismos
    static_cast<CPowerUp *>(powerUp->GetComponent(CompType::PowerUpComp).get())->timeStart = system_clock::now();
}

// TO-DO ELIMINARLO TODO AL MISMO TIEMPO ANTES DE RENDERIZAR
//void ManPowerUp::DeletePowerUp(DataMap* d){
//    auto renderFacadeManager = RenderFacadeManager::GetInstance();
//    auto renderEngine = renderFacadeManager->GetRenderFacade();
//
//    for(long unsigned int i=0; i< entities.size(); ++i){
//        if(entities[i].get() == any_cast<Entity*>((*d)[POWER_UP])){
//            renderEngine->DeleteEntity(entities[i].get());
//            entities.erase(entities.begin()+i);
//        }
//    }
//}

void ManPowerUp::Update() {
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    for (long unsigned int i = 0; i < entities.size(); ++i) {
        auto cRemovableObj = static_cast<CRemovableObject *>(entities[i].get()->GetComponent(CompType::RemovableObjectComp).get());
        if (cRemovableObj->destroy) {
            renderEngine->DeleteEntity(entities[i].get());
            entities.erase(entities.begin() + i);
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
        EventType::NEW_THROW_PU_RECEIVED,
        bind(&ManPowerUp::NewPowerUpReceivedFromServer, this, placeholders::_1),
        "CreatePowerUp"));

    

    //EventManager::GetInstance().SubscribeMulti(Listener(
    //    EventType::DELETE_POWERUP,
    //    bind(&ManPowerUp::DeletePowerUp, this, placeholders::_1),
    //    "DeletePowerUp"));
}
