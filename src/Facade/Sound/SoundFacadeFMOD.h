#pragma once

#include "SoundFacade.h"
#include <SoundEngine/SoundEngine.h>

#include <iostream>
#include <unordered_map>

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>

#include <EventManager/EventManager.h>
#include <EventManager/Event.h>
#include <Aliases.h>

using namespace std;

class SoundFacadeFMOD : public SoundFacade {
    public:
        explicit SoundFacadeFMOD() : SoundFacade() {};
        ~SoundFacadeFMOD() override;

        void Initialize()               override;
        void Terminate()                override;
        void SetState(const uint8_t)    override;

        void PlayEvent(const string&)   override;
        void StopAllEvents()            override;
        void StopEvent(const string&)   override;
        void PauseAllEvent()            override;
        void ResumeAllEvent()           override;
        void PauseEvent(const string&);
        void ResumeEvent(const string&);

        void Update()                   override;


        void SetEventPositionEstatic3D(const string&, const glm::vec3&);
        void SetEventPositionDinamic3D(const string&, const glm::vec3&);
        void SetParameter(const string&, const string&, const float) override;

        void CreateSound2D(const string&);
        void CreateSoundEstatic3D(uint16_t, glm::vec3&, string&);
        void CreateSoundDinamic3D(uint16_t, glm::vec3&, string&);

    private:

        void LoadSoundByState(const uint8_t) override;
        void LoadSoundBank(const string&, const bool) override;
        void LoadSoundEvent(const string&, const bool) override;

        void SubscribeToGameEvents(const uint8_t) override;

        // -------------------------------- Eventos del juego: IN GAME
        void StartGame(DataMap*);
        void SoundClaxon(DataMap*);
        void SoundThrowPowerup(DataMap*);
        void SoundHurt(DataMap*);
        void SoundCatchTotem(DataMap*);
        void SoundCrash(DataMap*);
        void SoundBreakBox(DataMap*);
        void SoundDrift(DataMap*);
        void SoundRandomSentence(DataMap*);

        void StopShield(DataMap*);
        void StopDrift(DataMap*);

        // -------------------------------- Eventos del juego: MENU
        void SoundMenuOption(DataMap*);



        float character { 0 } ;
        enum TipoVoz { ChoqueEnemigo, ChoquePowerup, Derrape, Powerup, Random };
        CLSE::SoundEngine* soundEngine { nullptr };

        shared_ptr<EventManager> eventManager;

        unordered_map<string, vector<string>> events = {
            { "InGame2D",
                                {
                                "Ambiente/ambiente",
                                "Coche/claxon",
                                "Personajes/voces",
                                "Partida/cuenta_atras",
                                "Musica/in_game_1"
                                } 
            },
            { "InGame3DD",
                            {
                                "Coche/motor",
                                "Partida/totem",
                                "PowerUp/escudo",
                                "PowerUp/melonmolon",
                                "PowerUp/robojorobo",
                                "PowerUp/telebanana",
                                "PowerUp/telebanana_prov"
                                }
            },
            { "InGame3DE",
                            {
                                //"PowerUp/escudo_roto",
                                //"Partida/coger_totem",
                                //"Coche/choque_powerup",
                                //"Coche/choque",
                                //"PowerUp/pudin",
                                "Partida/coger_caja"
                                }

            },
            { "EndRace",
                                {
                                "Musica/fin_partida",
                                "Personajes/derrota",
                                "Personajes/victoria"
                                }
            },
            { "Menu",
                            {
                                "Menu/atras",
                                "Menu/aceptar",
                                "Menu/cambio_opcion",
                                "Musica/menu"
                                }
            }
        };
};