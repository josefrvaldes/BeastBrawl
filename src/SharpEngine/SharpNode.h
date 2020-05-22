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
 * @author Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/
 * 
 */
 
 
 //Made with FMOD Studio by Firelight Technologies Pty Ltd.

#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <fmod_studio.h>
#include <fmod.h>

namespace CLSE {
    class SharpNode {
        public:
            SharpNode() {};
            SharpNode(uint16_t, glm::vec3&, bool);
            ~SharpNode() {};

            void SetInstance(FMOD_STUDIO_EVENTINSTANCE& i)              { instance = &i; };
            void SetPosition(glm::vec3 p)                               { pos = p; };
            void SetMusic(bool m)                                       { music = m; };

            uint16_t GetIdEntity() const                                { return idEntity; };
            glm::vec3 GetPosition() const                               { return pos; };
            FMOD_STUDIO_EVENTINSTANCE* GetInstance() const              { return instance; };
            bool GetClean() const                                       { return clean; };
            bool GetMusic() const                                       { return music; };

        private:
            uint16_t idEntity { 0 };
            glm::vec3 pos { glm::vec3(0.0f,0.0f,0.0f) };
            bool clean { false };
            bool music { false };
            FMOD_STUDIO_EVENTINSTANCE* instance;
    };
}