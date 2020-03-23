#pragma once

#include "CLEntity.h"


namespace CLE {

    class CLShadowMapping : public CLEntity {
        public:
            CLShadowMapping();
            ~CLShadowMapping() {};

            //void PrepareToRender();
            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);

            // variables
            unsigned int depthMapFBO;
            static inline unsigned int depthCubemap;
            const unsigned int SHADOW_WIDTH = 1024;
            const unsigned int SHADOW_HEIGHT = 1024;
                  
    };
}