#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceTexture.h"

#include <glm/gtc/type_ptr.hpp>


namespace CLE {

    class CLSprite : public CLEntity {
        public:
            CLSprite() = default;
            CLSprite(unsigned int idEntity) : CLEntity(idEntity) {};
            ~CLSprite() = default;

            //SETTERS
            void SetTexture(CLResourceTexture* t)         { texture = t; }

            //GETTERS
            CLResource* GetTexture() const                { return texture; }

            void Draw(GLuint shaderID, CLResourceTexture&, glm::vec2, glm::vec2, GLfloat, glm::vec3);


        private:

            GLuint quadVAO;
            CLResourceTexture* texture;

            void initRenderData();
            
    };
}