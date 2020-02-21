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
        void DrawTree();
        void EndScene();
        void RenderImgui();
        void UpdateViewport();
        double GetTime();
        bool InputClose();


        CLNode* GetSceneManager();
        CLResourceManager* GetResourceManager();

        
    private:

        void CreateGlfwWindow(const unsigned int, const unsigned int, const string&);

        void ImGuiInit();
        void TerminateImGui();

        int width;
        int height;
        GLFWwindow *window { nullptr };


        std::unique_ptr<CLNode> smgr {nullptr};
        std::unique_ptr<CLResourceManager> resourceManager {nullptr};
};
}
