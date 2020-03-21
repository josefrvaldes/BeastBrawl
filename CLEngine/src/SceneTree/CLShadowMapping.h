#pragma once

#include "CLEntity.h"


namespace CLE {

    class CLShadowMapping : public CLEntity {
        public:
            CLShadowMapping();
            ~CLShadowMapping() {};

            //void PrepareToRender();
            void Draw(GLuint shaderID);

            // variables
            static inline unsigned int depthMapFBO, depthCubemap;
            const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
                  
    };
}