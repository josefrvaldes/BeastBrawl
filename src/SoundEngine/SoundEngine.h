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
            bool LoadSoundBank(const string&);
            void LoadSoundEvent(const string&, const bool);
            void PlayEvent(const string&);
            void StopAllEvents();
            void StopEvent(const string&);
            void PauseAllEvents();
            void ResumeAllEvents();
            void PauseEvent(const string&);
            void ResumeEvent(const string&);

            bool IsPlaying(FMOD::Studio::EventInstance*) const;
            bool IsPlaying2D(const string&) const;
            bool IsPlayingEstatic3D(const string&) const;
            bool IsPlayingDinamic3D(const string&) const;

            void UpdateEngine();

            void SetParameter(const string&, const string&, const float);
            void SetListenerPosition(const glm::vec3&, const glm::vec3&);
            void Set3DAttributes(const string&, const glm::vec3&);
            void SetEventPosition3D(FMOD::Studio::EventInstance*, const glm::vec3&);

            void CreateSoundNode2D(const string&);
            void CreateSoundNodeEstatic3D(uint16_t, glm::vec3&, string&, bool);
            void CreateSoundNodeDinamic3D(uint16_t, glm::vec3&, string&, bool, bool);



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