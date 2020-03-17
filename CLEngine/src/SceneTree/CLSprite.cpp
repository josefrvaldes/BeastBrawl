#include "CLSprite.h"


void CLE::CLSprite::initRenderData() {
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
            // Pos      // Tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void CLE::CLSprite::Draw(GLuint shaderID) {
    // Prepare transformations
    //glUseProgram(shaderID);
    //glm::mat4 model;
    //model = glm::translate(model, glm::vec3(200.0f, 200.0f, 0.0f));

    //model = glm::translate(model, glm::vec3(0.5f * 300.0f, 0.5f * 400.0f, 0.0f));
    //model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::translate(model, glm::vec3(-0.5f * 300.0f, -0.5f * 400.0f, 0.0f));

    //model = glm::scale(model, glm::vec3(300.0f, 400.0f, 1.0f));

    //glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderID, "spriteColor"), 0.0f, 1.0f, 0.0f);

    glActiveTexture(GL_TEXTURE0);
    texture->Draw(shaderID);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    //cout << "ME DIBUJAO :c" << endl;
}
