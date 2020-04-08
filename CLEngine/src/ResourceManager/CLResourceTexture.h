#pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>
#include <string>
#include <stb_image.h>

namespace CLE {
    
    
    class CLResourceTexture : public CLResource {
        public:
            CLResourceTexture() {};
            ~CLResourceTexture() {};

            void Draw(GLuint shaderID) override;
            bool LoadFile(string, bool) override;
            
            unsigned int GetTextureID() { return textureID; };
            int GetWidth(){ return widthText; };
            int GetHeight(){ return heightText; };

         private:
            unsigned int textureID;
            string type;
            int widthText = 0;
            int heightText = 0;
    };

}