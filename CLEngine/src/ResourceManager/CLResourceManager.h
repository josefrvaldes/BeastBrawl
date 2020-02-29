#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "../../include/glew/glew.h"

#include "CLResourceMesh.h"
#include "CLResourceShader.h"
#include "CLResourceTexture.h"

using namespace std;
namespace CLE {

    class CLResourceManager {
        public:
            CLResourceManager();
            ~CLResourceManager();

            CLResourceMesh* GetResourceMesh(const std::string);
            CLResourceTexture* GetResourceTexture(const std::string);
            CLResourceShader* GetResourceShader(const std::string, const std::string);

        private:

            vector<shared_ptr<CLResourceMesh>> meshes;
            vector<shared_ptr<CLResourceTexture>> textures;
            vector<shared_ptr<CLResourceShader>> shaders;

    };

}