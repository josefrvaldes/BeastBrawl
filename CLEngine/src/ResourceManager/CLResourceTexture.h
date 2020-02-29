#pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>

namespace CLE {
    
    class CLResourceTexture : public CLResource {
        public:
            CLResourceTexture() {};
            ~CLResourceTexture() {};

            void Draw(glm::mat4) override;
            bool LoadFile(string) override;


        private:
    
    };

}