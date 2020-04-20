#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>

extern "C" {
    #include <fmod_studio.h>
    #include <fmod.h>
    #include <fmod_errors.h>
}

#include "SoundNode.h"


using namespace std;

namespace CLSE {
    class SoundEngine {
        public:
            explicit SoundEngine();
            ~SoundEngine();

            void InitSoundEngine(bool);
            void TerminateSoundEngine();
            void UnloadAllBanks();
            bool LoadSoundBank(const string&);
            void LoadSoundEvent(const string&, const bool);
            void PlayEvent(const string&);
            void PlayEventWithVolume(const string&, float);
            void StopAllEvents();
            void StopEvent(const string&);
            void PauseAllEvents();
            void ResumeAllEvents();
            void PauseEvent(const string&);
            void ResumeEvent(const string&);

            bool IsPlaying(FMOD_STUDIO_EVENTINSTANCE*) const;
            bool IsPlaying2D(const string&) const;
            bool IsPlayingEstatic3D(const string&) const;
            bool IsPlayingDinamic3D(const string&) const;

            void UpdateEngine();

            void SetParameter(const string&, const string&, const float);
            void SetListenerPosition(const glm::vec3&, const glm::vec3&);
            void Set3DAttributes(const string&, const glm::vec3&, const float);
            void SetEventPosition3D(FMOD_STUDIO_EVENTINSTANCE*, const glm::vec3&, const float);
            void SetGlobalVolume(float gv);
            float GetGlobalVolume() { return globalVolume; };
            void SetVolume(FMOD_STUDIO_EVENTINSTANCE*, float);
            void SetVolume(const string&, float);

            void CreateSoundNode2D(const string&);
            void CreateSoundNodeEstatic3D(uint16_t, glm::vec3&, string&, bool);
            void CreateSoundNodeDinamic3D(uint16_t, glm::vec3&, string&, bool, bool);



        private:
            void LoadMasterBank();
            void UnloadMasterBank();

            FMOD_SYSTEM*           coreSystem  { nullptr };
            FMOD_STUDIO_SYSTEM*   system      { nullptr };
            FMOD_STUDIO_BANK*     masterBank  { nullptr };
            FMOD_STUDIO_BANK*     stringsBank { nullptr };

            float globalVolume { 0.33 };

            unordered_map<string, FMOD_STUDIO_BANK*>              banks;
            unordered_map<string, FMOD_STUDIO_EVENTDESCRIPTION *>  soundDescriptions;

            unordered_map<string, unique_ptr<SoundNode>>     eventInstances2D;
            unordered_map<string, unique_ptr<SoundNode>>     eventInstancesEstatic3D;
            unordered_map<string, unique_ptr<SoundNode>>     eventInstancesDinamic3D;
    };
}