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
        void SetEventPosition(const string, const glm::vec3&);
        void SetParameter(const string, const string, const float) override;

        void PlayEvent(const string) override;
        void StopEvent(const string) override;
        void PauseAllEvent() override;
        void ResumeAllEvent() override;
        void PauseEvent(const string);
        void ResumeEvent(const string);
        
        bool IsPlaying(FMOD::Studio::EventInstance*);
        void Update() override;

        //unordered_map<string, FMOD::Studio::EventDescription*> GetDescriptions() { return soundDescriptions;};
        //unordered_map<string, FMOD::Studio::EventInstance*> GetInstances() { return eventInstances; }

    private:
        // eventos del juego
        void StartGame(DataMap);
        void SoundClaxon(DataMap);
        void SoundThrowPowerup(DataMap);
        void SoundHurt(DataMap);
        void SoundCatchTotem(DataMap);
        void SoundCrash(DataMap);
        void SoundBreakBox(DataMap);
        void SoundDrift(DataMap);
        void SoundRandomSentence(DataMap);
        
        void SoundMenuOption(DataMap);

        void StopPrueba(DataMap);           // TO-DO: Quitar
        void StopShield(DataMap);           // TO-DO: Crear uno para todos los power-up
        void StopDrift(DataMap);



        void LoadMasterBank();
        void UnloadMasterBank();
        FMOD::Studio::EventInstance* CreateInstance(const string);
        void LoadSoundByState(const uint8_t) override;
        void LoadSoundBank(const string, const bool) override;
        void LoadSoundEvent(const string, const bool) override;
        
        void SubscribeToGameEvents(const uint8_t) override;



        FMOD::System* coreSystem = NULL;
        FMOD::Studio::System* system = NULL;
        FMOD::Studio::Bank* masterBank = NULL;
        FMOD::Studio::Bank* stringsBank = NULL;

        float character { 0 } ;

        unordered_map<string, FMOD::Studio::Bank*> banks;
        unordered_map<string, FMOD::Studio::EventDescription*> soundDescriptions;
        // TO-DO: Separar instancias
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
                                "Menu/cambio_opcion",
                                "Musica/in_game_1"              //Metido
                                } 
            },
            { "InGame3D",       {
                                "Coche/motor",                  // "Metido" en 2D
                                "Partida/coger_totem",          // "Metido" en 2D
                                "Partida/coger_caja",
                                "Partida/totem",
                                "PowerUp/escudo",               // "Metido" en 2D
                                "PowerUp/escudo_roto",
                                "Coche/choque",          
                                "PowerUp/pudin",                // "Metido" en 2D
                                "PowerUp/robojorobo",           // "Metido" en 2D
                                "PowerUp/telebanana",
                                "PowerUp/telebanana_prov",      // "Metido" como provisional
                                "PowerUp/melonmolon",           // "Metido" como provisional
                                "Coche/choque_powerup"          // "Metido" en 2D
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
            },
            { "InGameMusic",    {
                                "Musica/menu"
                                }
            }
        };

        shared_ptr<EventManager> eventManager;
};