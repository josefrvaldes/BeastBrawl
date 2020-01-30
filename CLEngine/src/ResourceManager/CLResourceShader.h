#pragma once

#include "CLResource.h"
#include "../../include/glew/glew.h"

namespace CLE {
    
    class CLResourceShader : public CLResource {
        public:
            CLResourceShader() {}
            ~CLResourceShader();

            void Draw();
            bool LoadFile(std::string);

            void SetShaderName(GLenum &e)   { shaderName = e; }

            GLenum GetShaderName()          const { return shaderName; }

        private:

            GLenum shaderName;
    
    };

}