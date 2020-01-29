#pragma once

#include "CLEntity.h"

namespace CLE {

    class CLMesh : public CLEntity {
        public:
            CLMesh() {};
            CLMesh(unsigned int idEntity) : CLEntity(idEntity) {};
            ~CLMesh() {};

            //SetMesh

            //GetMesh

            void Draw(glm::mat4&);

        protected:

            //Resource* mesh

    };
}