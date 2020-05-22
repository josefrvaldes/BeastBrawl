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
 
 
 #include "ManParticleSystem.h"

#include <Components/CParentNode.h>
#include <Components/CPowerUp.h>
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

    EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::CREATE_PARTICLES_ROBOJOROBO,
                bind(&ManParticleSystem::CreateParticleRobojorobo, this, placeholders::_1),
                "CreateParticleRobojorobo"});
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
    auto typePU = any_cast<typeCPowerUp>((*data)[TYPE]);
    vector<string> texturePs2;
    switch (typePU) {
        case typeCPowerUp::MelonMolon:
            texturePs2.push_back("media/particleTriangleBlack.png");
            texturePs2.push_back("media/particleTriangleGreen.png");
            texturePs2.push_back("media/particleStarBlack.png");
            texturePs2.push_back("media/particleStarGreen.png");
            break;
        case typeCPowerUp::TeleBanana:
            texturePs2.push_back("media/particleYellowTriangle.png");
            texturePs2.push_back("media/particleYellowStar.png");
            texturePs2.push_back("media/particleTriangleGrey.png");
            break;
        
        case typeCPowerUp::PudinDeFrambuesa:
            texturePs2.push_back("media/particleStarMarron.png");
            texturePs2.push_back("media/particleTriangleBrown.png");
            texturePs2.push_back("media/particleYellowStar.png");
            texturePs2.push_back("media/particleRedTriangle.png");
            texturePs2.push_back("media/particleYellowTriangle.png");
            break;
        default: 
            texturePs2.push_back("media/particleStarMarron.png");
            texturePs2.push_back("media/particleYellowStar.png");
            texturePs2.push_back("media/particleRedTriangle.png");
            texturePs2.push_back("media/particleYellowTriangle.png");
            break;
    }

    CreateParticleSystem(-1,position,15,glm::vec3(400.0f,400.0f,400.0f),texturePs2,10,10,100,15,180,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),5, 0x1,false,true);

    renderEngine->FacadeAddObject(entities[entities.size()-1].get());
}

void ManParticleSystem::CreateParticleRobojorobo(DataMap* data){
    auto renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();

    vector<string> texturePs;
    texturePs.push_back("media/robojoroboParticle.png");


    auto transformable = any_cast<CTransformable*>((*data)[CAR_TRANSFORMABLE]);

    CreateParticleSystem(-1, glm::vec3(transformable->position.x,transformable->position.y +8, transformable->position.z), 50, glm::vec3(0.0f, 100.0f, 0.0f), texturePs, 5, 5, 50, 10, 200, glm::vec3(10.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0, 0x4, false, true);

    renderEngine->FacadeAddObject(entities[entities.size()-1].get());


}