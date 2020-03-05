#pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>

namespace CLE {
    
    class CLResourceTexture : public CLResource {
        public:
            CLResourceTexture() {};
            ~CLResourceTexture() {};

            void Draw(GLuint shaderID) override;
            bool LoadFile(string) override;


        private:
    
    };

}