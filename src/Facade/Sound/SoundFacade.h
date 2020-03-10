#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <Entities/CarHuman.h>

using namespace std;

class SoundFacade {
    public:
        explicit SoundFacade() = default;
        virtual ~SoundFacade() = default;

        virtual void Initialize() = 0;
        virtual void Terminate() = 0;

        virtual void SetState(const uint8_t) = 0;
        virtual void SetParameter(const string&, const string&, const float) = 0;

        virtual void PlayEvent(const string&) = 0;
        virtual void StopAllEvents() = 0;
        virtual void StopEvent(const string&) = 0;
        virtual void PauseAllEvent() = 0;
        virtual void ResumeAllEvent() = 0;

        virtual void UpdateCars(const vector<shared_ptr<Entity>>&) = 0;
        virtual void UpdatePowerUps(const vector<shared_ptr<Entity>>&) = 0;
        virtual void UpdateTotem(const vector<shared_ptr<Entity>>&) = 0;
        virtual void UpdateListener(const shared_ptr<CarHuman>&) = 0;
        virtual void Update() = 0;


        virtual void CreateSound2D(const string&) = 0;
        virtual void CreateSoundEstatic3D(uint16_t, glm::vec3&, string&, bool) = 0;
        virtual void CreateSoundDinamic3D(uint16_t, glm::vec3&, string&, bool, bool) = 0;

    private:
        virtual void LoadSoundByState(const uint8_t) = 0;
        virtual void LoadSoundBank(const string&, const bool) = 0;
        virtual void LoadSoundEvent(const string&, const bool) = 0;
        
        virtual void SubscribeToGameEvents(const uint8_t) = 0;
    
};