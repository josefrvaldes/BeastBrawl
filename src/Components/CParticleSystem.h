#pragma once


#include "Component.h"
#include <glm/vec3.hpp>
#include <string>
using namespace std;

class CParticleSystem : public Component{
public:
    CParticleSystem();
    CParticleSystem(unsigned long _nParticles, glm::vec3 _velocity, string _texture, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started);
    ~CParticleSystem() = default;

    unsigned long nParticles {0}; // Numero de particulas
    glm::vec3 velocity;           // Velocidad de las particulas
    string texture;               // Textura de la particula
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