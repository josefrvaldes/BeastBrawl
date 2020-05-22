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
#include <unordered_map>
#include <memory>

extern "C" {
    #include <fmod_studio.h>
    #include <fmod.h>
    #include <fmod_errors.h>
}

#include "SharpNode.h"


using namespace std;

namespace CLSE {
    class SharpEngine {
        public:
            explicit SharpEngine();
            ~SharpEngine();

            void InitSharpEngine(bool);
            void TerminateSharpEngine();
            void UnloadAllBanks();
            bool LoadSoundBank(const string&);
            void LoadSoundEvent(const string&, const bool, const bool);
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
            void SetEventPosition3D( const string&, FMOD_STUDIO_EVENTINSTANCE*, const glm::vec3&, const float);
            void SetMusicVolume(float mv);
            void SetGlobalVolume(float gv);
            void SetVolume(FMOD_STUDIO_EVENTINSTANCE*, float);
            void SetVolume(const string&, float);
            float GetGlobalVolume() { return globalVolume; };

            void CreateSoundNode2D(const string&, bool);
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
            float musicVolume { 0.99 };

            unordered_map<string, FMOD_STUDIO_BANK*>              banks;
            unordered_map<string, FMOD_STUDIO_EVENTDESCRIPTION *>  soundDescriptions;

            unordered_map<string, unique_ptr<SharpNode>>     eventInstances2D;
            unordered_map<string, unique_ptr<SharpNode>>     eventInstancesEstatic3D;
            unordered_map<string, unique_ptr<SharpNode>>     eventInstancesDinamic3D;
    };
}