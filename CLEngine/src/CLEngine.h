#pragma once

#include <iostream>
#include <memory>

#include <glew.h>
#include <SDL.h>

using namespace std;

namespace CLE {
    
    class CLEngine {
        public:
            explicit CLEngine() = default;
            ~CLEngine();

            
        private:

            unique_ptr<SDL_Window> window;
    }
}
