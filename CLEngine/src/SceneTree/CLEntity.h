#pragma once

#include <glm/glm.hpp>
#include "../../include/glew/glew.h"

namespace CLE {

    class CLEntity {
        public:
            CLEntity(){};
            CLEntity(unsigned int idEntity) : id(idEntity) {}
            ~CLEntity(){};

            unsigned int GetID() { return id; }

            virtual void Draw (GLuint) = 0;
            virtual void DrawDepthMap (GLuint) = 0;

        protected:

            unsigned int id;

    };
}
