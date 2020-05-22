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
 * @author Antonio Jose Martinez Garcia <https://www.linkedin.com/in/antonio-jose-martinez-garcia/>
 * 
 */
 
 
 #pragma once

#include "CLEntity.h"


namespace CLE {
    //! Clase para añadir sombras en el motor
    class CLShadowMapping : public CLEntity {
        public:
            CLShadowMapping(GLuint _lightId);
            ~CLShadowMapping() {};

            //void PrepareToRender();
            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);

            // variables
            //GLuint lightId = 0;
            unsigned int depthMapFBO;
            static inline unsigned int depthMap;
            const unsigned int SHADOW_WIDTH = 1024;
            const unsigned int SHADOW_HEIGHT = 1024;
                  
    };
}