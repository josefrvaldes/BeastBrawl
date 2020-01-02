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

        void PlayEvent3D(const string) override;
        void PlayEvent2D(const string) override;
        void StopEvent(const string) override;
        void PauseAllEvent() override;
        void ResumeAllEvent() override;
        void PauseEvent(const string);
        void ResumeEvent(const string);
        
        bool IsPlaying(FMOD::Studio::EventInstance*);
        void Update() override;

        unordered_map<string, FMOD::Studio::EventDescription*> GetDescriptions() { return soundDescriptions;};
        unordered_map<string, FMOD::Studio::EventInstance*> GetInstances() { return eventInstances; }

    private:
        // eventos del juego
        void SoundClaxon(DataMap);
        void SoundThrowPowerup(DataMap);
        void SoundHurtVoice(DataMap);       // TO-DO: ¿Juntar con el de abajo?
        void SoundHurt(DataMap);            // TO-DO: ¿Juntar con el de arriba?
        void SoundCatchTotem(DataMap);
        void SoundCrash(DataMap);           // TO-DO: ¿Juntar con el de abajo?
        //void SoundCrashVoice(DataMap);      // TO-DO: ¿Juntar con el de arriba?
        void SoundBreakBox(DataMap);
        void SoundDrift(DataMap);
        void SoundRandomSentence(DataMap);
        
        void SoundMenuOption(DataMap);

        void StopPrueba(DataMap);           // TO-DO: Quitar
        void StopShield(DataMap);           // TO-DO: Crear uno para todos los power-up
        void StopDrift(DataMap);



        void LoadMasterBank();
        void UnloadMasterBank();
        void LoadSoundByState(const uint8_t) override;
        void LoadSoundBank(const string, const bool) override;
        void LoadSoundEvent(const string, const bool) override;
        
        void SubscribeToGameEvents(const uint8_t) override;



        FMOD::System* coreSystem = NULL;
        FMOD::Studio::System* system = NULL;
        FMOD::Studio::Bank* masterBank = NULL;
        FMOD::Studio::Bank* stringsBank = NULL;

        unordered_map<string, FMOD::Studio::Bank*> banks;
        unordered_map<string, FMOD::Studio::EventDescription*> soundDescriptions;
        unordered_map<string, FMOD::Studio::EventInstance*> eventInstances;
        unordered_map<string, vector<string>> events = {
            { "InGame2D",       {
                                "Ambiente/ambiente",            // Metido
                                "Coche/claxon",                 // Metido
                                "Personajes/choque_enemigo",    
                                "Personajes/choque_powerup",    // Metido
                                "Personajes/derrape", 
                                "Personajes/powerup",           // Metido
                                "Personajes/random",
                                "Partida/cuenta_atras",
                                "Menu/cambio_opcion"        
                                } 
            },
            { "InGame3D",       {
                                "Coche/motor",                  // "Metido" en 2D
                                "Partida/coger_totem",          // "Metido" en 2D
                                "Partida/coger_caja",
                                "Partida/totem",
                                "PowerUp/escudo",               // "Metido" en 2D
                                "Coche/choque_enemigo",          
                                "PowerUp/pudin",         
                                "PowerUp/robojorobo",           // "Metido" en 2D
                                "PowerUp/telebanana"
                                }
            },
            { "EndRace",        {
                                "Musica/fin_partida",           // Metido
                                "Personajes/derrota",
                                "Personajes/victoria",
                                "Menu/aceptar"
                                }

            },
            { "Menu",           {
                                "Menu/atras",
                                "Menu/aceptar",
                                "Menu/cambio_opcion"
                                }

            }
        };

        shared_ptr<EventManager> eventManager;
};