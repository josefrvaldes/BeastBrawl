#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceMesh.h"


namespace CLE {

    class CLMesh : public CLEntity {
        public:
            CLMesh();
            CLMesh(unsigned int idEntity) : CLEntity(idEntity) {};
            ~CLMesh() {};

            void SetMesh(CLResourceMesh* m)         { mesh = m; }

            //GETTERS
            CLResource* GetMesh() const             { return mesh; }

            void Draw(glm::mat4&);


        private:
            CLResource* mesh;

            
    };
}