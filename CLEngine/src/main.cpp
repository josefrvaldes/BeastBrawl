#include <iostream>
#include "../include/glew/glew.h"
#include "../include/glfw/glfw3.h"

using namespace std;

void checkInput (GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {

    glfwInit();
    cout << ">>>>> GLFW ON" << endl;

    // TO-DO: Establecer version. ¿Es necesario?
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow( 1280, 720, "Beast Brawl", NULL, NULL );

    if (!window) {
        cout << "No se ha podido crear la ventana" << endl;
        glfwTerminate();
        return -1;
    }

    cout << "    > Ventana creada" << endl;

    glfwMakeContextCurrent(window);
    glViewport(0, 0, 1280, 720);
    cout << "    > Viewport establecido" << endl;

    while (!glfwWindowShouldClose(window)) {

        // Input.
        checkInput(window);

        // Render.
        glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Cambia de buffer y checkea eventos.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    cout << ">>>>> GLFW OFF" << endl;

    return 0;
    
}