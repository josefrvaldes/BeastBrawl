#pragma once

#include "CLEntity.h"


namespace CLE {

    class CLShadowMapping : public CLEntity {
        public:
            CLShadowMapping();
            ~CLShadowMapping() {};

            // variables
            unsigned int depthMapFBO, depthMap;
            const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
                  
    };
}