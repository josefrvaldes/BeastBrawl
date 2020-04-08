#include "CLParticleSystem.h"
#include "CLNode.h"
using namespace CLE;


CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong particlesNumber, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan) : CLEntity(idEntity){
    nParticles        = particlesNumber;
    speedDirection    = _speedDirection;
    clTexture         = CLResourceManager::GetResourceManager()->GetResourceTexture(texture,false);
    width             = _width;
    height            = _height;
    spawnDelay        = _spawnDelay;
    nParticlesToSpawn = _nParticlesToSpawn;
    lifeSpan          = _lifeSpan;
    spawnType         = SpawnType::Point;

    particles.reserve(nParticles);

}

//Line, Square y Cube, depende del valor de _offset
CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,glm::vec3 _offset, glm::vec3 _orientation) 
: CLParticleSystem(idEntity,_nParticles,_speedDirection,texture,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan){

    offset = _offset;
    orientation = _orientation;
    if((offset.x != 0 && !offset.y && !offset.z) || (!offset.x && offset.y != 0 && !offset.z) || (!offset.x && !offset.y && offset.z != 0)){

        //Si solo tiene 1 valor es linea
        spawnType = SpawnType::Line;
        cout << "Soy una linea\n";

    }else if((offset.x != 0 && offset.y != 0 && !offset.z) || (offset.x != 0 && !offset.y && offset.z != 0) || (!offset.x && offset.y != 0 && offset.z != 0)){

        //Si tiene 2 valores es Cuadrado
        spawnType = SpawnType::Square;
        cout << "Soy un cuadrado\n";
    }else if( offset.x != 0 && offset.y != 0 && offset.z != 0){

        //Si tiene 3 valores es Cubo
        spawnType = SpawnType::Cube;
        cout << "Soy un cubo\n";
    }else{
        spawnType = SpawnType::Point;
        cout << "Soy un punto\n";

    }
}

//Circle
CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, glm::vec3 _orientation)
: CLParticleSystem(idEntity,_nParticles,_speedDirection,texture,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan){

    spawnType = SpawnType::Circle;
    radious = _radious;
    orientation = _orientation;
    cout << "Soy un circulo\n";

}

//Sphere
CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious)
: CLParticleSystem(idEntity,_nParticles,_speedDirection,texture,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan){

    spawnType = SpawnType::Sphere;
    radious = _radious;
    cout << "Soy una esfera\n";

}



void CLParticleSystem::Draw(GLuint shaderID) {
    //En este Draw que llama al resto de draws podriamos settear cosas generales para todos las particulas
    Update(); //Este Update se encarga de cosas como spawnear nuevas particulas

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDepthMask(GL_FALSE);
    glUseProgram(shaderID);

    glm::mat4 viewProjection = node->GetProjectionMatrix()*node->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "VPMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjection));

    glm::vec3 camPos = node->GetActiveCameraNode()->GetGlobalTranslation();
    glUniform3fv(glGetUniformLocation(shaderID, "cameraPosition"), 1, glm::value_ptr(camPos));


    for(auto& particle : particles){
        particle->Draw(shaderID);
    }
}

void CLParticleSystem::Update(){
    //Comprobamos si ha pasado el tiempo de spawn
    auto diff = duration_cast<milliseconds>(system_clock::now() - timeStart).count();

    if(diff > spawnDelay){
        timeStart = system_clock::now();

        for(unsigned int i = 0; i<nParticlesToSpawn && particles.size()+nParticlesToSpawn<=nParticles ; ++i){
            particles.emplace_back(make_shared<CLParticle>(this));
        }
    }
}

// -----------------------------------------
// ------------Clase CLParticle-------------
// -----------------------------------------

CLParticleSystem::CLParticle::CLParticle(CLParticleSystem* emitter){
    particleSystem = emitter;   
    position       = CalculateSpawnPosition();
    lifeSpan       = particleSystem->GetLifeSpan();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

}



void CLParticleSystem::CLParticle::Update(){
    //Comprobamos si deberia seguir con vida la particula
    auto diff = duration_cast<milliseconds>(system_clock::now() - timeStart).count();
    if(diff >= lifeSpan){
        //la respawneamos en la posicion inicial
        position = CalculateSpawnPosition();
        timeStart = system_clock::now();
    }else{
        //la movemos
        position.x += particleSystem->GetSpeedDirection().x * Constants::DELTA_TIME;
        position.y += particleSystem->GetSpeedDirection().y * Constants::DELTA_TIME;
        position.z += particleSystem->GetSpeedDirection().z * Constants::DELTA_TIME;
    }
}

glm::vec3 CLParticleSystem::CLParticle::CalculateSpawnPosition(){
    glm::vec3 newPosition(0.0f);
    SpawnType spawnType = particleSystem->GetSpawnType();
    glm::vec3 center = particleSystem->GetCLNode()->GetGlobalTranslation();

    std::random_device rd;
    std::mt19937 rng(rd());


    if(spawnType == SpawnType::Point){
        newPosition = particleSystem->GetCLNode()->GetGlobalTranslation();
    }else if(spawnType == SpawnType::Line || spawnType == SpawnType::Square || spawnType == SpawnType::Cube){
        std::uniform_real_distribution<float> genX(center.x - particleSystem->GetOffset().x, center.x + particleSystem->GetOffset().x);
        std::uniform_real_distribution<float> genY(center.y - particleSystem->GetOffset().y, center.y + particleSystem->GetOffset().y);
        std::uniform_real_distribution<float> genZ(center.z - particleSystem->GetOffset().z, center.z + particleSystem->GetOffset().z);
        float offX = genX(rng);
        float offY = genY(rng);
        float offZ = genZ(rng);

        //Reset de numeros aleatorios
        genX(rng);genX(rng);genX(rng);
        genY(rng);genY(rng);genY(rng);
        genZ(rng);genZ(rng);genZ(rng);


        newPosition = glm::vec3(offX,offY,offZ);
    }
    
    
    return newPosition;
}


void CLParticleSystem::CLParticle::Draw(GLuint shaderID){
    Update();
    
    glUniform1f(glGetUniformLocation(shaderID, "width"), particleSystem->GetWidth());
    glUniform1f(glGetUniformLocation(shaderID, "height"), particleSystem->GetHeight());
    
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, particleSystem->GetTexture()->GetTextureID());
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, (void*)0);    
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}