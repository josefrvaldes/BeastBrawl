/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author  Antonio Jose Martinez Garcia <https://www.linkedin.com/in/antonio-jose-martinez-garcia/>
 * 
 */
 
 
 #pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "../ResourceManager/CLResourceTexture.h"


namespace CLE {

    class CLGrassSystem{
        public:
            CLGrassSystem(float width, float height, const glm::vec3& position, const glm::vec3& scale, bool realistGrass); // cuadrado
            CLGrassSystem(float radious, const glm::vec3& position, const glm::vec3& scale, bool realistGrass);     // circular
            ~CLGrassSystem() {};

            void CalculateNumBushes();
            void CreateGrass();
            void CreateRealistGrass();

            void CreateGrassCircle();
            void CreateRealistGrassCircle();

            void ConfigureBuffers();
            void Draw(GLuint shaderID, const glm::mat4& projection, const glm::mat4& view);

            glm::vec3 GetPosition() { return position; };
            float GetSize();

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

            int extraScaleRealistic;
            int extraPositionRealistic = 20;
            float wingTimer = 0.0;

            // buffers
            unsigned int bufferModelVBO, bufferPosVBO;
            unsigned int planeVAO;

            // 6 vertices de la textura de la hierva
            /*float grassVertices[30] = {
                // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
                -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, 0.0f,  0.0f,  1.0f,  1.0f,

                -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,
                0.5f, 0.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  1.0f,  0.0f,  1.0f,  0.0f
            };*/

    };
}