#pragma once

#include "CLResource.h"

namespace CLE {
    
    class CLResourceShader : public CLResource {
        public:
            CLResourceShader() {};
            ~CLResourceShader() {};

            void Draw(glm::mat4&) override;
            bool LoadFile(std::string) override;

            void SetShaderType(GLenum type)   { shaderType = type; }

            GLenum GetShaderType()          const { return shaderType; }
            GLuint GetShaderID()            const { return shaderID;   }

        private:
            GLuint shaderID;
            GLenum shaderType;
    
    };

}