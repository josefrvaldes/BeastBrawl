#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"
#include "../ResourceManager/CLResourceMaterial.h"


namespace CLE {

    class CLMesh : public CLEntity {
        public:
            CLMesh();
            CLMesh(unsigned int idEntity) : CLEntity(idEntity) {};
            ~CLMesh() {};

            void SetMesh(CLResourceMesh* m)                 { mesh = m; }
            void SetMaterial(CLResourceMaterial* m)         { material = m; }

            //GETTERS
            CLResource* GetMesh() const             { return mesh; }

            void Draw(GLuint shaderID);


        private:
            CLResourceMesh* mesh = nullptr;
            CLResourceMaterial* material = nullptr;
            CLResourceTexture* texture = nullptr;

            
    };
}