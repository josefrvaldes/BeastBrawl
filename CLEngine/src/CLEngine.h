#pragma once

#include <iostream>
#include <memory>

#include "../include/glew/glew.h"
#include "../include/glfw/glfw3.h"

using namespace std;

namespace CLE {
    
    class CLEngine {
        public:
            CLEngine() {};
            CLEngine(const unsigned int, const unsigned int, const string&);
            ~CLEngine();

            GLFWwindow* GetWindow() const { return window; }
            bool Loop();
            bool Run();
            
        private:

            void CreateGlfwWindow(const unsigned int, const unsigned int, const string&);

            int width;
            int height;
            GLFWwindow *window = nullptr;
    };
}
