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
        virtual void PlayEventWithSpecificVolume(const string&, float) = 0;
        virtual void StopAllEvents() = 0;
        virtual void StopEvent(const string&) = 0;
        virtual void PauseAllEvent() = 0;
        virtual void ResumeEvent(const string&) = 0;
        virtual void ResumeAllEvent() = 0;

        virtual void UpdateCars(const vector<shared_ptr<Entity>>&) = 0;
        virtual void UpdatePowerUps(const vector<shared_ptr<Entity>>&) = 0;
        virtual void UpdateTotem(const shared_ptr<CarHuman>&, const vector<shared_ptr<Entity>>&) = 0;
        virtual void UpdateListener(const shared_ptr<CarHuman>&) = 0;
        virtual void Update() = 0;


        virtual void CreateSound2D(const string&) = 0;
        virtual void CreateSoundEstatic3D(uint16_t, glm::vec3&, string&, bool) = 0;
        virtual void CreateSoundDinamic3D(uint16_t, glm::vec3&, string&, bool, bool) = 0;

    private:
        virtual void LoadSoundByState(const uint8_t) = 0;
        virtual void LoadSoundBank(const string&, const bool) = 0;
        virtual void LoadSoundEvent(const string&, const bool, const bool) = 0;
        
        virtual void SubscribeToGameEvents(const uint8_t) = 0;
    
};