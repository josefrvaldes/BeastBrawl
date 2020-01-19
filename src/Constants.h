#pragma once

#define SERVER_HOST "localhost"
#define SERVER_PORT "1234"

class Constants {
   public:
    static const bool DEBUG_SHOW_SPHERES{true};
    // static const string SERVER_HOST{"localhost"};
    // static const string SERVER_PORT{"1234"};

    enum InputTypes { FORWARD,
                      BACK,
                      LEFT,
                      RIGHT,
                      LAUNCH_PU,
                      CLAXON };
};