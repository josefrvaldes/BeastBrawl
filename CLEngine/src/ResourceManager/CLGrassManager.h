#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "../SceneTree/CLGrass.h"
#include "../ResourceManager/CLResourceTexture.h"


namespace CLE {

    class CLGrassManager{
        public:
            CLGrassManager(float width, float height, const glm::vec3& position, const glm::vec3& scale);
            ~CLGrassManager() {};

            void CalculateNumBushes();
            void CreateGrass();
            void Draw(GLuint shaderID, const glm::mat4& projection, const glm::mat4& view);

        private:
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
            std::vector<std::unique_ptr<CLGrass>> bushes;

    };
}