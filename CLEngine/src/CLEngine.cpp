#include "CLEngine.h"

using namespace std;
using namespace CLE;

CLEngine::CLEngine (const unsigned int w, const unsigned int h, const string& title) {
    CreateGlfwWindow(w, h, title);
}

CLEngine::~CLEngine() {
    glfwDestroyWindow(window);
    glfwTerminate();
    cout << ">>>>> GLFW OFF" << endl;
}

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
        exit(EXIT_FAILURE);
    }
    cout << "    > Ventana creada" << endl;


    glfwMakeContextCurrent(window);
    glViewport(0, 0, w, h);
    cout << "    > Viewport establecido" << endl;

    width = w;
    height = h;
}