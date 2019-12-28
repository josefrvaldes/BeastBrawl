#pragma once

#include <iostream>
#include <unordered_map>

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "../../EventManager/EventManager.h"
#include "../../EventManager/Event.h"
#include "../../Aliases.h"
#include "SoundFacade.h"

using namespace std;

class SoundFacadeFMOD : public SoundFacade {
    public:
        explicit SoundFacadeFMOD() : SoundFacade() {};
        ~SoundFacadeFMOD();

        // Funciones basicas del motor de audio.
        void InitSoundEngine() override;
        void TerminateSoundEngine() override;
        void UnloadAllBanks();

        // Cambio de banco de audio y subscripcion a eventos.
        void SetState(const uint8_t) override;
        
        bool IsPlaying(FMOD::Studio::EventInstance*);
        void Update() override;

        unordered_map<string, FMOD::Studio::EventDescription*> GetDescriptions() { return soundDescriptions;};
        unordered_map<string, FMOD::Studio::EventInstance*> GetInstances() { return eventInstances; }

    private:
        // eventos del juego
        void SoundClaxon(DataMap);


        void LoadMasterBank();
        void UnloadMasterBank();
        void LoadSoundByState(const uint8_t) override;
        void LoadSounds(const string, const bool) override;
        void LoadSoundEvent(const string, const bool) override;
        void PlayEvent3D(const string);
        void PlayEvent2D(const string);
        
        void SubscribeToGameEvents(const uint8_t) override;



        FMOD::System* coreSystem = NULL;
        FMOD::Studio::System* system = NULL;
        FMOD::Studio::Bank* masterBank = NULL;
        FMOD::Studio::Bank* stringsBank = NULL;

        unordered_map<string, FMOD::Studio::Bank*> banks;
        unordered_map<string, FMOD::Studio::EventDescription*> soundDescriptions;
        unordered_map<string, FMOD::Studio::EventInstance*> eventInstances;
        unordered_map<string, vector<string>> events = {
            {"InGame2D", {
                            "Coche/claxon",
                            "Personajes/choque_enemigo", 
                            "Personajes/choque_powerup",
                            "Personajes/derrape",
                            "Personajes/derrota",
                            "Personajes/powerup",
                            "Personajes/nitro",
                            "Personajes/random",
                            "PowerUp/robojorobo",
                            "Partida/cuenta_atras"
                         } 
            }
        };

        shared_ptr<EventManager> eventManager;
};