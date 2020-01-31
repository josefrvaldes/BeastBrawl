#pragma once

#include "CLResource.h"

namespace CLE {
    
    class CLResourceShader : public CLResource {
        public:
            CLResourceShader() {}
            ~CLResourceShader();

            void Draw(glm::mat4&);
            bool LoadFile(std::string);

            void SetShaderName(std::string &e)   { shaderName = e; }

            std::string GetShaderName()          const { return shaderName; }

        private:

            std::string shaderName;
    
    };

}