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
 
 
 #pragma once


#include "Component.h"
#include <glm/vec3.hpp>
#include <string>
#include <vector>
using namespace std;

class CParticleSystem : public Component{
public:
    CParticleSystem();
    CParticleSystem(uint32_t _nParticles, glm::vec3 _velocity, vector<string> _textures, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started);
    ~CParticleSystem() = default;

    uint32_t nParticles {0}; // Numero de particulas
    glm::vec3 velocity;           // Velocidad de las particulas
    vector<string> textures;               // Textura de la particula
    int width {50};               // Anchura de la particula
    int height{50};               // Altura de la particula
    int spawnDelay {100};         // Tiempo de spawn entre particulas
    int particlesToSpawn{1};      // Particulas a spawnear cada ciclo
    int lifeSpan {2000};          // Tiempo de vida
    glm::vec3 offset;             // Dimensiones del shape
    glm::vec3 orientation;        // Orientacion del shape
    float radious {0};            // Radio del shape
    std::uint_fast8_t flags;      // Efectos
    bool loop { false };          // Efecto loop
    bool started { false };       // Si esta iniciado por defecto
};