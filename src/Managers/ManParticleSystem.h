#pragma once

#include <Managers/Manager.h>
#include <Entities/ParticleSystem.h>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>



using namespace std;
struct Data;

class ManParticleSystem : public Manager{
   public:
    ManParticleSystem();
    ~ManParticleSystem() = default;

    void CreateParticleSystem(unsigned int parentId, glm::vec3 pos,unsigned long _nParticles, glm::vec3 _velocity, vector<string> _textures, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started);

    void ActivateParticlesBoxPowerUp(DataMap* data);
    void CreateParticlePowerUpCollision(DataMap* data);


   private:
    void SubscribeToEvents() override;

};