#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"
#include <glm/glm.hpp>



namespace CLE {

    class CLGrass : public CLEntity {
        public:
            CLGrass(const glm::vec3& _position);
            CLGrass(unsigned int);
            ~CLGrass() {};

            void Draw(GLuint shaderID);
            void Draw(GLuint shaderID, const glm::mat4& VPmatrix, const glm::mat4& auxScale);
            void DrawDepthMap(GLuint shaderID);

            //unsigned int skyboxID, skyboxVAO, skyboxVBO;
            unsigned int transparentVAO, transparentVBO;
            unsigned int planeVAO, planeVBO;
            unsigned int transparentTexture;

        private:
            float transparentVertices[30] = {
                // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
                -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, 0.0f,  0.0f,  1.0f,  1.0f,

                -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,
                0.5f, 0.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  1.0f,  0.0f,  1.0f,  0.0f
            };

            glm::vec3 position = glm::vec3(1.0f);

    };
}