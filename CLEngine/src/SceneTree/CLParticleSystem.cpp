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
 
 
 #include "CLParticleSystem.h"
#include "CLNode.h"
#include "../../../src/Systems/Utils.h"
using namespace CLE;


CLParticleSystem::CLParticleSystem(unsigned int idEntity, unsigned int particlesNumber, glm::vec3 _speedDirection,vector<string> textures,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan, std::uint_fast8_t _flags) : CLEntity(idEntity){
    nParticles        = particlesNumber;
    speedDirection    = _speedDirection;
    for(string path : textures){
        clTextures.emplace_back(CLResourceManager::GetResourceManager()->GetResourceTexture(path,false));
    }
    //clTexture         = CLResourceManager::GetResourceManager()->GetResourceTexture(texture,false);
    width             = _width;
    height            = _height;
    spawnDelay        = _spawnDelay;
    nParticlesToSpawn = _nParticlesToSpawn;
    lifeSpan          = _lifeSpan;
    spawnType         = SpawnType::PointSpawn;
    flags             = _flags;

    particles.reserve(nParticles);

}

//Line, Square y Cube, depende del valor de _offset
CLParticleSystem::CLParticleSystem(unsigned int idEntity, unsigned int _nParticles, glm::vec3 _speedDirection,vector<string> textures,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,glm::vec3 _offset, glm::vec3 _orientation, std::uint_fast8_t _flags) 
: CLParticleSystem(idEntity,_nParticles,_speedDirection,textures,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan,_flags){

    offset = _offset;
    orientation = _orientation;
    if((offset.x != 0 && !offset.y && !offset.z) || (!offset.x && offset.y != 0 && !offset.z) || (!offset.x && !offset.y && offset.z != 0)){

        //Si solo tiene 1 valor es linea
        spawnType = SpawnType::LineSpawn;

    }else if((offset.x != 0 && offset.y != 0 && !offset.z) || (offset.x != 0 && !offset.y && offset.z != 0) || (!offset.x && offset.y != 0 && offset.z != 0)){

        //Si tiene 2 valores es Cuadrado
        spawnType = SpawnType::SquareSpawn;

    }else if( offset.x != 0 && offset.y != 0 && offset.z != 0){

        //Si tiene 3 valores es Cubo
        spawnType = SpawnType::CubeSpawn;

    }else{
        spawnType = SpawnType::PointSpawn;

    }
}

//Circle
CLParticleSystem::CLParticleSystem(unsigned int idEntity, unsigned int _nParticles, glm::vec3 _speedDirection,vector<string> textures,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, glm::vec3 _orientation, std::uint_fast8_t _flags)
: CLParticleSystem(idEntity,_nParticles,_speedDirection,textures,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan,_flags){

    //No lo estoy controlando porque es mi propio motor pero lo suyo es que solo pase 2 ejes en el _orientation
    spawnType = SpawnType::CircleSpawn;
    radious = _radious;
    orientation = _orientation;

}

//Sphere
CLParticleSystem::CLParticleSystem(unsigned int idEntity, unsigned int _nParticles, glm::vec3 _speedDirection,vector<string> textures,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, std::uint_fast8_t _flags)
: CLParticleSystem(idEntity,_nParticles,_speedDirection,textures,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan,_flags){

    spawnType = SpawnType::SphereSpawn;
    radious = _radious;

}



void CLParticleSystem::Draw(GLuint shaderID) {
    if(!started) return;
    //En este Draw que llama al resto de draws podriamos settear cosas generales para todos las particulas
    Update(); //Este Update se encarga de cosas como spawnear nuevas particulas

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDepthMask(GL_FALSE);
    glUseProgram(shaderID);



    for(auto& particle : particles){
        particle->Draw(shaderID);
    }
}

void CLParticleSystem::Update(){

    //cout << "Hay " << numParticleDeads << " particulas muertas de " << nParticles << endl;
    
    //Comprobamos si ha pasado el tiempo de spawn
    auto diff = duration_cast<milliseconds>(system_clock::now() - timeStart).count();

    if(diff > spawnDelay){
        timeStart = system_clock::now();

        for(unsigned int i = 0; i<nParticlesToSpawn && particles.size()<nParticles ; ++i){
            particles.emplace_back(make_shared<CLParticle>(this));
        }

        //Vamos a comprobar si tenemos que revivir alguna particula porque han activado de nuevo el loop
        if(loop || oneIterationMode){ 
            int particlesToRespawn = nParticlesToSpawn; //Solo vamos a revivir por iteracion a nParticlesToSpawn
            for(auto& particle : particles){
                if(particle->GetParticleDead()){
                    particle->ReviveParticle();
                    particlesToRespawn--;
                }

                if(particlesToRespawn <= 0) break;
            }
        }
    }


    if(numParticleDeads == 0){
        oneIterationMode = false;
    }

}

// -----------------------------------------
// ------------Clase CLParticle-------------
// -----------------------------------------

CLParticleSystem::CLParticle::CLParticle(CLParticleSystem* emitter){
    particleSystem = emitter;   
    position       = CalculateSpawnPosition();
    lifeSpan       = particleSystem->GetLifeSpan();
    velocity       = particleSystem->GetSpeedDirection();
    timeStart      = system_clock::now();

    int indxTexture = 0;
    if(particleSystem->GetTextures().size() > 1){
        indxTexture = Utils::getRandomInt(0,particleSystem->GetTextures().size()-1);
    }
    texture = particleSystem->GetTextures().at(indxTexture);
    //Comprobamos si el flag del effecto esta activado
    CalculateVelocity();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

}


void CLParticleSystem::CLParticle::Draw(GLuint shaderID){
    if(particleDead) return;
    Update();
    
    if(particleSystem->GetFlags() & EFFECT_FADING){
        glUniform1f(glGetUniformLocation(shaderID, "alpha"), glm::min(lifeTime / lifeSpan,1.0f));

    }else{
        glUniform1f(glGetUniformLocation(shaderID, "alpha"), 100);

    }
    glUniform1f(glGetUniformLocation(shaderID, "width"), particleSystem->GetWidth());
    glUniform1f(glGetUniformLocation(shaderID, "height"), particleSystem->GetHeight());
    
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, (void*)0);    
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void CLParticleSystem::CLParticle::Update(){
    //Comprobamos si deberia seguir con vida la particula
    lifeTime = duration_cast<milliseconds>(system_clock::now() - timeStart).count();

    if(lifeTime >= lifeSpan){
        lifeTime = lifeSpan;
        if(particleSystem->GetLoop()){
            //la respawneamos en la posicion inicial
            position = CalculateSpawnPosition();
            timeStart = system_clock::now();
        }else{
            //Si no hay loop al llegar a su lifeSpan la desactivamos y reposicionamos
            particleDead = true;
            particleSystem->UpdateNumParticlesDead(+1);
        }
        
    }else{
        //la movemos
        position.x += velocity.x * Constants::DELTA_TIME;
        position.y += velocity.y * Constants::DELTA_TIME;
        position.z += velocity.z * Constants::DELTA_TIME;
    }
}

glm::vec3 CLParticleSystem::CLParticle::CalculateSpawnPosition(){
    glm::vec3 newPosition(0.0f);
    SpawnType spawnType = particleSystem->GetSpawnType();
    glm::vec3 center = particleSystem->GetCLNode()->GetGlobalTranslation();

    
    if(spawnType == SpawnType::PointSpawn){
        newPosition = particleSystem->GetCLNode()->GetGlobalTranslation();
    }else if(spawnType == SpawnType::LineSpawn || spawnType == SpawnType::SquareSpawn || spawnType == SpawnType::CubeSpawn){
        float offX = Utils::getRandomFloat(center.x - particleSystem->GetOffset().x, center.x + particleSystem->GetOffset().x);//genX(rng);
        float offY = Utils::getRandomFloat(center.y - particleSystem->GetOffset().y, center.y + particleSystem->GetOffset().y);//genY(rng);
        float offZ = Utils::getRandomFloat(center.z - particleSystem->GetOffset().z, center.z + particleSystem->GetOffset().z);//genZ(rng);

        newPosition = glm::vec3(offX,offY,offZ);
    }else if(spawnType == SpawnType::SphereSpawn || spawnType == SpawnType::CircleSpawn){
        float radious = particleSystem->GetRadious();
        glm::vec3 orientation = particleSystem->GetOrientation();

        float offX = Utils::getRandomFloat(center.x - (radious*orientation.x), center.x + (radious*orientation.x));
        float offY = Utils::getRandomFloat(center.y - (radious*orientation.y), center.y + (radious*orientation.y));
        float offZ = Utils::getRandomFloat(center.z - (radious*orientation.z), center.z + (radious*orientation.z));

        newPosition = glm::vec3(offX,offY,offZ);
    }else{
        newPosition = particleSystem->GetCLNode()->GetGlobalTranslation();

    }
    
    
    return newPosition;
}

void CLParticleSystem::CLParticle::ReviveParticle(){
    particleDead = false;
    particleSystem->UpdateNumParticlesDead(-1);
    position = CalculateSpawnPosition();
    CalculateVelocity();
    timeStart = system_clock::now();
}

void CLParticleSystem::CLParticle::CalculateVelocity(){

    //Comprobamos si el flag del effecto esta activado
    if(particleSystem->GetFlags() & EFFECT_DIR_ALEATORITY){

        float valueX = 0;
        float valueY = 0;
        float valueZ = 0;

        do{
            valueX = Utils::getRandomFloat(-1.0,1.0);
            valueY = Utils::getRandomFloat(-1.0,1.0);
            valueZ = Utils::getRandomFloat(-1.0,1.0);
        }while(!valueX && !valueY && !valueZ);

        velocity.x = particleSystem->GetSpeedDirection().x * valueX;
        velocity.y = particleSystem->GetSpeedDirection().y * valueY;
        velocity.z = particleSystem->GetSpeedDirection().z * valueZ;

    }else{
        velocity = particleSystem->GetSpeedDirection();
    }

}