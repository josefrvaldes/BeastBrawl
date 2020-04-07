#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"


namespace CLE {

    class CLGrass : public CLEntity {
        public:
            CLGrass();
            CLGrass(unsigned int);
            ~CLGrass() {};

            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);

            //unsigned int skyboxID, skyboxVAO, skyboxVBO;
            unsigned int transparentVAO, transparentVBO;
            unsigned int planeVAO, planeVBO;
            unsigned int transparentTexture;

        private:
            float transparentVertices[30] = {
                // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
                0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
                0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
                1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

                0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
                1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
                1.0f,  0.5f,  0.0f,  1.0f,  0.0f
            };

    };
}