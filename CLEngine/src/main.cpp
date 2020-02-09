#include <iostream>
#include <memory>

#include "CLEngine.h"
#include "SceneTree/CLLight.h"
#include "SceneTree/CLNode.h"
#include "ResourceManager/CLResourceManager.h"
#include "ResourceManager/CLResourceShader.h"
#include "ResourceManager/CLResourceMesh.h"
#include "ResourceManager/CLResource.h"

#include "../include/glew/glew.h"
#include "../include/glfw/glfw3.h"
#include <math.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

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

int main() {
    CLEngine *device = new CLEngine(1280, 720, "Beast Brawl");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(device->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");

    
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //----------------------------------
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
    auto resourceMesh = resourceManager->GetResourceMesh("media/kart.obj");

    

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


    //Todo preparado, ahora comienza la magia
    // 1. bind Vertex Array Object
    //Todo esto esta muy bien pero lo mejor es tener un array para todos los VBO que queramos dibujar
    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);  //Crea un buffer para VBO(Vertex buffer object) con id unico 
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //Fijamos el tipo de buffer (ARRAY_BUFFER)
   
    //GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /** glVertexAttribPointer
     * 1º Valor: Como pusimos layout = 0 pues ahora mandamos un 0
     * 2º Valor: Numero de vertices que enviamos 3 = vec3
     * 3º Valor: Si normalizamos o no los datos
     * 4º Valor: El tamaño de cada bloque, al ser 3 floats cada vertice entonces 3*sizeof(float)
     * 5º Valor: offset
     */

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    //TEXTURES
    int width, height, nrChannels; // width, height, numero de colores
    unsigned char *data = stbi_load("CLEngine/container.jpg", &width, &height, &nrChannels, 0); 

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);  //Como todos los ejemplos generamos un ID
    glBindTexture(GL_TEXTURE_2D, texture1);  //Seleccionamos el ID a modificar

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 1) Tipo | 2) Mipmap levels | 3) Tipo de color | 4) Anchura | 5) Altura | 6) Siempre 0 | 7) y 8) Valores de la imagen cargada
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    data = stbi_load("CLEngine/awesomeface.png", &width, &height, &nrChannels, 0); 
    glGenTextures(1, &texture2);  //Como todos los ejemplos generamos un ID
    glBindTexture(GL_TEXTURE_2D, texture2);  //Seleccionamos el ID a modificar

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 1) Tipo | 2) Mipmap levels | 3) Tipo de color | 4) Anchura | 5) Altura | 6) Siempre 0 | 7) y 8) Valores de la imagen cargada
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram,"texture1"),0);
    glUniform1i(glGetUniformLocation(shaderProgram,"texture2"),1);

    
    ImVec4 triangleColor;
    while (!device->Run()) {
        //glfwPollEvents();

        checkInput(device->GetWindow());

        //Apartir de aqui hacemos cosas, de momento en el main para testear

        device->UpdateViewport(); //Por si reescalamos la ventana


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        // int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        // glUniform4f(vertexColorLocation, triangleColor.x, triangleColor.y,triangleColor.z,triangleColor.w);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //resourceMesh->Draw(glm::mat4(1.0));

        glfwPollEvents();
        glfwSwapBuffers(device->GetWindow());

    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete device;

    return 0;
}