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
 * @author Rubén Rubio Martínez <https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/> 
 * 
 * 
 */
 
 
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

//! Mask para no aplicar efectos
constexpr std::uint_fast8_t EFFECT_NONE          { 0x0 }; // hex for 0000 0001 
//! Mask para aplicar el efecto de direccion aleatoria 
constexpr std::uint_fast8_t EFFECT_DIR_ALEATORITY{ 0x1 }; // hex for 0000 0001 
//! Mask para aplicar el efecto de zigzag
constexpr std::uint_fast8_t EFFECT_ZIG_ZAG       { 0x2 }; // hex for 0000 0010
//! Mask para efecto de desvanecimiento
constexpr std::uint_fast8_t EFFECT_FADING        { 0x4 }; // hex for 0000 0100
// constexpr std::uint_fast8_t mask2{ 0x4 }; // hex for 0000 0100
// constexpr std::uint_fast8_t mask3{ 0x8 }; // hex for 0000 1000

//! Enum para diferentes tipos shapes para el spawner
enum SpawnType{
   PointSpawn,
   LineSpawn,
   SquareSpawn,
   CubeSpawn,
   CircleSpawn,
   SphereSpawn
    
};

namespace CLE {
class CLNode;
    //! Clase para crear sistemas de particulas
    class CLParticleSystem : public CLEntity {

        public:
            //! Constructor para spawner de punto
            //! @param idEntity Id de la entidad
            //! @param _nParticles Número de particulas
            //! @param _speedDirection Vector de velocidad de las particulas
            //! @param texture Textura para las particulas
            //! @param _width Anchura de las particulas
            //! @param _height Altura de las particulas
            //! @param _spawnDelay Delay de spawn entre particulas
            //! @param _nParticlesToSpawn Número de particulas a spawnear cada vez que pasa _SpawnDelay
            //! @param _lifeSpan Vida de las particulas
            //! @param _flags Mascara con los efectos para aplicarle
            CLParticleSystem(unsigned int idEntity, unsigned int _nParticles, glm::vec3 _speedDirection,vector<string> texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan, std::uint_fast8_t _flags);
            //! Constructor para spawner de linea, cuadrado y cubo
            //! @param idEntity Id de la entidad
            //! @param _nParticles Número de particulas
            //! @param _speedDirection Vector de velocidad de las particulas
            //! @param texture Textura para las particulas
            //! @param _width Anchura de las particulas
            //! @param _height Altura de las particulas
            //! @param _spawnDelay Delay de spawn entre particulas
            //! @param _nParticlesToSpawn Número de particulas a spawnear cada vez que pasa _SpawnDelay
            //! @param _lifeSpan Vida de las particulas
            //! @param _offset Propiedades del shape del spawner, dependiendo de los valores puede ser una linea, un cuadrado o un cubo
            //! @param _orientation Direccion a la que mira la shape del spawner
            //! @param _flags Mascara con los efectos para aplicarle
            CLParticleSystem(unsigned int idEntity, unsigned int _nParticles, glm::vec3 _speedDirection,vector<string> texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,glm::vec3 _offset,glm::vec3 _orientation, std::uint_fast8_t _flags);
            //! Constructor para spawner de circulo
            //! @param idEntity Id de la entidad
            //! @param _nParticles Número de particulas
            //! @param _speedDirection Vector de velocidad de las particulas
            //! @param texture Textura para las particulas
            //! @param _width Anchura de las particulas
            //! @param _height Altura de las particulas
            //! @param _spawnDelay Delay de spawn entre particulas
            //! @param _nParticlesToSpawn Número de particulas a spawnear cada vez que pasa _SpawnDelay
            //! @param _lifeSpan Vida de las particulas
            //! @param _radious Radio del circulo del spawner
            //! @param _orientation Direccion a la que mira la shape del spawner
            //! @param _flags Mascara con los efectos para aplicarle
            CLParticleSystem(unsigned int idEntity, unsigned int _nParticles, glm::vec3 _speedDirection,vector<string> texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, glm::vec3 _orientation, std::uint_fast8_t _flags);
            //! Constructor para spawner de esfera
            //! @param idEntity Id de la entidad
            //! @param _nParticles Número de particulas
            //! @param _speedDirection Vector de velocidad de las particulas
            //! @param texture Textura para las particulas
            //! @param _width Anchura de las particulas
            //! @param _height Altura de las particulas
            //! @param _spawnDelay Delay de spawn entre particulas
            //! @param _nParticlesToSpawn Número de particulas a spawnear cada vez que pasa _SpawnDelay
            //! @param _lifeSpan Vida de las particulas
            //! @param _radious Radio de la circunferencia
            //! @param _flags Mascara con los efectos para aplicarle
            CLParticleSystem(unsigned int idEntity, unsigned int _nParticles, glm::vec3 _speedDirection,vector<string> texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, std::uint_fast8_t _flags);

            ~CLParticleSystem() = default;

            void Draw(GLuint shaderID) override;
            void DrawDepthMap(GLuint shaderID) override {};

            //SETTERS
            //! Asigna el nodo del arbol 
            //! @param clnode Puntero al nodo del arbol de la escena
            void SetCLNode(CLNode* clnode) {node = clnode;}
            //! Asigna el estado de loop
            //! @param _loop Si el spawner esta en modo repetición o no
            void SetLoop(bool _loop) {loop = _loop;}
            //! Inicia la emisión de particulas
            void Start() { started = true; }
            //! Inicia una sola iteracion de particulas
            void StartOneIteration() { started = true; oneIterationMode = true; }
            

            //GETTERS
            //! Devuelve el nodo del arbol donde spawnea
            //! @returns node CLE::CLNode()
            CLNode*            GetCLNode()            const { return node; }
            //! Devuelve el vector de velocidad
            //! @returns speedDirection
            glm::vec3          GetSpeedDirection()    const { return speedDirection; }
            //! Devuelve el numero de particulas
            //! @returns nParticles
            unsigned int              GetNumberOfParticles() const { return nParticles; }
            //! Devuelve la posición del spawner
            //! @returns spawnerPosition
            glm::vec3          GetSpawnerPosition()   const { return spawnerPosition; }
            //! Devuelve las texturas de las particulas del spawner
            //! @returns clTextures
            vector<CLResourceTexture*> GetTextures()           const { return clTextures; }
            //! Devuelve el ancho de las particulas
            //! @returns width
            uint16_t           GetWidth()             const { return width; }
            //! Devuelve la altura de las particulas
            //! @returns heihgt
            uint16_t           GetHeight()            const { return height; }
            //! Devuelve delay entre spawn de particulas del spawner
            //! @returns spawnDelay
            float              GetSpawnDelay()        const { return spawnDelay; }
            //! Devuelve el numero de particulas a spawnear a la vez
            //! @returns nParticlesToSpawn
            unsigned int       GetNParticlesToSpawn() const { return nParticlesToSpawn; }
            //! Devuelve el tiempo de vida de las particulas
            //! @returns lifeSpan
            float              GetLifeSpan()          const { return lifeSpan; }
            //! Devuelve si el modo loop
            //! @returns loop
            bool               GetLoop()              const { return loop; }
            //! Devuelve el tipo de Spawner que es
            //! @returns spawnType
            //! @see SpawnType
            SpawnType          GetSpawnType()         const { return spawnType; }
            //! Devuelve el radio del spawner
            //! @returns radious
            float              GetRadious()           const { return radious; }
            //! Devuelve la orientacion del spawner
            //! @returns orientation
            glm::vec3          GetOrientation()       const { return orientation; }
            //! Devuelve el offset del spawner
            //! @returns offset
            glm::vec3          GetOffset()            const { return offset; }
            //! Devuelve los flags de los efectos del spawner
            //! @returns flags
            std::uint_fast8_t  GetFlags()             const { return flags; }
            //! Devuelve si ha empezado alguna vez
            //! @returns started
            bool               GetStarted()           const { return started; }
            //! Devuele si esta activado el modo de 1 iteracion
            //! @returns oneIterationMode
            bool               GetOneIterationMode()  const { return oneIterationMode; }



        private:

            void Update();
            //! Indica que ha muerto o revivido una particula
            //! @note puede ser 1 o -1
            void UpdateNumParticlesDead(int change) { numParticleDeads += change; }

            class CLParticle; //Forward declaration

            vector<shared_ptr<CLParticle>> particles;
            CLNode* node {nullptr};     //CLNode para poder acceder a su matriz modelo, vista, proyeccion y demas cosas utiles
            unsigned int nParticles { 10 }; //Numero de particulas que vamos a tener
            glm::vec3 speedDirection;
            glm::vec3 spawnerPosition;
            vector<CLResourceTexture*> clTextures;
            uint16_t width{0}, height{0};
            time_point<system_clock> timeStart;
            float spawnDelay = 1000; //Tiempo en ms
            unsigned int nParticlesToSpawn = 1; //Particulas a spawnear a la vez
            float lifeSpan = 1000; //Tiempo de vida de las particulas
            bool loop = false;
            bool started = false;
            bool oneIterationMode = false;
            uint32_t numParticleDeads = 0;
            SpawnType spawnType = SpawnType::PointSpawn;
            float radious = 0; //Usado en SpawnType::Circle y SpawnType::Sphere
            glm::vec3 orientation = glm::vec3(1.0f); //Usado en SpawnType::Circle
            glm::vec3 offset; //Usado en SpawnType::Line, SpawnType::Square y  SpawnType::Cube
            std::uint_fast8_t flags = 0x0;


            class CLParticle{

                public:
                    CLParticle() = default;
                    CLParticle(CLParticleSystem* emitter);
                    ~CLParticle() { glDeleteVertexArrays(1, &VAO);glDeleteBuffers(1, &VBO);};

                    void Draw(GLuint shaderID);
                    bool GetParticleDead() const { return particleDead; }
                    void ReviveParticle();

                private:
                    void Update();              //Aqui haremos los calculos necesarios de vida, posicion, etc
                    glm::vec3 CalculateSpawnPosition();
                    void CalculateVelocity();
                    
                    GLuint VBO,VAO;
                    float lifeTime = 0;
                    CLResourceTexture* texture;
                    CLParticleSystem* particleSystem {nullptr};
                    glm::vec3 position;
                    glm::vec3 velocity;
                    float lifeSpan = 1000;
                    time_point<system_clock> timeStart;
                    bool particleDead = false;


            };
            
    };
}