#include "CParticleSystem.h"

CParticleSystem::CParticleSystem(){
    m_compType = CompType::ParticleSystemComp;
}

CParticleSystem::CParticleSystem(unsigned long _nParticles, glm::vec3 _velocity, string _texture, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started){
 
    m_compType       = CompType::ParticleSystemComp;
    nParticles       = _nParticles;
    velocity         = _velocity;
    texture          = _texture;
    width            = _width;
    height           = _height;
    spawnDelay       = _spawnDelay;
    particlesToSpawn = _particlesToSpawn;
    lifeSpan         = _lifeSpan;
    offset           = _offset;
    orientation      = _orientation;
    radious          = _radious;
    flags            = _flags;
    loop             = _loop;
    started          = _started;

}