#pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>

namespace CLE {
    
    class CLResourceShader : public CLResource {
        public:
            CLResourceShader() {};
            ~CLResourceShader() {};

            void Draw(GLuint shaderID) override;
            bool LoadFile(string) override;
            bool LoadFile(string,string);
            bool LoadFile(string,string,string);

            int GetProgramID()                  const { return programID;   }

        private:
            bool LoadShader(string, GLenum);
            bool LinkShaders();
            bool LinkShadersGeometry();
            GLuint vertexID;
            GLuint fragmentID;
            GLuint geometryID;
            int programID;
            //GLenum shaderType;
    
    };

}