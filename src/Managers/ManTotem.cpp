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
#include "../Systems/PhysicsTotem.h"

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
    //auto cId = static_cast<CId*>(entities[0]->GetComponent(CompType::IdComp).get());
    string nameEvent = "Partida/coger_totem";
    SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(0, posNewTotem, nameEvent, 0);

    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(entities[0]->GetComponent(CompType::CurrentNavMeshComp).get());
    cCurrentNavMesh->currentNavMesh = currentNavMesh;
    auto cTotem = static_cast<CTotem*>(entities[0]->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    //cout << "EL NAVMESH DEL TOTEM ES:::::::::::::::::::::::::::::::::::::::::::::::    " << cCurrentNavMesh->currentNavMesh << endl;

    SubscribeToEvents();
    physicsTotem = make_unique<PhysicsTotem>();
}

ManTotem::~ManTotem() {
    entities.clear();
    entities.shrink_to_fit();
}

void ManTotem::Update() {
    // Update de las fisicas de los PU (melon molon, telebanana, superMegaNitro)
    UpdatePhysics();

}

void ManTotem::UpdatePhysics(){
    physicsTotem->update(GetEntities()[0].get());
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
    glm::vec3 posNewTotem = vec3(0.0f,10.0f,0.0f);
    float angleRotation = (transfActualCar->rotation.y * M_PI) / 180.0;
    posNewTotem.x = transfActualCar->position.x - cos(angleRotation)*(-25);
    posNewTotem.z = transfActualCar->position.z + sin(angleRotation)*(-25);
    posNewTotem.y = transfActualCar->position.y + 15;


    auto cTotem = static_cast<CTotem*>(entities[0]->GetComponent(CompType::TotemComp).get());
    // recorremos todos los NavMesh para saber si el totem ha caido en un sitio correcto
    //ManNavMesh manNavMesh;
    int rotTotem_Y = 0;
    auto currentNavMesh = manNavMesh->CalculateNavMesh(posNewTotem);
    if( currentNavMesh == -1 ){     // significa que la posicion es invalida
        // de momento lo reseteamos en el unico punto de Spawn
        // TODO:: RESETEARLO EN ALGUN PUNTO DE SPAWN DEL MAPA, EN ALGUN PUNTO DE SPAWN PREDETERMINADO PARA ESO
        ifstream i("data.json");
        json j = json::parse(i);
        posNewTotem = glm::vec3(j["TOTEM"]["x"].get<double>(),j["TOTEM"]["y"].get<double>()+5,j["TOTEM"]["z"].get<double>());
        currentNavMesh = manNavMesh->CalculateNavMesh(posNewTotem);


        // en el caso de resetearlo al centro... no le damos velocidad ni nada.
        cTotem->speed = cTotem->MaxSpeed; // ya estamos a la maxima velocidad asi que no actualizamos
    }else{
        // en caso contrario, debe de soltarlo, pero no siempre igual, vamos a jugar un poquito 
        // lo lanzara para atras pero con un angulo aleatorio de entre unos 90 grados
        // si partimos de que el coche mira con rotacion 0 grados, el baremos de rangas sera: 135 y 225
        // ANGULO DEL COCHE INVERSO
        float angleCarInv = transfActualCar->rotation.y+180.0; if(angleCarInv > 360)    angleCarInv -=360;
        // RANGOS
        int minValue = (int(angleCarInv) - 65); //if(minValue < 0 )    minValue += 360;
        int maxValue = (int(angleCarInv) + 65); //if(maxValue > 360)   maxValue -= 360;

        //cout  << "EL VALOR MINIMO ES: " << minValue << " Y EL MAXIMO ES: " << maxValue << endl;

        //Número aleatorios entre minValue y maxValue: --> ROTACION DEL TOTEM
        //if(minValue > maxValue) std::swap(minValue, maxValue);
        rotTotem_Y = minValue+rand()%((maxValue+1)-minValue);
        if(rotTotem_Y < 0)      rotTotem_Y += 360; 
        if(rotTotem_Y > 360)    rotTotem_Y -= 360;


        //cout  << "EL FINAL: " << rotTotem_Y << endl;
        auto cTransfTotem = static_cast<CTransformable*>(entities[0]->GetComponent(CompType::TransformableComp).get());
        cTransfTotem->rotation.y = rotTotem_Y;

        // FINALMENTE, SOLO NOS QUEDA PONERLE LA VELOCIDAD A 0
        cTotem->speed = 0.0; // listo para salir para atras, y sin que se tenga que modificar nada del online :)
    }
    //cout << "Hemos perdido el totem con una rotation[" << rotTotem_Y << "] y un speed[" << cTotem->speed << "]" << endl;

    if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_SINGLE){
        ActivateTotem(posNewTotem, currentNavMesh);
    }else if(Game::GetInstance()->GetState()->GetState() == State::States::INGAME_MULTI){
        auto cOnline = static_cast<COnline*>(any_cast<Entity*>((*d)[ACTUAL_CAR])->GetComponent(CompType::OnlineComp).get());
        systemOnline->SendLostTotem(cOnline->idClient, posNewTotem, cTotem->speed, rotTotem_Y, currentNavMesh);
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
    //cout << "ManTotem::RecievedCatchTotem" << endl;
    auto cTotem = static_cast<CTotem*>(entities[0]->GetComponent(CompType::TotemComp).get());
    cTotem->active = false;
}


void ManTotem::RecievedLostTotem(DataMap* d){
    float speed = any_cast<float>((*d)[DataType::SPEED]);
    uint16_t rotationTotemY = any_cast<uint16_t>((*d)[DataType::ROTATION]);
    auto cTransformable = static_cast<CTransformable*>(entities[0]->GetComponent(CompType::TransformableComp).get());
    cTransformable->rotation.y = rotationTotemY;
    auto cTotem = static_cast<CTotem*>(entities[0]->GetComponent(CompType::TotemComp).get());
    cTotem->speed = speed;
    //cout << "Hemos recibido un lost totem con una rotation[" << rotationTotemY << "] y un speed[" << speed << "]" << endl;
    ActivateTotem(any_cast<glm::vec3>((*d)[DataType::VEC3_POS]), any_cast<int>((*d)[DataType::ID]));
}



void ManTotem::SyncTotem(DataMap* d){
    //if(any_cast<bool>((*d)[CAR_WITHOUT_TOTEM])){
    //    // Esto puede generar un problema al estar las posiciones modificandose respecto a la del otro
    //    // auto cTransTotem = static_cast<CTransformable*>(GetEntities()[0]->GetComponent(CompType::TransformableComp).get());
    //    // cTransTotem->position = any_cast<glm::vec3>((*d)[VEC3_POS]);
    //}else{
    //    // aqui no deberia haber totem, comprobar si hay totem y eliminarlo en caso de que exista
    //}

}