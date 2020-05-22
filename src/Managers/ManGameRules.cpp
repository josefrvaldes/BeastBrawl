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
 
 
 #include "ManGameRules.h"
#include "../Entities/Clock.h"
#include "../Components/CTransformable.h"
#include "../Components/CTotem.h"
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
         //cout << "CREO QUE TE HAS EQUIVOCADO DE MAPA AMIGOO \n";
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

    if(static_cast<CTotem*>(totems[0]->GetComponent(CompType::TotemComp).get())->active){
        posTotem = static_cast<CTransformable*>(totems[0]->GetComponent(CompType::TransformableComp).get())->position;
    }else{
        for(unsigned long int i=0; i< cars.size(); i++){
            if(static_cast<CTotem*>(cars[i]->GetComponent(CompType::TotemComp).get())->active){
                posTotem = static_cast<CTransformable*>(cars[i]->GetComponent(CompType::TransformableComp).get())->position;
            }
        }
    }
    //cout << "HOLAAAA" << endl;
    systemGameRules->UpdatePositionsMiniMap(cars, totems, positionsPlane, positionTotemPlane);
    return systemGameRules->UpdateGameRules( *(globalClock.get()) );
}

void ManGameRules::RestartAllTimers(vector<shared_ptr<Entity>> entities, int64_t timeStartPause) {
    systemGameRules->RestartAllTimers(entities, *(globalClock.get()), timeStartPause);
}

void ManGameRules::SubscribeToEvents() {

}


void ManGameRules::InitializeMiniMap(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems) {
    systemGameRules->UpdatePositionsMiniMap(cars, totems, positionsPlane, positionTotemPlane);
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
    shared_ptr<GameObject> deco_down_left		= make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/deco_down_left.obj");
        ground.push_back(deco_down_left);
    shared_ptr<GameObject> deco_down_right		= make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/deco_down_right.obj");
        ground.push_back(deco_down_right);
    shared_ptr<GameObject> deco_up_left			= make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/deco_up_left.obj");
        ground.push_back(deco_up_left);
    shared_ptr<GameObject> deco_up_right		= make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/deco_up_right.obj");
        ground.push_back(deco_up_right);
    shared_ptr<GameObject> joke_head1			= make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/joke_head1.obj");
        ground.push_back(joke_head1);
    shared_ptr<GameObject> joke_head2			= make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/joke_head2.obj");
        ground.push_back(joke_head2);
	shared_ptr<GameObject> ramp1				= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/ramp1.obj");
		ground.push_back(ramp1);
	shared_ptr<GameObject> ramp2				= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/ramp2.obj");
		ground.push_back(ramp2);
	shared_ptr<GameObject> ramp3				= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/ramp3.obj");
		ground.push_back(ramp3);
	shared_ptr<GameObject> ramp4				= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/ramp4.obj");
		ground.push_back(ramp4);
	shared_ptr<GameObject> snake_head1			= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/snake_heads1.obj");
		ground.push_back(snake_head1);
	shared_ptr<GameObject> snake_head2			= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/snake_heads2.obj");
		ground.push_back(snake_head2);
	shared_ptr<GameObject> walls_east_west1		= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/walls_east_west1.obj");
		ground.push_back(walls_east_west1);
	shared_ptr<GameObject> walls_east_west2		= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/walls_east_west2.obj");
		ground.push_back(walls_east_west2);
	shared_ptr<GameObject> walls_north_south1	= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/walls_north_south1.obj");
		ground.push_back(walls_north_south1);
	shared_ptr<GameObject> walls_north_south2	= make_shared<GameObject>(default0, default0, default1, "", "maps/mayan/walls_north_south2.obj");
		ground.push_back(walls_north_south2);
    shared_ptr<GameObject> groundswalls			= make_shared<GameObject>(default0,default0,default1, "", "maps/mayan/groundswalls.obj");
        ground.push_back(groundswalls);
}