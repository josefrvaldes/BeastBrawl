#include <iostream>
#include <memory>

#include "CLEngine.h"
#include "SceneTree/CLLight.h"
#include "SceneTree/CLNode.h"
#include "ResourceManager/CLResourceManager.h"
#include "ResourceManager/CLResourceShader.h"
#include "ResourceManager/CLResourceMesh.h"
#include "ResourceManager/CLResource.h"

#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <math.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"

using namespace std;
using namespace CLE;


/**
 * Mira si se ha pulsado ESC para cerrar la ventana.
 * @param window - Ventana sobre la que mira los eventos. 
 */
void checkInput (GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void initialiceImGUI(CLEngine *device) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(device->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void terminateInGUI() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    CLEngine *device = new CLEngine(1280, 720, "Beast Brawl");

    //initialiceImGUI(device);

    float vertices[] = {
        // positions            // colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f
    };      


    //--------------------Scene Tree-----------------------
        unique_ptr<CLEntity> entity1 = make_unique<CLLight>(1);
        unique_ptr<CLEntity> entity2 = make_unique<CLLight>(2);
        unique_ptr<CLEntity> entity3 = make_unique<CLLight>(3);
        unique_ptr<CLEntity> entity4 = make_unique<CLLight>(4);
        unique_ptr<CLEntity> entity5 = make_unique<CLLight>(5);
        unique_ptr<CLNode> smgr = make_unique<CLNode>(entity1.get());
        unique_ptr<CLNode> node2 = make_unique<CLNode>(entity2.get());
        unique_ptr<CLNode> node3 = make_unique<CLNode>(entity3.get());
        unique_ptr<CLNode> node4 = make_unique<CLNode>(entity4.get());
        unique_ptr<CLNode> node5 = make_unique<CLNode>(entity5.get());

        smgr->AddChild(node2.get());
        smgr->AddChild(node3.get());
        node2->AddChild(node4.get());
        node4->AddChild(node5.get());

        //smgr->DrawTree(smgr.get());

        node5->SetScalation(glm::vec3(1.5f,1.0f,1.0f));
        node5->SetRotation(glm::vec3(20.0f,30.0f,0.0f));

        smgr->DFSTree(glm::mat4(1.0));


    //-------------------Resource manager-------------------
        unique_ptr<CLResourceManager> resourceManager = make_unique<CLResourceManager>();
        auto resourceVertex = resourceManager->GetResourceShader("CLEngine/src/Shaders/vertex.glsl", GL_VERTEX_SHADER);
        auto resourceFragment = resourceManager->GetResourceShader("CLEngine/src/Shaders/fragment.glsl", GL_FRAGMENT_SHADER);

        

    //Una vez ambos shaders estan inicializados tenemos que linkarlos para que el output de uno vaya al otro
    int shaderProgram = glCreateProgram(); //Como siempre nos devuelve un identificador

    //Bueno aqui es obvio, los enlaza ambos al shaderProgram
    glAttachShader(shaderProgram, resourceVertex->GetShaderID());
    glAttachShader(shaderProgram, resourceFragment->GetShaderID());
    glLinkProgram(shaderProgram);

    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Ha petado el linkado de shaders :(\n";
    }

    //Tecnicamente una vez linkados se pueden borrar los shaders
    glDeleteShader(resourceVertex->GetShaderID());
    glDeleteShader(resourceFragment->GetShaderID());  



    // ------------------------MAGIA--------------------------------
    //Todo preparado, ahora comienza la magia
    // 1. bind Vertex Array Object
    //Todo esto esta muy bien pero lo mejor es tener un array para todos los VBO que queramos dibujar
    unsigned int VBO,VAO;
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);  //Crea un buffer para VBO(Vertex buffer object) con id unico 
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //Fijamos el tipo de buffer (ARRAY_BUFFER)
   
    //GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /**
     * Segunda parte del tutorial para dibujar un cuadrado
     *unsigned int EBO, EBO2;
     *glGenBuffers(1,&EBO);
     *glGenBuffers(1,&EBO2
     *glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     *glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, *STATIC_DRAW); 
     *glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
     *glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, *STATIC_DRAW); 
    */

    /** glVertexAttribPointer
     * 1º Valor: Como pusimos layout = 0 pues ahora mandamos un 0
     * 2º Valor: Numero de datos por bloque que enviamos 3 = vec3
     * 3º Valor: Si normalizamos o no los datos
     * 4º Valor: El tamaño de cada bloque, al ser 3 floats cada vertice entonces 3*sizeof(float)
     * 5º Valor: offset por el que se empieza a leer
     */
    //Por cada layaout del vertex shader los diferenciamos por el primer parametro
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);  

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    

    
    //ImVec4 triangleColor;
    bool show_demo_window = true;
    while (!device->Run()) {

        checkInput(device->GetWindow());

        //Apartir de aqui hacemos cosas, de momento en el main para testear

        device->UpdateViewport(); //Por si reescalamos la ventana


        glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        ImGui::ShowDemoWindow(&show_demo_window);*/


        // Start the Dear ImGui frame
        //ImGui_ImplOpenGL3_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();

        //ImGui::Begin("Modificador de shader!");                          // Create a window called "Hello, world!" and append into it.
        //ImGui::ColorEdit3("clear color", (float*)&triangleColor); // Edit 3 floats representing a color
        //ImGui::End();

        glUseProgram(shaderProgram);
        //int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        //glUniform4f(vertexColorLocation, triangleColor.x, triangleColor.y,triangleColor.z,triangleColor.w);

            //float timeValue = glfwGetTime();
            //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
            //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUseProgram(shaderProgram);
            //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(device->GetWindow());

    }

    //terminateInGUI();

    delete device;

    return 0;
}