#pragma once

#define SERVER_HOST "54.38.189.168"
#define SERVER_PORT_UDP 80
#define SERVER_PORT_TCP 80

#define SERVER_HOST_OVH "54.38.189.168"

class Constants {
   public:
    inline static const bool DEBUG_SHOW_SPHERES{true};
    inline static const size_t ONLINE_BUFFER_SIZE{512};

    enum InputTypes { FORWARD,
                      BACK,
                      LEFT,
                      RIGHT,
                      LAUNCH_PU,
                      CLAXON,
                      DRIFT };

    enum PetitionTypes { CONNECTION_REQUEST,
                         SEND_INPUT,
                         SEND_INPUTS,
                         SEND_SYNC };
};