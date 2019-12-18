#pragma once

#include <iostream>
#include "SoundFacadeFMOD.h"

class SoundFacadeManager {
    public:
        ~SoundFacadeManager() {
            delete soundFacade;
        };

        void InitializeFacadeFmod() {
            soundFacade = new SoundFacadeFMOD();
        }

        static SoundFacadeManager* GetInstance();
        SoundFacade *GetSoundFacade() { return soundFacade; };

    private:
        explicit SoundFacadeManager() : soundFacade {nullptr} {}

        static SoundFacadeManager* instance;
        SoundFacade *soundFacade;
};