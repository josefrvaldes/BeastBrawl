#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"


namespace CLE {

    class CLBillboard : public CLEntity {
        public:
            CLBillboard(CLResourceTexture* texture_, glm::vec3 posBillBoard_, float width_, float height_);
            ~CLBillboard() {};

            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);


            CLResourceTexture* getTexture(){ return texture;};
            glm::vec3 getPosition(){ return position;};
            void setTexture(CLResourceTexture* t) { texture = t;};
            void setPosition(glm::vec3 p) { position = p;};

        private:
            CLResourceTexture*  texture = nullptr;
            glm::vec3 position;
            GLuint vertexBuffer {0}, VAO {0};
            float width {0}, height {0};
                  
    };
}