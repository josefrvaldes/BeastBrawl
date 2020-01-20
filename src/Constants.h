#pragma once

#define SERVER_HOST "localhost"
#define SERVER_PORT "1234"

class Constants {
   public:
    static const bool DEBUG_SHOW_SPHERES{true};

    enum InputTypes { FORWARD,
                      BACK,
                      LEFT,
                      RIGHT,
                      LAUNCH_PU,
                      CLAXON };

    enum PetitionTypes { SEND_INPUT,
                         SEND_INPUTS,
                         SEND_SYNC };
};