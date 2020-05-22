#pragma once

#include "CLEntity.h"


namespace CLE {
    //! Clase para añadir sombras en el motor
    class CLShadowMapping : public CLEntity {
        public:
            CLShadowMapping(GLuint _lightId);
            ~CLShadowMapping() {};

            //void PrepareToRender();
            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);

            // variables
            //GLuint lightId = 0;
            unsigned int depthMapFBO;
            static inline unsigned int depthMap;
            const unsigned int SHADOW_WIDTH = 1024;
            const unsigned int SHADOW_HEIGHT = 1024;
                  
    };
}