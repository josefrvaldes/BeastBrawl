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
            bool LoadFile(string) override;
            
            unsigned int GetTextureID() { return textureID; };

        private:
            unsigned int textureID;
            string type;
    };

}