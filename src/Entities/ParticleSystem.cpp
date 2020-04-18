#include "ParticleSystem.h"

#include <Components/CId.h>
#include <Components/CTransformable.h>
#include <Components/CParticleSystem.h>
#include <Components/CType.h>
#include <Components/CParentNode.h>
#include <iostream>

using namespace std;



ParticleSystem::ParticleSystem(unsigned int parentId, glm::vec3 pos,unsigned long _nParticles, glm::vec3 _velocity, vector<string> _textures, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started)
{


    shared_ptr<CParentNode> cParentNode = make_shared<CParentNode>(parentId);
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    shared_ptr<CType> cType   = make_shared<CType>(ModelType::ParticleSystem);
    shared_ptr<CParticleSystem> cParticleSystem = make_shared<CParticleSystem>(_nParticles,_velocity,_textures,_width,_height,_spawnDelay,_particlesToSpawn,_lifeSpan,_offset,_orientation,_radious,_flags,_loop, _started);

    AddComponent(cParentNode);
    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cType);
    AddComponent(cParticleSystem);
}

