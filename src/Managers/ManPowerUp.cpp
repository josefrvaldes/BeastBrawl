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
 
 
 #include "ManPowerUp.h"

#include <Components/CDimensions.h>
#include <Components/CIDOnline.h>
#include <Components/CId.h>
#include <Entities/PowerUp.h>
#include <Entities/Car.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacadeManager.h>
#include <Game.h>
#include <iostream>
#include "../Components/CBoundingSphere.h"
#include "../Components/COnline.h"
#include "../Components/CRemovableObject.h"
#include "../Components/CType.h"
#include "../Systems/PhysicsPowerUp.h"

class Position;
using namespace std;

// TODO: No es un to-do, pero quiero indicar que los powerUps tienen una reserva de 50
ManPowerUp::ManPowerUp(shared_ptr<ManCar> manCars_) : manCars{manCars_} {
    entities.reserve(50);
    physicsPowerUp = unique_ptr<PhysicsPowerUp>();  // Creamos sistemas
    SubscribeToEvents();
}

ManPowerUp::~ManPowerUp() {
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
    powerUp->AddComponent(cidOnline); 
    MaterializePowerUp(powerUp, typePU);
}

void ManPowerUp::CreatePowerUp(DataMap *d) {
    typeCPowerUp type = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);

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
    bool isOnline = any_cast<bool>((*d)[IS_ONLINE]);
    int medidaPowerUp = 15;
    if(isOnline)
        medidaPowerUp = 45;
    float posX = 0, posZ = 0;
    vec3 positionPowerUp;
    float angleRotation = (transforSalida->rotation.y * glm::pi<float>() / 180.0);
    if (type == typeCPowerUp::PudinDeFrambuesa) {
        posX = transforSalida->position.x - cos(angleRotation) * (-1 * ((dimensionsCarSalida->width / 2) + medidaPowerUp));
        posZ = transforSalida->position.z + sin(angleRotation) * (-1 * ((dimensionsCarSalida->depth / 2) + medidaPowerUp));
        positionPowerUp = vec3(posX, transforSalida->position.y + 10, posZ);
    } else {
        posX = transforSalida->position.x - cos(angleRotation) * ((dimensionsCarSalida->width / 2) + medidaPowerUp);
        posZ = transforSalida->position.z + sin(angleRotation) * ((dimensionsCarSalida->depth / 2) + medidaPowerUp);
        positionPowerUp = vec3(posX, transforSalida->position.y + 10, posZ);
    }

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
            // enviamos al server que ese PU se ha lanzado
            systemOnline->SendThrowPU(powerUp, idToPursue);
        }
        MaterializePowerUp(powerUp, type);

    }
}


void ManPowerUp::MaterializePowerUp(shared_ptr<PowerUp> powerUp , typeCPowerUp type) {
    entities.push_back(powerUp);

    auto cTransformable = static_cast<CTransformable *>(powerUp->GetComponent(CompType::TransformableComp).get());

    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngine->FacadeAddObject(powerUp.get());

    //auto cDimensions = static_cast<CDimensions *>(powerUp->GetComponent(CompType::DimensionsComp).get());
    //auto radioSphere = ((cDimensions->width / 2) + (cDimensions->depth / 2)) / 2;
    //cout <<"EL RADIO DE CREACION ES DEEEEEEEEEEEEEEEEE: " << radioSphere << endl;
    shared_ptr<CBoundingSphere> cBoundingSphere;
    if (type == typeCPowerUp::PudinDeFrambuesa) {
        cBoundingSphere = make_shared<CBoundingSphere>(cTransformable->position, 7.5, 6.0);
    }else{
        cBoundingSphere = make_shared<CBoundingSphere>(cTransformable->position, 7.5, 0.0);
    }

    powerUp->AddComponent(cBoundingSphere);

    //renderEngine->FacadeAddSphereOnObject(powerUp.get());

    //Cuando creamos el powerUp, ponemos su tiempo inicial de inactivadad --> para no danyarnos a nostros mismos
    static_cast<CPowerUp *>(powerUp->GetComponent(CompType::PowerUpComp).get())->timeStart = system_clock::now();


    // Sonido de powerUp
    auto cId = static_cast<CId*>(powerUp->GetComponent(CompType::IdComp).get());
    auto cType = static_cast<CPowerUp*>(powerUp->GetComponent(CompType::PowerUpComp).get());
    if (cId && cType) {
        switch(int(cType->typePowerUp)) {
            case int(typeCPowerUp::PudinDeFrambuesa): {
                string nameEvent = "PowerUp/pudin";
                SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(cId->id, cTransformable->position, nameEvent, 1, 1);
                break;
            }
            case int(typeCPowerUp::MelonMolon): {
                string nameEvent = "PowerUp/melonmolon";
                SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(cId->id, cTransformable->position, nameEvent, 1, 1);
                nameEvent = "PowerUp/choque_powerup";
                SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(cId->id, cTransformable->position, nameEvent, 0, 0);
                break;
            }
            case int(typeCPowerUp::TeleBanana): {
                string nameEvent = "PowerUp/telebanana";
                SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(cId->id, cTransformable->position, nameEvent, 1, 1);
                nameEvent = "PowerUp/choque_powerup";
                SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(cId->id, cTransformable->position, nameEvent, 0, 0);
                break;
            }
            default:
                break;
        }
    }
}


void ManPowerUp::Update() {
    //cout << "- ACTUALIZO LOS PU" << endl;
    // Update de las fisicas de los PU (melon molon, telebanana, superMegaNitro)
    UpdatePhysics();
    //cout << "- - - HE ACTUALIZADO LAS FISICAS DE LOS PU" << endl;
    // recorremos todos los PU, y borramos los correspondientes
    DeletePowerUps();
    //cout << "- - - HE BORRADO LOS PU QUE HAY QUE BORRAR" << endl;

}

void ManPowerUp::UpdatePhysics(){
    for (auto &actualPowerUp : this->GetEntities()) {  // actualizamos las fisicas de los powerUps
        physicsPowerUp->update(actualPowerUp.get());
    }
}

void ManPowerUp::DeletePowerUps(){
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    for (long unsigned int i = 0; i < entities.size(); ++i) {
        auto cRemovableObj = static_cast<CRemovableObject *>(entities[i].get()->GetComponent(CompType::RemovableObjectComp).get());
        auto cId = static_cast<CId*>(entities[i].get()->GetComponent(CompType::IdComp).get());
        auto cTypePU = static_cast<CPowerUp*>(entities[i].get()->GetComponent(CompType::PowerUpComp).get());
        auto cTransformable = static_cast<CTransformable*>(entities[i].get()->GetComponent(CompType::TransformableComp).get());
        if (cId && cTypePU && cTransformable && cRemovableObj->destroy) {

            //Antes de borrarlo paro su sonido
                if (cTypePU->typePowerUp == typeCPowerUp::MelonMolon) {
                    shared_ptr<DataMap> data = make_shared<DataMap>();
                    (*data)[ID] = cId->id;
                    EventManager::GetInstance().AddEventMulti(Event{EventType::STOP_SOUND_MM, data});
                } else if (cTypePU->typePowerUp == typeCPowerUp::TeleBanana) {
                    shared_ptr<DataMap> data = make_shared<DataMap>();
                    (*data)[ID] = cId->id;
                    EventManager::GetInstance().AddEventMulti(Event{EventType::STOP_SOUND_TB, data});
                }

            //Eliminamos el powerup y lanzamos el evento para las particlas
            //Comentar si la queremos para todas o simplemente para banana y melon
            //VEC3_POS
            shared_ptr<DataMap> data = make_shared<DataMap>();
            (*data)[VEC3_POS] = cTransformable->position;
            (*data)[TYPE] = cTypePU->typePowerUp;
            EventManager::GetInstance().AddEventMulti(Event{EventType::CREATE_PARTICLES_COLLISION_POWERUP, data});


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

}
