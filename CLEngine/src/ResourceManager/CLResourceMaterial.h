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


struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

namespace CLE {
    
    
    class CLResourceMaterial : public CLResource {
        public:
            CLResourceMaterial() {};
            ~CLResourceMaterial() {};

            void Draw(GLuint shaderID) override;
            bool LoadFile(std::string file) override;
            vector<Material> GetMaterials() { return materials;};

        private:
            void processNode(aiNode *node, const aiScene *scene);
            Material processMaterial(aiMesh *mesh, const aiScene *scene);
            Material loadMaterial(aiMaterial* mat); 
            vector<Material> materials;

            const aiScene *scene;

    };

}