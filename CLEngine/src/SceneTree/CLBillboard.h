#pragma once

#include "CLEntity.h"


namespace CLE {

    class CLBillboard : public CLEntity {
        public:
            CLBillboard(GLuint _lightId);
            ~CLBillboard() {};

            //void PrepareToRender();
            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);

            // variables
            GLuint lightId = 0;
            unsigned int depthMapFBO;
            static inline unsigned int depthCubemap;
            const unsigned int SHADOW_WIDTH = 1024;
            const unsigned int SHADOW_HEIGHT = 1024;
                  
    };
}