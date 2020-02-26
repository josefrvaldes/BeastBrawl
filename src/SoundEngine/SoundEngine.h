#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "SoundNode.h"


using namespace std;

namespace CLSE {
    class SoundEngine {
        public:
            explicit SoundEngine();
            ~SoundEngine();

            void InitSoundEngine();
            void TerminateSoundEngine();
            void UnloadAllBanks();

            void CreateSoundNode2D(const string&);
            void CreateSoundNodeEstatic3D(uint16_t, glm::vec3&, string&);
            void CreateSoundNodeDinamic3D(uint16_t, glm::vec3&, string&);

            void UpdateDevice();

        private:
            void LoadMasterBank();
            void UnloadMasterBank();

            FMOD::System*           coreSystem  { nullptr };
            FMOD::Studio::System*   system      { nullptr };
            FMOD::Studio::Bank*     masterBank  { nullptr };
            FMOD::Studio::Bank*     stringsBank { nullptr };

            unordered_map<string, FMOD::Studio::Bank*>              banks;
            unordered_map<string, FMOD::Studio::EventDescription*>  soundDescriptions;

            unordered_map<string, unique_ptr<SoundNode>>     eventInstances2D;
            unordered_map<string, unique_ptr<SoundNode>>     eventInstancesEstatic3D;
            unordered_map<string, unique_ptr<SoundNode>>     eventInstancesDinamic3D;
    };
}