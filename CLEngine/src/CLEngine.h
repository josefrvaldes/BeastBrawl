#pragma once

#include <iostream>
#include <memory>

// #include <glew/glew.h>
// #include <glfw/glfw3.h>

#include "../include/glew/glew.h"
#include "../include/glfw/glfw3.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"

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
        void BeginScene();
        void EndScene();
        void UpdateViewport();
        
    private:

        void CreateGlfwWindow(const unsigned int, const unsigned int, const string&);
        void ImGuiInit();
        void TerminateImGui();
        int width;
        int height;
        GLFWwindow *window = nullptr;
};
}
