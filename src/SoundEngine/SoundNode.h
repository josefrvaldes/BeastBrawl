#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <fmod_studio.hpp>
#include <fmod.hpp>

namespace CLSE {
    class SoundNode {
        public:
            SoundNode() {};
            SoundNode(uint16_t, glm::vec3&);
            ~SoundNode() {};

            void SetInstance(FMOD::Studio::EventInstance& i)            { instance = &i; };

            uint16_t GetIdEntity() const                                { return idEntity; };
            glm::vec3 GetPosition() const                               { return pos; };
            FMOD::Studio::EventInstance* GetInstance() const            { return instance; };

        private:
            uint16_t idEntity { 0 };
            glm::vec3 pos { glm::vec3(0.0f,0.0f,0.0f) };
            FMOD::Studio::EventInstance* instance;
    };
}