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
 * @author Rubén Rubio Martínez <https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/> 
 * 
 */
 
 
 #pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>
#include <string>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

//! Estructura que engloba el material de un objeto
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

namespace CLE {
    
    //! Clase para almacenar materiales
    class CLResourceMaterial : public CLResource {
        public:
            CLResourceMaterial() {};
            ~CLResourceMaterial() {};

            void Draw(GLuint shaderID) override;
            bool LoadFile(std::string file, bool) override;
            //! Devuelve los materiales
            //! @returns materials std::vector<Material> materiales del recurso
            vector<Material> GetMaterials() { return materials;};

        private:
            void processNode(aiNode *node, const aiScene *scene);
            Material processMaterial(aiMesh *mesh, const aiScene *scene);
            Material loadMaterial(aiMaterial* mat); 
            std::vector<Material> materials;

            const aiScene *scene;

    };

}