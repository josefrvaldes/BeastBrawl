#pragma once

#include <iostream>
#include <memory>
#include <map>

#include <glew/glew.h>
#include <glfw/glfw3.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"

#include <stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H


//Motor grafico
#include "SceneTree/CLNode.h"
#include "SceneTree/CLEntity.h"
#include "SceneTree/CLMesh.h"
#include "SceneTree/CLPointLight.h"
#include "SceneTree/CLCamera.h"
#include "SceneTree/CLShadowMapping.h"
#include "ResourceManager/CLResourceManager.h"

using namespace std;

namespace CLE {

struct Character {
    GLuint      TextureID;   // ID handle of the glyph texture
    glm::ivec2  Size;    // Size of glyph
    glm::ivec2  Bearing;  // Offset from baseline to left/top of glyph
    GLuint      Advance;    // Horizontal offset to advance to next glyph
};

class CLEngine {
    public:
        CLEngine() = default;;
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
        void DrawDepthMap();
        void RenderDepthMap(CLShadowMapping& shadowMap, CLResourceShader* depthShader, glm::vec3 posLight);
        void Clear();

        void LoadFont(const std::string&);
        void DrawImage2D(float _x, float _y, float _width, float _height, float _depth, string& file, bool);
        void RenderText2D(std::string& text, GLfloat x, GLfloat y, GLfloat depth, GLfloat scale, glm::vec3& color);


        CLNode* GetSceneManager();
        CLResourceManager* GetResourceManager();

        int GetScreenWidth() { return width; };
        int GetScreenHeight() { return height; };

        
    private:

        void CreateGlfwWindow(const unsigned int, const unsigned int, const string&);

        void ImGuiInit();
        void TerminateImGui();

        int width{};
        int height{};
        GLFWwindow *window { nullptr };
        unsigned int hudShader { 0 };
        unsigned int textShader { 0 };

        
        std::unique_ptr<CLNode> smgr {nullptr};
        std::unique_ptr<CLResourceManager> resourceManager {nullptr};

        GLuint VAOText, VBOText;
        std::map<GLchar, Character> characters;
};
}
