#include "CLGrass.h"
#include "../ResourceManager/CLResourceManager.h"



using namespace CLE;

CLGrass::CLGrass(const glm::vec3& _position) : position(_position){
    // transparent
    glGenVertexArrays(1, &transparentVAO); 
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    transparentTexture = static_cast<CLResourceTexture*>(CLResourceManager::GetResourceManager()->GetResourceTexture("media/grass2.png", false))->GetTextureID();
}

CLGrass::CLGrass(unsigned int idEntity) : CLEntity(idEntity){
    // transparent
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    transparentTexture = static_cast<CLResourceTexture*>(CLResourceManager::GetResourceManager()->GetResourceTexture("media/grass2.png", false))->GetTextureID();
}



void CLGrass::Draw(GLuint shaderID) {
    //glActiveTexture(GL_TEXTURE0); // a saber
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, transparentTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}


void CLGrass::Draw(GLuint shaderID, const glm::mat4& VPmatrix, const glm::mat4& auxScale) {
    glm::mat4 auxTrans = glm::mat4(1.0f);
    auxTrans = glm::translate(auxTrans, position);

    //glActiveTexture(GL_TEXTURE0); // a saber
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, transparentTexture);

    float degrees = 0.0f;
    for(unsigned int i = 0; i<3; i++){
        glm::mat4 auxRot = glm::mat4(1.0f);
        auxRot = glm::rotate(auxRot, glm::radians(degrees) , glm::vec3(0,1,0));
        auxRot = glm::rotate(auxRot, glm::radians(0.f) , glm::vec3(0,0,1));
        auxRot = glm::rotate(auxRot, glm::radians(0.f) , glm::vec3(1,0,0));

        glm::mat4 model = auxTrans * auxRot * auxScale;
        glm::mat4 MVP = VPmatrix * model;
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        degrees+=60.0;
    }
 

    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void CLGrass::DrawDepthMap(GLuint shaderID){

}
