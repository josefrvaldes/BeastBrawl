#include "ManGameRules.h"
#include "../Entities/Clock.h"
#include "../Components/CClock.h"
#include "../Systems/SystemGameRules.h"
#include <iostream>


ManGameRules::ManGameRules() {
    // creamos el reloj global
    systemGameRules = make_unique<SystemGameRules>();
    // CreateGlobalClock(15);
    
    CreateGlobalClock(GameValues::GetInstance()->GetGameTime());
    //CreateGlobalClock(5);

    // SUPONEMOS QUE NO TENEMOS QUE HACER EL SET AQUI, SINO QUE NOS VIENE DE FUERA
    GameValues::GetInstance()->SetMap(MAP::MAYAN);

    CreateCurrentMap(GameValues::GetInstance()->GetMap());

}


void ManGameRules::CreateCurrentMap(MAP type){
   switch(type) {
      case MAYAN:
         CreateMayanMap();
        break;
      default:
         cout << "CREO QUE TE HAS EQUIVOCADO DE MAPA AMIGOO \n";
        break;
   }
}



void ManGameRules::CreateGlobalClock( const uint32_t durationTime_) {
    globalClock = make_unique<Clock>(durationTime_);

    auto cClock = static_cast<CClock*>(globalClock->GetComponent(CompType::ClockComp).get());
    cClock->active = true;
    cClock->timeStart = system_clock::now();
}

void ManGameRules::ResetClock() {
    auto cClock = static_cast<CClock*>(globalClock->GetComponent(CompType::ClockComp).get());
    cClock->active = true;
    cClock->timeStart = system_clock::now();
}

ManGameRules::~ManGameRules() {
}

bool ManGameRules::Update(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems){
    //cout << "HOLAAAA" << endl;
    systemGameRules->UpdatePositionsMiniMap(cars, totems, positionsPlane, positionTotemPlane);
    return systemGameRules->UpdateGameRules( *(globalClock.get()) );
}

void ManGameRules::RestartAllTimers(vector<shared_ptr<Entity>> entities, int64_t timeStartPause) {
    systemGameRules->RestartAllTimers(entities, *(globalClock.get()), timeStartPause);
}

void ManGameRules::SubscribeToEvents() {

}




void ManGameRules::CreateMayanMap(){
    auto default0 = glm::vec3(0.0f, 0.0f, 0.0f);
    auto default1 = glm::vec3(1.0f, 1.0f, 1.0f);

    shared_ptr<GameObject> bridge_north         = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/bridge_north.obj");
        ground.push_back(bridge_north);
    shared_ptr<GameObject> bridge_south         = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/bridge_south.obj");
        ground.push_back(bridge_south);
    shared_ptr<GameObject> central_plaza        = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/central_plaza.obj");
        ground.push_back(central_plaza);
    shared_ptr<GameObject> grounds_assets       = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/grounds+assets.obj");
        ground.push_back(grounds_assets);
    shared_ptr<GameObject> joke_heads           = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/joke_heads.obj");
        ground.push_back(joke_heads);
    shared_ptr<GameObject> ramps                = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/ramps.obj");
        ground.push_back(ramps);
    shared_ptr<GameObject> snake_heads          = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/snake_heads.obj");
        ground.push_back(snake_heads);
    shared_ptr<GameObject> wall_east_west       = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/wall_east_west.obj");
        ground.push_back(wall_east_west);
    shared_ptr<GameObject> wall_north_south     = make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/wall_north_south.obj");
        ground.push_back(wall_north_south);
}