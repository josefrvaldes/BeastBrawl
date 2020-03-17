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
        [[nodiscard]] CLResource* GetTexture() const                { return texture; }

        void Draw(GLuint shaderID) override;


    private:

        GLuint quadVAO {0};
        CLResourceTexture* texture {nullptr};

        void initRenderData();

    };
}