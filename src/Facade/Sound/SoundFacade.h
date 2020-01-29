#pragma once

using namespace std;

class SoundFacade {
    public:
        explicit SoundFacade() = default;
        virtual ~SoundFacade() = default;

        virtual void InitSoundEngine() = 0;
        virtual void TerminateSoundEngine() = 0;

        virtual void SetState(const uint8_t) = 0;
        virtual void SetParameter(const string, const string, const float) = 0;

        virtual void PlayEvent(const string) = 0;
        virtual void StopAllEvents() = 0;
        virtual void StopEvent(const string) = 0;
        virtual void PauseAllEvent() = 0;
        virtual void ResumeAllEvent() = 0;

        virtual void Update() = 0;

    private:
        virtual void LoadSoundByState(const uint8_t) = 0;
        virtual void LoadSoundBank(const string, const bool) = 0;
        virtual void LoadSoundEvent(const string, const bool) = 0;
        
        virtual void SubscribeToGameEvents(const uint8_t) = 0;
    
};