#pragma once

#include "Entity.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class ParticleSystem : public Entity
{

public:
    ParticleSystem(unsigned int parentId, glm::vec3 pos,unsigned long _nParticles, glm::vec3 _velocity, vector<string> _textures, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started);
    ~ParticleSystem() = default;
};
