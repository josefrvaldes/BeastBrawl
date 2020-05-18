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