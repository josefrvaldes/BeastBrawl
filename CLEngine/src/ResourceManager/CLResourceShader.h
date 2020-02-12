#pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>

namespace CLE {
    
    class CLResourceShader : public CLResource {
        public:
            CLResourceShader() {};
            ~CLResourceShader() {};

            void Draw(glm::mat4) override;
            bool LoadFile(string) override;

            void SetShaderType(GLenum type)   { shaderType = type; }

            GLenum GetShaderType()          const { return shaderType; }
            GLuint GetShaderID()            const { return shaderID;   }

        private:
            GLuint shaderID;
            GLenum shaderType;
    
    };

}