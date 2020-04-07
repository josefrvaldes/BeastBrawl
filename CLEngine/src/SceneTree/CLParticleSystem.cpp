#include "CLParticleSystem.h"
#include "CLNode.h"
using namespace CLE;


CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong particlesNumber, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,uint16_t _nParticlesToSpawn,float _lifeSpan) : CLEntity(idEntity){
    nParticles        = particlesNumber;
    speedDirection    = _speedDirection;
    clTexture         = CLResourceManager::GetResourceManager()->GetResourceTexture(texture,false);
    width             = _width;
    height            = _height;
    spawnDelay        = _spawnDelay;
    nParticlesToSpawn = _nParticlesToSpawn;
    lifeSpan          = _lifeSpan;

    particles.reserve(nParticles);

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

        for(uint16_t i = 0; i<nParticlesToSpawn && particles.size()+nParticlesToSpawn<=nParticles ; ++i){
            particles.emplace_back(make_shared<CLParticle>(this));
        }
    }
}

void CLParticleSystem::SetCLNode(CLNode* clnode){
    node = clnode;
}
// -----------------------------------------
// ------------Clase CLParticle-------------
// -----------------------------------------

CLParticleSystem::CLParticle::CLParticle(CLParticleSystem* emitter){
    particleSystem = emitter;   
    position       = particleSystem->GetCLNode()->GetGlobalTranslation();
    lifeSpan       = particleSystem->GetLifeSpan();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

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

void CLParticleSystem::CLParticle::Update(){
    //Comprobamos si deberia seguir con vida la particula
    auto diff = duration_cast<milliseconds>(system_clock::now() - timeStart).count();
    if(diff >= lifeSpan){
        //la respawneamos en la posicion inicial
        position = particleSystem->GetCLNode()->GetGlobalTranslation();
        timeStart = system_clock::now();
    }else{
        //la movemos
        position.x += particleSystem->GetSpeedDirection().x * Constants::DELTA_TIME;
        position.y += particleSystem->GetSpeedDirection().y * Constants::DELTA_TIME;
        position.z += particleSystem->GetSpeedDirection().z * Constants::DELTA_TIME;
    }
}