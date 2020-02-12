#include "ManTotem.h"

#include <Entities/Totem.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Components/CDimensions.h>
#include <Components/CCurrentNavMesh.h>
#include <include_json/include_json.hpp>

class Position;
using namespace std;
using json = nlohmann::json;
ManTotem::ManTotem() {
    // CREAMOS EL TOTEM
    ifstream i("data.json");
    json j = json::parse(i);

    double totemPosX = j["TOTEM"]["x"].get<double>();
    double totemPosY = j["TOTEM"]["y"].get<double>();
    double totemPosZ = j["TOTEM"]["z"].get<double>();
    std::cout << " LA POS X DEL TOTEM ES LA: " << totemPosX << std::endl;
    //CreateTotem(glm::vec3(-100.0, 20.0, -100.0));
    CreateTotem(glm::vec3(totemPosX, totemPosY-15, totemPosZ));
    SubscribeToEvents();
   // CreateTotem();
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
        entities.push_back(totem); 
    }
}


// elimina el totem de irrlich y lo oculta del mapa
void ManTotem::AppertainCar(DataMap* d){
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    for(long unsigned int i=0; i< entities.size(); ++i){
        if(entities[i] == any_cast<shared_ptr<Entity>>((*d)[TOTEM])){
            renderEngine->DeleteEntity(entities[i].get());
            entities.erase(entities.begin()+i);
        }
    }
}


// crea el objeto en irlich y lo pone en el mapa de nuevo
void ManTotem::ResetTotem(DataMap* d){
    //std::cout << "reseteamos posicion del totem" << std::endl;

    // calculamos la posicion donde queremos dejar el totem
    auto transfActualCar = any_cast<CTransformable*>((*d)[CAR_TRANSFORMABLE]); 
    glm::vec3 posNewTotem = glm::vec3(0.0f,10.0f,0.0f);
    float angleRotation = (transfActualCar->rotation.y * 3.141592) / 180.0;
    posNewTotem.x = transfActualCar->position.x - cos(angleRotation)*(-25);
    posNewTotem.z = transfActualCar->position.z + sin(angleRotation)*(-25);

    // recorremos los navMesh para saber si es una posicion correcta
    ManNavMesh* manNavMesh = any_cast<ManNavMesh*>((*d)[MAN_NAVMESH]);
    //std::cout << "cogemos correctamente el mesh del nav conoooooooooooooooo" << std::endl;  
    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(any_cast<Entity*>((*d)[ACTUAL_CAR])->GetComponent(CompType::CurrentNavMeshComp).get()); 
    auto navMesh = manNavMesh->GetEntities()[cCurrentNavMesh->currentNavMesh]; //NavMesh en el que esta el coche
    auto cDimensions = static_cast<CDimensions*>(navMesh->GetComponent(CompType::DimensionsComp).get());
    auto cTransformableNav = static_cast<CTransformable*>(navMesh->GetComponent(CompType::TransformableComp).get());
    if( !(( (transfActualCar->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
        (transfActualCar->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
        ( (transfActualCar->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
        (transfActualCar->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) ))  ){
            std::cout << "El totem se sale, Redimensionamos al centro del NavMesh" << std::endl;
            posNewTotem.x = cTransformableNav->position.x;
            posNewTotem.z = cTransformableNav->position.z; 
    }      

    CreateTotem(posNewTotem);

    //std::cout << "vamos a pasar el totem desde eventos" << std::endl;
    // Vamos a actualizar el NavMesh en el que se va a soltar el Totem
    shared_ptr<DataMap> data = make_shared<DataMap>();

    (*data)[TOTEM] = GetEntities()[0].get();                                                        // pasamos un puntero al totem
    EventManager::GetInstance().AddEventMulti(Event{EventType::ACTUALIZE_NAVMESH_TOTEM, data});
    //std::cout << "volvemos de lanzar el evento beibeee" << std::endl;

    // Debemos de crearlo tambien en iirlicht
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    //for(long unsigned int i=0; i< totems.size(); ++i){
        renderEngine->FacadeAddObjectTotem(entities[0].get());
   // }
}


void ManTotem::SubscribeToEvents() {
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_PLAYER_TOTEM,
        bind(&ManTotem::AppertainCar, this, placeholders::_1),
        "AppertainCar"));
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::COLLISION_AI_TOTEM,
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