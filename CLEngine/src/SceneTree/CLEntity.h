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

#include <glm/glm.hpp>
#include "../../include/glew/glew.h"

namespace CLE {

    //! Clase virtual para que hereden el resto de entidades
    class CLEntity {
        public:
            CLEntity(){};
            CLEntity(unsigned int idEntity) : id(idEntity) {}
            ~CLEntity(){};

            unsigned int GetID() { return id; }

            virtual void Draw (GLuint) = 0;
            virtual void DrawDepthMap (GLuint) = 0;

        protected:

            unsigned int id;

    };
}
