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

#include "SoundFacade.h"
#include <SharpEngine/SharpEngine.h>

#include <unordered_map>
#include <iostream>

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
        void PlayEventWithSpecificVolume (const string&, float) override;
        void StopAllEvents()            override;
        void StopEvent(const string&)   override;
        void PauseAllEvent()            override;
        void ResumeAllEvent()           override;
        void PauseEvent(const string&);
        void ResumeEvent(const string&) override;

        void UpdateCars(const vector<shared_ptr<Entity>>&)              override;
        void UpdatePowerUps(const vector<shared_ptr<Entity>>&)          override;
        void UpdateTotem(const shared_ptr<CarHuman>&, const vector<shared_ptr<Entity>>&)             override;
        void UpdateListener(const shared_ptr<CarHuman>&)                override;
        void Update()                                                   override;

        void SetEventPositionEstatic3D(const string&, const glm::vec3&);
        void SetEventPositionDinamic3D(const string&, const glm::vec3&, const float);
        void SetParameter(const string&, const string&, const float) override;

        void CreateSound2D(const string&) override;
        void CreateSoundEstatic3D(uint16_t, glm::vec3&, string&, bool) override;
        void CreateSoundDinamic3D(uint16_t, glm::vec3&, string&, bool, bool) override;

    private:

        void LoadSoundByState(const uint8_t) override;
        void LoadSoundBank(const string&, const bool) override;
        void LoadSoundEvent(const string&, const bool, const bool) override;

        void StartGame();
        void StopMusicInGame();

        void SubscribeToGameEvents(const uint8_t) override;

        // -------------------------------- Eventos: Ajustes
        void SetCharacter(DataMap*);
        void SetGlobalVolume(DataMap*);
        void SetMusicVolume(DataMap*);

        // -------------------------------- Eventos del juego: IN GAME
        void StartMusicInGame(DataMap*);
        void SoundClaxon(DataMap*);
        void SoundThrowPowerup(DataMap*);
        void SoundHurt(DataMap*);
        void SoundCatchTotem(DataMap*);
        void SoundCrash(DataMap*);
        void SoundCrashWall(DataMap*);
        void SoundBreakBox(DataMap*);
        void SoundDrift(DataMap*);
        void SoundRandomSentence(DataMap*);
        void SoundClock(DataMap*);

        void StopShield(DataMap*);
        void StopDrift(DataMap*);
        void StopClock(DataMap*);
        void StopSoundMM(DataMap*);
        void StopSoundTB(DataMap*);

        // -------------------------------- Eventos del juego: MENU
        void SoundMenuOption(DataMap*);
        void SoundMenuOk(DataMap*);
        void SoundMenuBack(DataMap*);

        void SoundVictoryVoice();
        //void SoundVictory(DataMap*);
        //void SoundDefeat(DataMap*);

        float character { 0 } ;
        enum TipoVoz { ChoqueEnemigo, ChoquePowerup, Derrape, Powerup, Random, Nitro, Seleccion };
        CLSE::SharpEngine* soundEngine { nullptr };

        shared_ptr<EventManager> eventManager;

        unordered_map<string, vector<string>> events = {
            { "InGame2D",
                                {
                                "Ambiente/ambiente",
                                "Coche/claxon",
                                "Personajes/voces",
                                "Partida/cuenta_atras",
                                "PowerUp/robojorobo",
                                "Partida/reloj",
                                "Partida/pitido_final",
                                "Partida/cinematica"
                                } 
            },
            { "InGame3DD",
                            {
                                "Coche/motor",
                                "Partida/totem",
                                "PowerUp/escudo",
                                "PowerUp/melonmolon",
                                "PowerUp/telebanana",
                                "PowerUp/pudin",
                                "PowerUp/telebanana_prov"
                                }
            },
            { "InGame3DE",
                            {
                                "PowerUp/escudo_roto",
                                "Partida/coger_totem",
                                "PowerUp/choque_powerup",
                                "Coche/choque",
                                "Partida/coger_caja"
                                }

            },
            { "EndRace",
                                {
                                "Personajes/derrota",
                                "Personajes/victoria"
                                }
            },
            { "Menu",
                                {
                                "Menu/atras",
                                "Menu/aceptar",
                                "Menu/cambio_opcion"
                                }
            },
            { "Musica",         {
                                "Musica/fin_partida",
                                "Musica/in_game_1",
                                "Musica/intro",
                                "Musica/menu",
                                "Musica/credits"
                                }
            }
        };
};