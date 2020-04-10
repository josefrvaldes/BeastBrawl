#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"


namespace CLE {
    //! Clase para añadir billboards
    //! @see Esta clase nos permite añadir billboards en nuestro arbol de la escena CLE::CLNode::AddBillboard(string&,bool,glm::vec3,floatfloat)
    class CLBillboard : public CLEntity {
        public:
            //! Constructor de Billboards
            //! @param texture_ Objecto CLResourceTexture para la textura del billboard
            //! @param posBillboard_ Posición del billboard
            //! @param width_ Anchura del billboard
            //! @param height Altura del billboard
            CLBillboard(CLResourceTexture* texture_, glm::vec3 posBillBoard_, float width_, float height_);
            ~CLBillboard() {};

            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);

            //! Devuelve la textura
            //! @returns texture El objeto CLResourceTexture
            CLResourceTexture* getTexture(){ return texture;};
            //! Devuelve la posicion
            //! @returns position La posicion del billboard
            glm::vec3 getPosition(){ return position;};
            //! Asigna una textura
            //! @param t Objeto de la textura
            void setTexture(CLResourceTexture* t) { texture = t;};
            //! Asigna una posicion
            //! @param p Posicion del billboard
            void setPosition(glm::vec3 p) { position = p;};

        private:
            CLResourceTexture*  texture = nullptr;
            glm::vec3 position;
            GLuint vertexBuffer {0}, VAO {0};
            float width {0}, height {0};
                  
    };
}