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
 
 
 #include "ParticleSystem.h"

#include <Components/CId.h>
#include <Components/CTransformable.h>
#include <Components/CParticleSystem.h>
#include <Components/CType.h>
#include <Components/CParentNode.h>
#include <iostream>

using namespace std;



ParticleSystem::ParticleSystem(int parentId, glm::vec3 pos,unsigned long _nParticles, glm::vec3 _velocity, vector<string> _textures, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started)
{

    shared_ptr<CParentNode> cParentNode = make_shared<CParentNode>(parentId);
    if(parentId != -1){
        AddComponent(cParentNode);

    }
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    shared_ptr<CType> cType   = make_shared<CType>(ModelType::ParticleSystem);
    shared_ptr<CParticleSystem> cParticleSystem = make_shared<CParticleSystem>(_nParticles,_velocity,_textures,_width,_height,_spawnDelay,_particlesToSpawn,_lifeSpan,_offset,_orientation,_radious,_flags,_loop, _started);

    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cType);
    AddComponent(cParticleSystem);
}

