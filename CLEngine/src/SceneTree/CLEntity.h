#pragma once

#include <glm/glm.hpp>

namespace CLE {

    class CLEntity {
        public:
            CLEntity(){};
            CLEntity(unsigned int idEntity) : id(idEntity) {}
            ~CLEntity(){};

            unsigned int GetID() { return id; }

            virtual void Draw (glm::mat4&) = 0;

        protected:

            unsigned int id;

    };
}
