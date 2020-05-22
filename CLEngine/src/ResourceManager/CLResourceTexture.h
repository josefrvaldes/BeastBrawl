/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Rubén Rubio Martínez <https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/> 
 * 
 */
 
 
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