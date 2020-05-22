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
 * @author Rubén Rubio Martínez <https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/> 
 * 
 */
 
 
 #pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>

namespace CLE {
    //! Clase para gestionar los shaders leidos
    //! Con ella se pueden leer cualquier shader y acceder de manera rapida durante la ejecición del código
    class CLResourceShader : public CLResource {
        public:
            CLResourceShader() {};
            ~CLResourceShader() {};

            void Draw(GLuint shaderID) override;
            bool LoadFile(string, bool) override;
            //! Para leer vertex y fragment
            //! @param vertex ruta del vertex shader
            //! @param fragment ruta del fragment shader
            //! @returns true si se ha leido correctamente
            bool LoadFile(string vertex,string fragment);
            //! Para leer vertex y fragment
            //! @param vertex ruta del vertex shader
            //! @param fragment ruta del fragment shader
            //! @param geometry ruta del geometry shader
            //! @returns true si se ha leido correctamente
            bool LoadFile(string vertex,string fragment,string geometry);
            //! Devuelve el ID del programa
            //! @returns programID identificador de Opengl para shaders
            int GetProgramID()                  const { return programID;   }

        private:
            bool LoadShader(string, GLenum);
            bool LinkShaders();
            bool LinkShadersGeometry();
            GLuint vertexID;
            GLuint fragmentID;
            GLuint geometryID;
            int programID;
            //GLenum shaderType;
    
    };

}