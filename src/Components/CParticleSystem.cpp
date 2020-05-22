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
 
 
 #include "CParticleSystem.h"

CParticleSystem::CParticleSystem(){
    m_compType = CompType::ParticleSystemComp;
}

CParticleSystem::CParticleSystem(uint32_t _nParticles, glm::vec3 _velocity, vector<string> _textures, int _width, int _height, int _spawnDelay, int _particlesToSpawn, int _lifeSpan, glm::vec3 _offset, glm::vec3 _orientation, float _radious, std::uint_fast8_t _flags, bool _loop, bool _started){
 
    m_compType       = CompType::ParticleSystemComp;
    nParticles       = _nParticles;
    velocity         = _velocity;
    textures         = _textures;
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