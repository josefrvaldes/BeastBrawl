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

    for(auto& particle : particles){
        particle->Draw(shaderID);
    }
}


// -----------------------------------------
// ------------Clase CLParticle-------------
// -----------------------------------------

CLParticleSystem::CLParticle::CLParticle(CLParticleSystem* emitter){
    particleSystem = emitter;


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

}

void CLParticleSystem::CLParticle::Draw(GLuint shaderID){
    Update();

    glUseProgram(shaderID);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(particleSystem->GetCLNode()->GetViewMatrix()));

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(particleSystem->GetCLNode()->GetProjectionMatrix()));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(50.0f,100.0f,-80.0f));

    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void CLParticleSystem::CLParticle::Update(){

}