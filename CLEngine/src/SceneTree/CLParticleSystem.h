#pragma once

#include "CLEntity.h"
#include "CLNode.h"
#include "../ResourceManager/CLResourceManager.h"
#include "../ResourceManager/CLResourceTexture.h"
#include "../../../src/Constants.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;
namespace CLE {
class CLNode;

    class CLParticleSystem : public CLEntity {

        public:
            //CLParticleSystem() = default;
            //CLParticleSystem(unsigned int idEntity) : CLEntity(idEntity) {};
            CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,uint16_t _nParticlesToSpawn);

            ~CLParticleSystem() = default;

            void Draw(GLuint shaderID) override;
            void DrawDepthMap(GLuint shaderID) override {};
            void Update();

            //SETTERS
            void SetCLNode(CLNode* clnode);
            void SetPosition(glm::vec3 position);

            //GETTERS
            CLNode*            GetCLNode()            const { return node; }
            glm::vec3          GetSpeedDirection()    const { return speedDirection; }
            ulong              GetNumberOfParticles() const { return nParticles; }
            glm::vec3          GetSpawnerPosition()   const { return spawnerPosition; }
            CLResourceTexture* GetTexture()           const { return clTexture; }
            uint16_t           GetWidth()             const { return width; }
            uint16_t           GetHeight()            const { return height; }
            float              GetSpawnDelay()        const { return spawnDelay; }
            uint16_t           GetNParticlesToSpawn() const { return nParticlesToSpawn; }



        private:
            class CLParticle; //Forward declaration

            vector<shared_ptr<CLParticle>> particles;
            CLNode* node {nullptr};     //CLNode para poder acceder a su matriz modelo, vista, proyeccion y demas cosas utiles
            ulong nParticles { 10 }; //Numero de particulas que vamos a tener
            glm::vec3 speedDirection;
            glm::vec3 spawnerPosition;
            CLResourceTexture* clTexture {nullptr};
            uint16_t width{0}, height{0};
            time_point<system_clock> timeStart;
            float spawnDelay = 1000; //Tiempo en ms
            uint16_t nParticlesToSpawn = 1; //Particulas a spawnear a la vez


            class CLParticle{

                public:
                    CLParticle() = default;
                    CLParticle(CLParticleSystem* emitter);
                    ~CLParticle() = default;

                    void Draw(GLuint shaderID);

                private:
                    void Update();              //Aqui haremos los calculos necesarios de vida, posicion, etc
                    
                    GLuint VBO,VAO;
                    CLParticleSystem* particleSystem {nullptr};
                    glm::vec3 position;


            };
            
    };
}