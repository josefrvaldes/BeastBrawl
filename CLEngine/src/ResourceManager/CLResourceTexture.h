#pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>
#include <string>
#include <stb_image.h>

namespace CLE {
    
    //! Clase para gestionar la lectura de texturas
    class CLResourceTexture : public CLResource {
        public:
            CLResourceTexture() {};
            ~CLResourceTexture();

            void Draw(GLuint shaderID) override;
            bool LoadFile(string, bool) override;
            
            //! Devuelve el ID de la textura
            //! @returns textureID ID de OpenGL para identificar texturas
            unsigned int GetTextureID() { return textureID; };
            int GetWidth(){ return widthText; };
            int GetHeight(){ return heightText; };

         private:
            unsigned int textureID;
            string type;
            int widthText = 0;
            int heightText = 0;
    };

}