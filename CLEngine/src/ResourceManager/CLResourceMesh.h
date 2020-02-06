#pragma once

#include "CLResource.h"
#include "../SceneTree/CLMesh.h"

#include "../../include/assimp/Importer.hpp"
#include "../../include/assimp/scene.h"
#include "../../include/assimp/postprocess.h"
#include <vector>


namespace CLE {
    class CLResourceMesh : public CLResource {
        public:
            CLResourceMesh(){};
            ~CLResourceMesh(){};

            void Draw(glm::mat4&) override;
            bool LoadFile(std::string) override;

        private:
            void processNode(aiNode *node, const aiScene *scene);
            CLMesh processMesh(aiMesh *mesh, const aiScene *scene)

            
    };
}