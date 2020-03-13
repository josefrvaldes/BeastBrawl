#pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>
#include <string>
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CLE {
    
    
    class CLResourceMaterial : public CLResource {
        public:
            CLResourceMaterial() {};
            ~CLResourceMaterial() {};

            void Draw(GLuint shaderID) override;
            bool LoadFile(std::string file) override;


        private:
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shinnines;
    };

}