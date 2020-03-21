#pragma once

#include <iostream>
#include <memory>

// #include <glew/glew.h>
// #include <glfw/glfw3.h>

#include <glew/glew.h>
#include <glfw/glfw3.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"

#include <stb_image.h>


//Motor grafico
#include "SceneTree/CLNode.h"
#include "SceneTree/CLEntity.h"
#include "SceneTree/CLMesh.h"
#include "SceneTree/CLLight.h"
#include "SceneTree/CLCamera.h"
#include "ResourceManager/CLResourceManager.h"

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
        void DrawObjects();
        void EndScene();
        void SetTitle(string&);
        void RenderImgui();
        void UpdateViewport();
        double GetTime();
        void InputClose();
        void CloseWindow();
        void PollEvents();

        void DrawImage2D(float _x, float _y, float _width, float _height, float _depth, string& file, bool);


        CLNode* GetSceneManager();
        CLResourceManager* GetResourceManager();

        
    private:

        void CreateGlfwWindow(const unsigned int, const unsigned int, const string&);

        void ImGuiInit();
        void TerminateImGui();

        int width;
        int height;
        GLFWwindow *window { nullptr };
        unsigned int hudShader { 0 };

        
        std::unique_ptr<CLNode> smgr {nullptr};
        std::unique_ptr<CLResourceManager> resourceManager {nullptr};
};
}
