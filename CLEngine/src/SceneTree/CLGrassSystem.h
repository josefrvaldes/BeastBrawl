#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "../ResourceManager/CLResourceTexture.h"


namespace CLE {

    class CLGrassSystem{
        public:
            CLGrassSystem(float width, float height, const glm::vec3& position, const glm::vec3& scale, bool realistGrass);
            ~CLGrassSystem() {};

            void CalculateNumBushes();
            void CreateGrass();
            void CreateRealistGrass();
            void Draw(GLuint shaderID, const glm::mat4& projection, const glm::mat4& view);
            void ConfigureBuffers();

        private:
            void AddLeafs(const glm::vec3& posLeaf, const glm::vec3& scaleLeaf);

            float width = 0;
            float height = 0;
            glm::vec3 position = glm::vec3(1.0f);
            glm::vec3 scale = glm::vec3(1.0f);

            string fileGrass = "media/grass2.png";
            CLResourceTexture* grassTexture = nullptr; // grassTexture->GetTextureID()
            
            unsigned int numBushesFiles = 0;
            unsigned int numBushesRows = 0;
            float sizeGrass = 0;
            float extraSize = 0;
            std::vector<glm::mat4> modelLeafVector;
            std::vector<glm::vec3> posLeafVector;

            int extraScaleRealistic = 20;
            int extraPositionRealistic = 20;
            float wingTimer = 0.0;

            // buffers
            unsigned int bufferVBO, bufferPosVBO;
            unsigned int planeVAO, planeVBO;

            // 6 vertices de la textura de la hierva
            float grassVertices[30] = {
                // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
                -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, 0.0f,  0.0f,  1.0f,  1.0f,

                -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,
                0.5f, 0.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  1.0f,  0.0f,  1.0f,  0.0f
            };

    };
}