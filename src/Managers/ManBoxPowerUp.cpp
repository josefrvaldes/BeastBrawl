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
 
 
 #include "ManBoxPowerUp.h"

//#include <functional> 
#include <cstring>
#include <sstream>
#include <Entities/BoxPowerUp.h>
#include <include_json/include_json.hpp>
#include <Facade/Sound/SoundFacadeManager.h>

class Position;
using json = nlohmann::json;
using namespace std;



ManBoxPowerUp::ManBoxPowerUp() {
    //Leemos y añadimos los WayPoints
    float x=0,y=0,z=0; //Vec3

    std::ifstream i("data.json");
    std::stringstream buffer;
    buffer << i.rdbuf();
    string jsonString = buffer.str();
    json j = json::parse(jsonString);
    i.close();

    int waypointsCount = j["WAYPOINTS"].size();
    //std::cout << "EL NUMERO DE WAYPOINTS EN EL JSON ES: " << waypointsCount << std::endl;
    //Leemos el array de waypoints
    for(int i = 0; i< waypointsCount; ++i){
        string idWayPoint = to_string(i);  //El primer elemento es "0" como string
        auto capaActual = j["WAYPOINTS"][idWayPoint];
        if( capaActual["type"].get<int>() == 1){
            x    = capaActual["x"].get<float>();
            y    = capaActual["y"].get<float>();    // INTERCAMBIAMOS REALMENTE LA "Y" POR LA "Z"
            z    = capaActual["z"].get<float>();
            glm::vec3 position = glm::vec3(x,y,z);
            CreateBoxPowerUp(position);
        }
    }

    //CreateBoxPowerUp(vec3(-450.f, -600.f, 700.f));
    

    SubscribeToEvents();
    //cout << "Hemos creado el manager de powerup, ahora tenemos " << entities.size() << " powerups" << endl;
}


ManBoxPowerUp::~ManBoxPowerUp() {
    entities.clear();
    entities.shrink_to_fit();
}


void ManBoxPowerUp::CreateBoxPowerUp(glm::vec3 _position){
	shared_ptr<BoxPowerUp> p = make_shared<BoxPowerUp>(_position);
    entities.push_back(p);
    auto idComp = static_cast<CId*>(p->GetComponent(CompType::IdComp).get());
    string nameEvent = "Partida/coger_caja";
    SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, _position, nameEvent, 0);
}


void ManBoxPowerUp::CreateBoxPowerUp() {
    shared_ptr<BoxPowerUp> p = make_shared<BoxPowerUp>();
    entities.push_back(p);
}

// se ejecuta en caso de que alguno de los coches coja la caja
void ManBoxPowerUp::EjecutarMeHanCogido(DataMap* d) {
    auto cBoxPowerUp = any_cast<CBoxPowerUp*>((*d)[BOX_POWER_UP_COMPONENT]);
    auto actualBox   = any_cast<shared_ptr<Entity>>((*d)[ACTUAL_BOX]);

    if(cBoxPowerUp->active == true){
        //cout << "Han cogido un powerup, madafaka!! sera la primera" << endl;
        auto renderFacadeManager = RenderFacadeManager::GetInstance();
        auto renderEngine = renderFacadeManager->GetRenderFacade();
        renderEngine->FacadeSetVisibleEntity(actualBox.get(),false);
        //renderEngine->DeleteEntity(actualBox.get());       // se elmina la caja en irrlich para que no la dibuje, pero en nuestro array sigue estando
        cBoxPowerUp->active = false;
        cBoxPowerUp->timeStart = system_clock::now();

        shared_ptr<DataMap> data = make_shared<DataMap>();
        /*auto cTranformableBox = static_cast<CTransformable*>(actualBox.get()->GetComponent(CompType::TransformableComp).get());
        (*data)[BOX_POSITION] = cTranformableBox->position;*/
        (*data)[ID] = static_cast<CId*>(actualBox.get()->GetComponent(CompType::IdComp).get())->id;
        EventManager::GetInstance().AddEventMulti(Event{EventType::BREAK_BOX, data});
    }
}


void ManBoxPowerUp::SubscribeToEvents() {
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::CATCH_BOX_POWERUP,
        bind(&ManBoxPowerUp::EjecutarMeHanCogido, this, placeholders::_1),
        "EjecutarMeHanCogido"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::CATCH_AI_BOX_POWERUP,
        bind(&ManBoxPowerUp::EjecutarMeHanCogido, this, placeholders::_1),
        "EjecutarMeHanCogido"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::CATCH_BOX_WITH_POWERUP,
        bind(&ManBoxPowerUp::EjecutarMeHanCogido, this, placeholders::_1),
        "EjecutarMeHanCogido"));
}


// se crea la caja de irrlich eliminada anteriormente
void ManBoxPowerUp::resetBox(Entity* resetBox){
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    //renderEngine->FacadeAddObject(resetBox);
    renderEngine->FacadeSetVisibleEntity(resetBox,true);
}
