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

//! Estructura utilizada para el dibujado de caracteres
struct Character {
    GLuint      TextureID;   // ID handle of the glyph texture
    glm::ivec2  Size;    // Size of glyph
    glm::ivec2  Bearing;  // Offset from baseline to left/top of glyph
    GLuint      Advance;    // Horizontal offset to advance to next glyph
};

//! Clase para iniciar el motor
class CLEngine {
    public:
        CLEngine() = default;
        CLEngine(const unsigned int, const unsigned int, const string&);
        ~CLEngine();

        //! Devuelve la ventana 
        //! @returns window La ventana de la aplicación
        GLFWwindow* GetWindow() const { return window; }
        bool Loop();
        //! Devuelve si la aplicación sigue corriendo 
        //! @returns bool del estado de la aplicación
        bool Run();
        //! Borra la pantalla
        void BeginScene();
        //! Dibuja todos los objetos del arbol
        void DrawObjects();
        //! Intercambio de buffers
        void EndScene();
        //! Asigna el titulo de la ventana
        //! @param title Titulo de la ventana
        void SetTitle(string&);
        //! Asigna el titulo de la ventana
        //! @param title Titulo de la ventana
        void SetTitle(string&&);
        //! Renderiza ImGui
        void RenderImgui();
        //! Actualiza el viewport
        void UpdateViewport();
        //! Devuelve el tiempo
        //! @returns time Tiempo de ejecucion
        double GetTime();
        //! Desactiva el cursor
        void DisableCursor();
        void CloseWindow();
        void PollEvents();
        void DrawDepthMap();
        void RenderDepthMap(CLShadowMapping& shadowMap, CLResourceShader* depthShader, glm::vec3 posLight);
        void Clear();

        //! Carga una fuente
        //! @param file Ruta a la fuente
        void LoadFont(const std::string&);
        //! Dibuja imagen 2D
        //! @param _x Valor de X
        //! @param _y Valor de Y
        //! @param _width Valor de la anchura
        //! @param _height Valor de la altura
        //! @param _depth Valor de la profundidad
        //! @param file Ruta a la imagen
        //! @param bool Flip vertical
        void DrawImage2D(float _x, float _y, float _width, float _height, float _depth, string file, bool);
        //! Dibuja texto 2D
        //! @param text Texto para dibujar
        //! @param x Posicion en X
        //! @param y Posicion en Y
        //! @param depth Profundidad
        //! @param scale Escalado
        //! @param color Color 
        void RenderText2D(std::string& text, GLfloat x, GLfloat y, GLfloat depth, GLfloat scale, glm::vec3& color);

        //void renderBillboard();

        //! Devuelve el SceneManager
        //! @returns CLNode*
        //! @see CLE::CLNode()
        CLNode* GetSceneManager();
        //! Devuelve el ResourceManager
        //! @returns CLResourceManager*
        //! @see CLE::CLResourceManager()
        CLResourceManager* GetResourceManager();

        //! Devuelve la anchura de la pantalla
        //! @returns widht Anchura de la pantalla
        int GetScreenWidth() { return width; };
        //! Devuelve la altura de la pantalla
        //! @returns height Altura de la pantalla
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
