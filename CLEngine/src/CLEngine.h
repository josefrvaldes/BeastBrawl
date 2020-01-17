#pragma once

#include <iostream>
#include <memory>

#include "../include/glew/glew.h"
#include <GLFW/glfw3.h>

using namespace std;

namespace CLE {
    
    class CLEngine {
        public:
            CLEngine() {};
            CLEngine(const unsigned int, const unsigned int, const string&);
            ~CLEngine();


            
        private:

            void CreateGlfwWindow(const unsigned int, const unsigned int, const string&);

            unsigned int width;
            unsigned int height;
            GLFWwindow *window = nullptr;
    };
}
