#include "CLGrass.h"
#include "../ResourceManager/CLResourceManager.h"



using namespace CLE;

CLGrass::CLGrass(){
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

    transparentTexture = static_cast<CLResourceTexture*>(CLResourceManager::GetResourceManager()->GetResourceTexture("media/grass.png", false))->GetTextureID();
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

    transparentTexture = static_cast<CLResourceTexture*>(CLResourceManager::GetResourceManager()->GetResourceTexture("media/grass.png", false))->GetTextureID();
}




void CLGrass::Draw(GLuint shaderID) {
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, transparentTexture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    /*glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);*/

    //glBindVertexArray(vegetationVAO);
    //glBindTexture(GL_TEXTURE_2D, shaderID);  

    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, position);				
    //shader.setMat4("model", model);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CLGrass::DrawDepthMap(GLuint shaderID){

}
