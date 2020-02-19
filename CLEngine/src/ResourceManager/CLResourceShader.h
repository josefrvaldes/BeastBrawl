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
            bool LoadFile(string,string);


            GLuint GetVertexID()            const { return vertexID;   }
            GLuint GetFragmentID()            const { return fragmentID;   }
            int GetProgramID()            const { return programID;   }

        private:
            bool LoadShader(string, GLenum);
            bool LinkShaders();
            GLuint vertexID;
            GLuint fragmentID;
            int programID;
            //GLenum shaderType;
    
    };

}