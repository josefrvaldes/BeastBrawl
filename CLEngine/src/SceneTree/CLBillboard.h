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
            //! @param offset_ Margen de la posicion original
            //! @param width_ Anchura del billboard
            //! @param height Altura del billboard
            CLBillboard(unsigned int id,CLResourceTexture* texture_, float width_, float height_);
            ~CLBillboard() {};

            void Draw(GLuint shaderID) override;
            void DrawDepthMap(GLuint shaderID) {};

            //! Devuelve la textura
            //! @returns texture El objeto CLResourceTexture
            CLResourceTexture* getTexture(){ return texture;};
            //! Asigna una textura
            //! @param t Objeto de la textura
            void setTexture(CLResourceTexture* t) { texture = t;};

        private:
            CLResourceTexture*  texture = nullptr;
            //Placeholder para mandar algo por el VAO si no no dibuja
            float position[6] = {
                1.0,1.0,1.0,
                1.0,1.0,1.0
            };
            GLuint vertexBuffer {0}, VAO {0};
            float width {0}, height {0};
                  
    };
}