#include "CLBillboard.h"



CLE::CLBillboard::CLBillboard(unsigned int id,CLResourceTexture* texture_, float width_, float height_) : CLEntity(id){
    texture = texture_;
    width = width_;
    height = height_;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexBuffer);
}

void CLE::CLBillboard::Draw(GLuint shaderID){
    if(texture != nullptr){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1f(glGetUniformLocation(shaderID, "width"), width);
        glUniform1f(glGetUniformLocation(shaderID, "height"), height);

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, (void*)0);    
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        //glDisableVertexAttribArray(0);

    }
}

