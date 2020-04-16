#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"
#include "../ResourceManager/CLResourceMaterial.h"


namespace CLE {
    //! Clase para crear mallas
    //! Clase que permite crear mallas en nuestro motor gráfico
    class CLMesh : public CLEntity {
        public:
            CLMesh();
            CLMesh(unsigned int idEntity) : CLEntity(idEntity) {};
            ~CLMesh() {};

            //! Asigna una malla
            //! @param m Objecto CLResourceMesh donde tiene la malla
            void SetMesh(CLResourceMesh* m)                 { mesh = m; }
            //! Asigna un material
            //! @param m Objeto CLResourceMaterial donde tiene el material
            void SetMaterial(CLResourceMaterial* m)         { material = m; }

            //GETTERS
            //! Devuelve la malla
            //! @returns mesh Objeto CLResourceMesh almacenado
            CLResourceMesh* GetMesh() const             { return mesh; }

            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);


        private:
            CLResourceMesh* mesh = nullptr;
            CLResourceMaterial* material = nullptr;
            CLResourceTexture* texture = nullptr;

            
    };
}