#pragma once

#include <cstddef>
#include <string>

//#define SERVER_HOST "54.38.189.168"
//#define SERVER_PORT_UDP 80
//#define SERVER_PORT_TCP 80

//#define SERVER_HOST "localhost"
// #define SERVER_HOST "54.38.189.168"
#define SERVER_PORT_UDP 1234
#define SERVER_PORT_TCP 1235

#define SERVER_HOST_OVH "54.38.189.168"

// #define MIN_NUM_PLAYERS 3
#define FRAME_RATE 60

class Constants {
  public:
    inline static std::string SERVER_HOST = "localhost";
    inline static uint8_t MIN_NUM_PLAYERS = 4;
    inline static const bool DEBUG_SHOW_CHASSIS{true};
    inline static const bool DEBUG_SHOW_SPHERES{true};
    inline static const size_t ONLINE_BUFFER_SIZE{512};
    inline static const float DELTA_TIME{1.0 / FRAME_RATE};

    inline static const uint16_t ANY_PLAYER = 255;

    inline static const float MELON_MOLON_SPEED = 510.0;

    inline static const float FAR_PLANE = 1500.0;

    inline static const bool CLIPPING_OCTREE = false;

    enum InputTypes { FORWARD,
                    BACK,
                    LEFT,
                    RIGHT,
                    LAUNCH_PU,
                    CLAXON,
                    DRIFT };

    enum PetitionTypes { CONNECTION_REQUEST = 2,
                        TCP_START_GAME,
                        TCP_FULL_GAME,
                        ENDGAME,
                        LAUNCH_ANIMATION_END,
                        LAUNCH_ANIMATION_COUNTDOWN,
                        SEND_INPUTS,
                        SEND_SYNC,
                        CATCH_PU,
                        SEND_CRASH_PU_CAR,
                        SEND_CRASH_PU_WALL,
                        SEND_DISCONNECTION,
                        CATCH_TOTEM,
                        LOST_TOTEM,
                        USED_ROBOJOROBO,
                        WAITING_FOR_COUNTDOWN,
                        COLLIDE_NITRO,
                        SEND_THROW_MELON_O_PUDIN,
                        SEND_THROW_TELEBANANA };

    enum RenderEngine { IRRLICHT,
                        CLOVER};
    inline static const RenderEngine RENDER_ENGINE = RenderEngine::CLOVER;
};