#include "CLBillboard.h"



CLE::CLBillboard::CLBillboard(CLResourceTexture* texture_, glm::vec3 posBillBoard_){
    texture = texture_;
    position = posBillBoard_;
    glGenBuffers(1, &vertexBuffer);
}

void CLE::CLBillboard::Draw(GLuint shaderID){
    if(texture != nullptr){

        auto textureID = texture->GetTextureID();
        glBindTexture(GL_TEXTURE_2D, textureID);

        //texture->Draw(shaderID);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_POINTS, 0, 1);

        glDisableVertexAttribArray(0);

    }
}

void CLE::CLBillboard::DrawDepthMap(GLuint shaderID){

}