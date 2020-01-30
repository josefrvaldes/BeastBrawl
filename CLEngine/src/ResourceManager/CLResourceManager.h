#pragma once

#include <vector>
#include <string>

#include "CLResourceMesh.h"
#include "CLResourceShader.h"

namespace CLE {

    class CLResourceManager {
        public:
            CLResourceManager();
            ~CLResourceManager();

            CLResourceMesh* GetResourceMesh(const std::string);
            CLResourceShader* GetResourceShader(const std::string);

        private:

            std::vector<CLResourceMesh*> meshes { nullptr };
            std::vector<CLResourceShader*> shaders { nullptr };

    };

}