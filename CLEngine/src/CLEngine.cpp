#include "CLEngine.h"

using namespace std;
using namespace CLE;

/**
 * Constructor que iniciara todas las configuraciones iniciales de OpenGL y la ventana.
 * @param w - Anchura en pixeles de la ventana.
 * @param h - Altura en pixeles de la ventana.
 * @param title - Titulo de la ventana.
 */
CLEngine::CLEngine (const unsigned int w, const unsigned int h, const string& title) {
    CreateGlfwWindow(w, h, title);
}

/**
 * Destruye la ventana de GLFW y libera la informacion.
 */
CLEngine::~CLEngine() {
    glfwDestroyWindow(window);
    glfwTerminate();
    cout << ">>>>> GLFW OFF" << endl;
}

/**
 * Inicia, crea y configura  la ventana OpenGL con GLFW.
 * @param w - Anchura en pixeles de la ventana.
 * @param h - Altura en pixeles de la ventana.
 * @param title - Titulo de la ventana.
 */
void CLEngine::CreateGlfwWindow (const unsigned int w, const unsigned int h, const string& title) {
    if (!glfwInit()) {
        cout << "- No se ha podido crear inicializar GLFW" << endl;
        exit(EXIT_FAILURE);
    }
    cout << ">>>>> GLFW ON" << endl;

    // Minima version de OpenGL, si la maquina no puede correr minimo a la indicada, falla la creacion de la ventana.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( w, h, title.c_str(), NULL, NULL );
    if (!window) {
        cout << "    > La ventana no se ha podido crear" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    cout << "    > Ventana creada" << endl;


    glfwMakeContextCurrent(window);
    glViewport(0, 0, w, h);
    cout << "    > Viewport establecido" << endl;

    width = w;
    height = h;
}

/**
 * Actualizacion de CLEngine.
 */
bool CLEngine::Run() {

    // Checkea eventos.
    glfwPollEvents();

    // Actualiza los valores de anchura y altura de la ventana por si se ha redimensionado y asi cambiar el viewport.
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Render
    glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Cambia de buffer.
    glfwSwapBuffers(window);

    return true;
}