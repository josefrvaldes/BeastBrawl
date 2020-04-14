#include "ManParticleSystem.h"

#include <Components/CParentNode.h>
#include <Components/CId.h>
#include <iostream>

using namespace std;

ManParticleSystem::ManParticleSystem() {
    SubscribeToEvents();
}

void ManParticleSystem::CreateParticleSystem(unsigned int parentId, glm::vec3 pos,unsigned long _nParticles, glm::vec3 _velocity, string _texture, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started){

    shared_ptr<ParticleSystem> e = make_shared<ParticleSystem>(parentId,pos,_nParticles,_velocity,_texture,_width,_height,_spawnDelay,_particlesToSpawn,_lifeSpan,_offset,_orientation,_radious,_flags,_loop, _started);

    entities.push_back(e);
 
}

void ManParticleSystem::SubscribeToEvents(){
    EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::BREAK_BOX,
                bind(&ManParticleSystem::ActivarParticulas, this, placeholders::_1),
                "ActivarParticulas"});
}

void ManParticleSystem::ActivarParticulas(DataMap* data){
    auto idEntity = any_cast<uint16_t>((*data)[ID]);

    for(const auto& particleSystem : entities){
        auto cParentNode = static_cast<CParentNode*>(particleSystem->GetComponent(CompType::ParentNodeComp).get());
        if(cParentNode->idParentNode == idEntity){
            //Se debe activar este sistema de particulas
            shared_ptr<DataMap> data = make_shared<DataMap>();
            (*data)[ID] = static_cast<CId*>(particleSystem.get()->GetComponent(CompType::IdComp).get())->id;
            EventManager::GetInstance().AddEventMulti(Event{EventType::INIT_PARTICLE_SYSTEM, data});
        }
    }
}