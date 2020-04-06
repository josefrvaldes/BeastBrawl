#include "CLParticleSystem.h"

using namespace CLE;


CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong particlesNumber, glm::vec3 _speedDirection) : CLEntity(idEntity){
    nParticles = particlesNumber;
    speedDirection = _speedDirection;

    particles.reserve(nParticles);

    for(ulong i = 0; i<nParticles; ++i){
        particles.emplace_back(make_shared<CLParticle>(this));
    }
}

void CLParticleSystem::Draw(GLuint shaderID) {
    //Aqui llamaremos al draw de cada CLParticle de nustros vector<CLParticle> particles

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


// -----------------------------------------
// ------------Clase CLParticle-------------
// -----------------------------------------

CLParticleSystem::CLParticle::CLParticle(CLParticleSystem* emitter){
    particleSystem = emitter;



}

void CLParticleSystem::CLParticle::Draw(GLuint shaderID){
    Update();

    
}

void CLParticleSystem::CLParticle::Update(){

}