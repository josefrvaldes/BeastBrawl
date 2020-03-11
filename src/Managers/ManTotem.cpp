#include "ManTotem.h"

#include <Entities/Totem.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacadeManager.h>
#include <Components/CDimensions.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CId.h>
#include "../Components/CTotem.h"
#include "../Components/COnline.h"
#include <include_json/include_json.hpp>
#include "ManNavMesh.h"
#include "../Game.h"

class Position;
using namespace std;
using json = nlohmann::json;
ManTotem::ManTotem(ManNavMesh *manNavMesh_) : manNavMesh{manNavMesh_} {
    // CREAMOS EL TOTEM
    ifstream i("data.json");
    json j = json::parse(i);

    double totemPosX = j["TOTEM"]["x"].get<double>();
    double totemPosY = j["TOTEM"]["y"].get<double>();
    double totemPosZ = j["TOTEM"]["z"].get<double>();
    vec3 posNewTotem = glm::vec3(totemPosX, totemPosY+5, totemPosZ);
    //std::cout << " LA POS X DEL TOTEM ES LA: " << totemPosX << std::endl;
    //CreateTotem(glm::vec3(-100.0, 20.0, -100.0));
    //CreateTotem(vec3(totemPosX, totemPosY-15, totemPosZ));
   // CreateTotem();

    auto currentNavMesh = manNavMesh->CalculateNavMesh(posNewTotem);
    if( currentNavMesh == -1 ){     // significa que la posicion es invalida
        // de momento lo reseteamos en el unico punto de Spawn
        // TODO:: RESETEARLO EN ALGUN PUNTO DE SPAWN DEL MAPA, EN ALGUN PUNTO DE SPAWN PREDETERMINADO PARA ESO
        ifstream i("data.json");
        json j = json::parse(i);
        posNewTotem = glm::vec3(j["TOTEM"]["x"].get<double>(),j["TOTEM"]["y"].get<double>()+5,j["TOTEM"]["z"].get<double>());
        currentNavMesh = manNavMesh->CalculateNavMesh(posNewTotem);
    }
    CreateTotem(posNewTotem);
    //TODO: Esto es peligroso [0] aunque sabemos que va a ir
    auto cId = static_cast<CId*>(entities[0]->GetComponent(CompType::IdComp).get());
    string nameEvent = "Partida/coger_totem";
    SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(0, posNewTotem, nameEvent, 0);

    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(entities[0]->GetComponent(CompType::CurrentNavMeshComp).get());
    cCurrentNavMesh->currentNavMesh = currentNavMesh;
    auto cTotem = static_cast<CTotem*>(entities[0]->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cout << "EL NAVMESH DEL TOTEM ES:::::::::::::::::::::::::::::::::::::::::::::::    " << cCurrentNavMesh->currentNavMesh << endl;

    SubscribeToEvents();

}

ManTotem::~ManTotem() {
    cout << "Llamando al destructor de ManTotem" << endl;
    entities.clear();
    entities.shrink_to_fit();
}

void ManTotem::Update(){

}


void ManTotem::CreateTotem() {
    if(entities.size() == 0){
        shared_ptr<Totem> totem = make_shared<Totem>();
        entities.push_back(totem);
    }
}

void ManTotem::CreateTotem(glm::vec3 _position) {
    if(entities.size() == 0){
        shared_ptr<Totem> totem = make_shared<Totem>(_position);
        string name = "Partida/totem";
        auto idComp = static_cast<CId*>(totem->GetComponent(CompType::IdComp).get());
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(idComp->id, _position, name, 1, 0);

        entities.push_back(totem); 
    }
}


// oculta el totem
void ManTotem::AppertainCar(DataMap* d){
   // lo unico qe hacemos es ponerlo a false, para que nadie lo pueda coger
   auto cTotem = static_cast<CTotem*>(entities[0]->GetComponent(CompType::TotemComp).get());
   cTotem->active = false;

    // escondemos el totem debajo del mapa cuando es multi hasta que se asigne
    if(State::States::INGAME_MULTI == Game::GetInstance()->GetState()->GetState()){
        auto cTrans = static_cast<CTransformable*>(entities[0]->GetComponent(CompType::TransformableComp).get());
        cTrans->position.y = -500.0;
    }
}


// lo pone en el mapa de nuevo
void ManTotem::ResetTotem(DataMap* d){
    //std::cout << "reseteamos posicion del totem" << std::endl;

    // calculamos la posicion donde queremos dejar el totem
    auto transfActualCar = any_cast<CTransformable*>((*d)[CAR_TRANSFORMABLE]); 
    glm::vec3 posNewTotem = glm::vec3(0.0f,10.0f,0.0f);
    float angleRotation = (transfActualCar->rotation.y * 3.141592) / 180.0;
    posNewTotem.x = transfActualCar->position.x - cos(angleRotation)*(-25);
    posNewTotem.z = transfActualCar->position.z + sin(angleRotation)*(-25);
    posNewTotem.y = transfActualCar->position.y + 15;

    // recorremos todos los NavMesh para saber si el totem ha caido en un sitio correcto
    //ManNavMesh manNavMesh;
    auto currentNavMesh = manNavMesh->CalculateNavMesh(posNewTotem);
    if( currentNavMesh == -1 ){     // significa que la posicion es invalida
        // de momento lo reseteamos en el unico punto de Spawn
        // TODO:: RESETEARLO EN ALGUN PUNTO DE SPAWN DEL MAPA, EN ALGUN PUNTO DE SPAWN PREDETERMINADO PARA ESO
        ifstream i("data.json");
        json j = json::parse(i);
        posNewTotem = glm::vec3(j["TOTEM"]["x"].get<double>(),j["TOTEM"]["y"].get<double>()+5,j["TOTEM"]["z"].get<double>());
        currentNavMesh = manNavMesh->CalculateNavMesh(posNewTotem);
    }

    if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_SINGLE){
        ActivateTotem(posNewTotem, currentNavMesh);
    }else if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_MULTI){
        auto cOnline = static_cast<COnline*>(any_cast<Entity*>((*d)[ACTUAL_CAR])->GetComponent(CompType::OnlineComp).get());
        systemOnline->SendLostTotem(cOnline->idClient, posNewTotem, currentNavMesh);
    }
}

void ManTotem::ActivateTotem(const glm::vec3 &position, int currentNavMesh){
    auto cTransformable = static_cast<CTransformable*>(entities[0]->GetComponent(CompType::TransformableComp).get());
    cTransformable->position = position;
    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(entities[0]->GetComponent(CompType::CurrentNavMeshComp).get());
    cCurrentNavMesh->currentNavMesh = currentNavMesh;
    auto cTotem = static_cast<CTotem*>(entities[0]->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
}



void ManTotem::SubscribeToEvents() {
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_PLAYER_TOTEM,
        bind(&ManTotem::AppertainCar, this, placeholders::_1),
        "AppertainCar"));
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::DROP_TOTEM,
        bind(&ManTotem::ResetTotem, this, placeholders::_1),
        "ResetTotem"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_SYNC_RECEIVED_TOTEM,
        bind(&ManTotem::SyncTotem, this, placeholders::_1),
        "SyncTotem"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_CATCH_TOTEM_RECEIVED,
        bind(&ManTotem::RecievedCatchTotem, this, placeholders::_1),
        "RecievedCatchTotem"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_LOST_TOTEM_RECEIVED,
        bind(&ManTotem::RecievedLostTotem, this, placeholders::_1),
        "RecievedLostTotem"));
}


void ManTotem::RecievedCatchTotem(DataMap* d){
    auto cTotem = static_cast<CTotem*>(entities[0]->GetComponent(CompType::TotemComp).get());
    cTotem->active = false;
}


void ManTotem::RecievedLostTotem(DataMap* d){
    ActivateTotem(any_cast<glm::vec3>((*d)[DataType::VEC3_POS]), any_cast<int>((*d)[DataType::ID]));
}


void ManTotem::SyncTotem(DataMap* d){
    if(any_cast<bool>((*d)[CAR_WITHOUT_TOTEM])){
        // Esto puede generar un problema al estar las posiciones modificandose respecto a la del otro
        // auto cTransTotem = static_cast<CTransformable*>(GetEntities()[0]->GetComponent(CompType::TransformableComp).get());
        // cTransTotem->position = any_cast<glm::vec3>((*d)[VEC3_POS]);
    }else{
        // aqui no deberia haber totem, comprobar si hay totem y eliminarlo en caso de que exista
    }

}