#pragma once

#include "CLEntity.h"
#include "CLNode.h"
#include "../ResourceManager/CLResourceManager.h"
#include "../ResourceManager/CLResourceTexture.h"
#include "../../../src/Constants.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

/**
 * Utilizamos flags por bits para saber que efectos tiene que aplicarse
 */
constexpr std::uint_fast8_t EFFECT_DIR_ALEATORITY{ 0x1 }; // hex for 0000 0001 
constexpr std::uint_fast8_t EFFECT_ZIG_ZAG{ 0x2 }; // hex for 0000 0010
// constexpr std::uint_fast8_t mask2{ 0x4 }; // hex for 0000 0100
// constexpr std::uint_fast8_t mask3{ 0x8 }; // hex for 0000 1000

enum SpawnType{
   Point,
   Line,
   Square,
   Cube,
   Circle,
   Sphere
    
};

namespace CLE {
class CLNode;

    class CLParticleSystem : public CLEntity {

        public:
            //Point
            CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan, std::uint_fast8_t _flags);
            //Line, Square y Cube, depende del valor de _offset
            CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,glm::vec3 _offset,glm::vec3 _orientation, std::uint_fast8_t _flags);
            //Circle
            CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, glm::vec3 _orientation, std::uint_fast8_t _flags);
            //Sphere
            CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, std::uint_fast8_t _flags);

            ~CLParticleSystem() = default;

            void Draw(GLuint shaderID) override;
            void DrawDepthMap(GLuint shaderID) override {};
            void Update();

            //SETTERS
            void SetCLNode(CLNode* clnode) {node = clnode;}
            void SetLoop(bool _loop) {loop = _loop;}

            //GETTERS
            CLNode*            GetCLNode()            const { return node; }
            glm::vec3          GetSpeedDirection()    const { return speedDirection; }
            ulong              GetNumberOfParticles() const { return nParticles; }
            glm::vec3          GetSpawnerPosition()   const { return spawnerPosition; }
            CLResourceTexture* GetTexture()           const { return clTexture; }
            uint16_t           GetWidth()             const { return width; }
            uint16_t           GetHeight()            const { return height; }
            float              GetSpawnDelay()        const { return spawnDelay; }
            unsigned int       GetNParticlesToSpawn() const { return nParticlesToSpawn; }
            float              GetLifeSpan()          const { return lifeSpan; }
            bool               GetLoop()              const { return loop; }
            SpawnType          GetSpawnType()         const { return spawnType; }
            float              GetRadious()           const { return radious; }
            glm::vec3          GetOrientation()       const { return orientation; }
            glm::vec3          GetOffset()            const { return offset; }
            std::uint_fast8_t  GetFlags()             const { return flags; }



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
            unsigned int nParticlesToSpawn = 1; //Particulas a spawnear a la vez
            float lifeSpan = 1000; //Tiempo de vida de las particulas
            bool loop = true;
            SpawnType spawnType = SpawnType::Point;
            float radious = 0; //Usado en SpawnType::Circle y SpawnType::Sphere
            glm::vec3 orientation = glm::vec3(1.0f); //Usado en SpawnType::Circle
            glm::vec3 offset; //Usado en SpawnType::Line, SpawnType::Square y  SpawnType::Cube
            std::uint_fast8_t flags = 0x0;


            class CLParticle{

                public:
                    CLParticle() = default;
                    CLParticle(CLParticleSystem* emitter);
                    ~CLParticle() = default;

                    void Draw(GLuint shaderID);

                private:
                    void Update();              //Aqui haremos los calculos necesarios de vida, posicion, etc
                    glm::vec3 CalculateSpawnPosition();
                    
                    GLuint VBO,VAO;
                    CLParticleSystem* particleSystem {nullptr};
                    glm::vec3 position;
                    glm::vec3 velocity;
                    float lifeSpan = 1000;
                    time_point<system_clock> timeStart;


            };
            
    };
}