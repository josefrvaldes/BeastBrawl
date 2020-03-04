#pragma once

#include <cstddef>

//#define SERVER_HOST "54.38.189.168"
//#define SERVER_PORT_UDP 80
//#define SERVER_PORT_TCP 80

#define SERVER_HOST "localhost"
// #define SERVER_HOST "54.38.189.168"
#define SERVER_PORT_UDP 1234
#define SERVER_PORT_TCP 1235

#define SERVER_HOST_OVH "54.38.189.168"

#define MIN_NUM_PLAYERS 2
#define FRAME_RATE 60

class Constants {
   public:
      inline static const bool DEBUG_SHOW_CHASSIS{true};
      inline static const bool DEBUG_SHOW_SPHERES{true};
      inline static const size_t ONLINE_BUFFER_SIZE{512};
      inline static const float DELTA_TIME{1.0 / FRAME_RATE};

      enum InputTypes { FORWARD,
                        BACK,
                        LEFT,
                        RIGHT,
                        LAUNCH_PU,
                        CLAXON,
                        DRIFT };

    enum PetitionTypes { CONNECTION_REQUEST,
                         SEND_INPUTS,
                         SEND_SYNC,
                         CATCH_PU,
                         SEND_DISCONNECTION,
                         CATCH_TOTEM };
      enum RenderEngine { IRRLICHT,
                          CLOVER};
      inline static const RenderEngine RENDER_ENGINE = RenderEngine::IRRLICHT;
};