#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "../../include/glew/glew.h"

#include "CLResourceMesh.h"
#include "CLResourceShader.h"
#include "CLResourceTexture.h"
#include "CLResourceMaterial.h"

using namespace std;
namespace CLE {

    class CLResourceManager {
        public:
            ~CLResourceManager(){delete instance;};
            static CLResourceManager* GetResourceManager(){
                if(!instance){
                    instance = new CLResourceManager();
                }
                return instance;
            }

            CLResourceMesh* GetResourceMesh(const std::string, bool);
            CLResourceMesh* GetResourceMesh(const std::string);
            CLResourceTexture* GetResourceTexture(const std::string, bool);
            CLResourceTexture* GetResourceTexture(const std::string);
            CLResourceMaterial* GetResourceMaterial(const std::string, bool);
            CLResourceMaterial* GetResourceMaterial(const std::string);
            CLResourceShader* GetResourceShader(const std::string, const std::string);
            CLResourceShader* GetResourceShader(const std::string, const std::string, const std::string);

        private:
            CLResourceManager();
            inline static CLResourceManager* instance { nullptr };
            vector<shared_ptr<CLResourceMesh>> meshes;
            vector<shared_ptr<CLResourceTexture>> textures;
            vector<shared_ptr<CLResourceMaterial>> materials;
            vector<shared_ptr<CLResourceShader>> shaders;

    };

}