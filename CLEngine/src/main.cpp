#include <iostream>
#include <glew.h>
#include <GLFW/glfw3.h>

using namespace std;

const char *vertexShaderSource = "#version 450 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

void checkInput (GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void checkVertexShaderCompilation (const unsigned int &vs) {
    int success;
    char infoLog[256];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vs, 256, NULL, infoLog);
        cout << "ERROR: vertex shader compilation has failed\n" << infoLog << endl;
    }
}

int main() {

    if (!glfwInit()) {
        cout << "No se ha podido iniciar GLFW" << endl;
        return -1;
    }
    cout << "GLFW ON" << endl;

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

    // Contexto de la ventana actual y viewport.
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 1280, 720);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Creación de un objeto búfer.
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Vinculación de un búfer para su uso.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Funcion que copia los datos definidos por el usuario al búfer vinculado.
    // 1 - Búfer actual vinculado, 2 - Tamaño en bytes de los datos a pasar, 3 - Los datos, 4 - Tipo de administracion
    // 4 -> GL_STATIC_DRAW (datos que no van a cambiar o poco), FL_DYNAMIC_DRAW (datos que cambiaran mucho), GL_STREAM_DRAW (los datos cambian cada vez que se dibujan)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Creamos el objeto shader del tipo que queremos.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Compilamos el codigo fuente del vertex shader.
    // 1 - Objeto shader, 2 - Cuantas cadenas de codigo fuente pasamos, 3 - Codigo fuente real, 4 - NULL
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Miramos si la compilacion del codigo fuente del vertex shader se ha hecho correctamente.
    checkVertexShaderCompilation(vertexShader);

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
    cout << "GLFW OFF" << endl;

    return 0;
    
}