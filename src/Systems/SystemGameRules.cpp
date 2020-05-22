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
 
 
 #include "SystemGameRules.h"

#include <Entities/CarAI.h>
#include <Components/CShield.h>
#include <Components/CTotem.h>
#include <Components/CNitro.h>
#include <Components/CClock.h>
#include <Components/CId.h>
#include <Systems/Utils.h>

#include <Facade/Render/RenderFacadeManager.h>


// Temporal
SystemGameRules::SystemGameRules(){
    xLeftMap = -1008;      xRightMap = 1008;
    yUpMap = -1204;        yDownMap = 1204;

    widthMap = xRightMap - xLeftMap;
    heightMap = yDownMap - yUpMap;
}

SystemGameRules::SystemGameRules(float xl, float xr, float yu, float yd) {
    //xLeftMap = xl;      xRightMap = xr;
    //yUpMap = yu;        yDownMap = yd;
    xLeftMap = -1008;      xRightMap = 1008;
    yUpMap = -1204;        yDownMap = 1204;

    widthMap = xRightMap - xLeftMap;
    heightMap = yDownMap - yUpMap;
}

bool SystemGameRules::UpdateGameRules(Entity& globalClock_) const{

    // UPDATE RELOJ GLOBAL
    auto cClock = static_cast<CClock*>(globalClock_.GetComponent(CompType::ClockComp).get());
    if(cClock->active) {
        cClock->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cClock->timeStart).count();
        cClock->timeStart = system_clock::now();
    }
    if(cClock->accumulatedTime/1000.0 > cClock->DURATION_TIME/1000.0){
        //cout << "Se acabo el tiempo, nadie gana!!! \n";
        //Game::GetInstance()->SetState(State::ENDRACE);
        return true;
        // EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});
    }

    //Sonido reloj
    int time = cClock->DURATION_TIME/1000 - cClock->accumulatedTime/1000;
    if (time == 30) {
        EventManager::GetInstance().AddEventMulti(Event{EventType::CLOCK});
    } else if (time == 0 ) {
        EventManager::GetInstance().AddEventMulti(Event{EventType::NOT_CLOCK});
    }

    return false;
}



bool SystemGameRules::UpdateRulesCarPowerUps(Entity& car_, Entity& totem_) const{
    auto cTotem = static_cast<CTotem*>(car_.GetComponent(CompType::TotemComp).get());
    if (cTotem->active) {
        cTotem->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
        // lo pintamos encima del coche
        auto cTransformCar = static_cast<CTransformable *>(car_.GetComponent(CompType::TransformableComp).get());
        auto cTransformTotem = static_cast<CTransformable *>(totem_.GetComponent(CompType::TransformableComp).get());
            cTransformTotem->position.x = cTransformCar->position.x;
            cTransformTotem->position.z = cTransformCar->position.z;
            cTransformTotem->position.y = cTransformCar->position.y + 46.0f;
    }

    if(cTotem->accumulatedTime/1000.0 > cTotem->DURATION_TIME/1000.0){
        //cout << "Has ganado \n";
        //Game::GetInstance()->SetState(State::ENDRACE);
        return true;
        // EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});

    }
    // Actualiza el componente nitro
    auto cNitro = static_cast<CNitro *>(car_.GetComponent(CompType::NitroComp).get());
    if(cNitro->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime){  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }
    // Actualiza el componente escudo
    auto cShield = static_cast<CShield *>(car_.GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime){  // comprueba el tiempo desde que se lanzo
        auto cId = static_cast<CId*>(car_.GetComponent(CompType::IdComp).get());
        
        cShield->deactivePowerUp();
        shared_ptr<DataMap> data = make_shared<DataMap>();
        (*data)[ID] = cId->id;
        (*data)[TRUEFALSE] = false;
        EventManager::GetInstance().AddEventMulti(Event{EventType::UPDATE_SHIELD_VISIBILITY, data});
    }
    return false;
}

/**
 * Pone las posiciones de los coches relativas al plano del suelo
 */
void SystemGameRules::UpdatePositionsMiniMap(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems, unordered_map<uint16_t, glm::vec2> &positions, glm::vec2 &positionTotem){

    positions.clear();

    for(const auto& car : cars) {
        //cTotem = static_cast<CTotem*>(cars->GetComponent(CompType::TotemComp).get());
        auto cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
        auto cTrans = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());

        // MINIMAPA
        if (cTrans && cId) {
            auto posXPjMM = (cTrans->position.x - xLeftMap) / widthMap;
            auto posYPjMM = (cTrans->position.z - yUpMap) / heightMap;

            positions[cId->id] = glm::vec2(posXPjMM, posYPjMM);

            //cout << "CAR " << positions.find(cId->id)->first << " CON POS: " << positions.find(cId->id)->second.x << " - " << positions.find(cId->id)->second.y << endl;
        }
    }  

    for(const auto& totem : totems) {
        auto cTrans = static_cast<CTransformable*>(totem->GetComponent(CompType::TransformableComp).get());
        if (cTrans) {
            auto posXTMM = (cTrans->position.x - xLeftMap) / widthMap;
            auto posYTMM = (cTrans->position.z - yUpMap) / heightMap;

            positionTotem = glm::vec2(posXTMM, posYTMM);
        }
    }

}

void SystemGameRules::RestartAllTimers(vector<shared_ptr<Entity>> entities, Entity &globalClock_, int64_t timeStartPause) {
    //cout << "Estamos reseteando los timers" << endl;
    auto cClock = static_cast<CClock*>(globalClock_.GetComponent(CompType::ClockComp).get());
    if(cClock->active) 
        cClock->timeStart = system_clock::now();

    for(auto e : entities) {
        auto cTotem = static_cast<CTotem*>(e->GetComponent(CompType::TotemComp).get());
        if(cTotem->active)
            cTotem->timeStart = system_clock::now();

        bool hasNitro = e->HasComponent(CompType::NitroComp);
        if(hasNitro) {
            auto cNitro = static_cast<CNitro*>(e->GetComponent(CompType::NitroComp).get());
            if(cNitro->activePowerUp) {
                int64_t now = Utils::getMillisSinceEpoch();
                int64_t interval = now-timeStartPause;
                cNitro->timeStart += std::chrono::milliseconds(interval);
            }
        }

        bool hasShield = e->HasComponent(CompType::NitroComp);
        if(hasShield) {
            auto cShield = static_cast<CShield*>(e->GetComponent(CompType::ShieldComp).get());
            if(cShield->activePowerUp) {
                int64_t now = Utils::getMillisSinceEpoch();
                int64_t interval = now-timeStartPause;
                cShield->timeStart += std::chrono::milliseconds(interval);
            }
        }
    }
}