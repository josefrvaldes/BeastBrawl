#include "ManParticleSystem.h"

#include <Components/CParentNode.h>
#include <Components/CId.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <iostream>

using namespace std;

ManParticleSystem::ManParticleSystem() {
    SubscribeToEvents();
}

void ManParticleSystem::CreateParticleSystem(unsigned int parentId, glm::vec3 pos,unsigned long _nParticles, glm::vec3 _velocity, vector<string> _textures, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started){

    shared_ptr<ParticleSystem> e = make_shared<ParticleSystem>(parentId,pos,_nParticles,_velocity,_textures,_width,_height,_spawnDelay,_particlesToSpawn,_lifeSpan,_offset,_orientation,_radious,_flags,_loop, _started);

    entities.push_back(e);
 
}

void ManParticleSystem::SubscribeToEvents(){
    
    EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::BREAK_BOX,
                bind(&ManParticleSystem::ActivateParticlesBoxPowerUp, this, placeholders::_1),
                "ActivateParticlesBoxPowerUp"});

    EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::CREATE_PARTICLES_COLLISION_POWERUP,
                bind(&ManParticleSystem::CreateParticlePowerUpCollision, this, placeholders::_1),
                "CreateParticlePowerUpCollision"});
}

void ManParticleSystem::ActivateParticlesBoxPowerUp(DataMap* data){
    auto idEntity = any_cast<uint16_t>((*data)[ID]);

    for(auto particleSystem : entities){
        if(particleSystem->HasComponent(CompType::ParentNodeComp)){

            auto cParentNode = static_cast<CParentNode*>(particleSystem->GetComponent(CompType::ParentNodeComp).get());
            if(cParentNode->idParentNode == idEntity){
                //Se debe activar este sistema de particulas
                shared_ptr<DataMap> data = make_shared<DataMap>();
                (*data)[ID] = static_cast<CId*>(particleSystem.get()->GetComponent(CompType::IdComp).get())->id;
                EventManager::GetInstance().AddEventMulti(Event{EventType::INIT_PARTICLES_BOX, data});
            }
        }
    }
}

void ManParticleSystem::CreateParticlePowerUpCollision(DataMap* data){
    auto renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();

    auto position = any_cast<glm::vec3>((*data)[VEC3_POS]);
    vector<string> texturePs2;
    texturePs2.push_back("media/particleRedStar.png");
    texturePs2.push_back("media/particleYellowStar.png");
    texturePs2.push_back("media/particleRedTriangle.png");
    texturePs2.push_back("media/particleYellowTriangle.png");
    CreateParticleSystem(-1,position,15,glm::vec3(400.0f,400.0f,400.0f),texturePs2,10,10,100,15,180,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),5, 0x1,false,true);

    renderEngine->FacadeAddObject(entities[entities.size()-1].get());
}
