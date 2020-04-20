#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <fmod_studio.h>
#include <fmod.h>

namespace CLSE {
    class SoundNode {
        public:
            SoundNode() {};
            SoundNode(uint16_t, glm::vec3&, bool);
            ~SoundNode() {};

            void SetInstance(FMOD_STUDIO_EVENTINSTANCE& i)              { instance = &i; };
            void SetPosition(glm::vec3 p)                               { pos = p; };

            uint16_t GetIdEntity() const                                { return idEntity; };
            glm::vec3 GetPosition() const                               { return pos; };
            FMOD_STUDIO_EVENTINSTANCE* GetInstance() const              { return instance; };
            bool GetClean() const                                       { return clean; };

        private:
            uint16_t idEntity { 0 };
            glm::vec3 pos { glm::vec3(0.0f,0.0f,0.0f) };
            bool clean { false };
            FMOD_STUDIO_EVENTINSTANCE* instance;
    };
}