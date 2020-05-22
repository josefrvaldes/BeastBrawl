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
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "../../include/glew/glew.h"

using namespace std;

namespace CLE {
    //! Clase virtual para los recursos
    //! Utilizada por el polimorfismo en todas las clases de recursos
    class CLResource {
        public:
            CLResource() {};
            ~CLResource() {};

            //! Metodo virtual para algunos recursos que necesiten dibujado
            virtual void Draw(GLuint shaderID) = 0;
            //! Metodo virtual para leer ficheros
            virtual bool LoadFile(string, bool) = 0;

            void SetName(string n)  { name = n; }
            //! @returns El nombre del fichero leido
            const string GetName()       const   { return name; }

        private:

            string name;
    };

}