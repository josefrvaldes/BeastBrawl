#pragma once

using namespace std;

class SoundFacade {
    public:
        explicit SoundFacade() = default;
        virtual ~SoundFacade() = default;

        virtual void InitSoundEngine() = 0;
        virtual void TerminateSoundEngine() = 0;

        virtual void SetState(const uint8_t) = 0;

        virtual void Update() = 0;

    private:
        virtual void LoadSoundByState(const uint8_t) = 0;
        virtual void LoadSounds(const string, const bool) = 0;
        virtual void LoadSoundEvent(const string, const bool) = 0;
        
        virtual void SubscribeToGameEvents(const uint8_t) = 0;
    
};