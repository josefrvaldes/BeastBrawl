#include <iostream>
#include <memory>
#include <math.h>

// INCLUDES
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


//SRC
#include "CLEngine.h"
#include "SceneTree/CLLight.h"
#include "SceneTree/CLCamera.h"
#include "SceneTree/CLNode.h"
#include "SceneTree/CLMesh.h"
#include "ResourceManager/CLResourceManager.h"
#include "ResourceManager/CLResourceShader.h"
#include "ResourceManager/CLResourceMesh.h"
#include "ResourceManager/CLResource.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"


using namespace std;
using namespace CLE;


/**
 * Mira si se ha pulsado ESC para cerrar la ventana.
 * @param window - Ventana sobre la que mira los eventos. 
 */
void checkInput (GLFWwindow *window, glm::vec3 cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    const float cameraSpeed = 0.15f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void initImGUI(CLEngine *device) {
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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    CLEngine *device = new CLEngine(1280, 720, "Beast Brawl");
    initImGUI(device);

    // configure global opengl state. Z depth active.
    glEnable(GL_DEPTH_TEST);

    
    //-------------------Resource manager-------------------
    unique_ptr<CLResourceManager> resourceManager = make_unique<CLResourceManager>();
    auto resourceShader = resourceManager->GetResourceShader("CLEngine/src/Shaders/vertex.glsl", "CLEngine/src/Shaders/fragment.glsl");
    auto resourceMesh = resourceManager->GetResourceMesh("media/kart.obj");

    //----------------------------------------------------------------------------------------------------------------SHADER
    

    
    //------------------------------------------------------------------------- ARBOLITO
    unique_ptr<CLEntity> entity1 = make_unique<CLLight>(1);
    unique_ptr<CLEntity> entity2 = make_unique<CLMesh>(2);
    unique_ptr<CLEntity> entity3 = make_unique<CLCamera>(3);
    unique_ptr<CLEntity> entity4 = make_unique<CLMesh>(4);
    unique_ptr<CLEntity> entity5 = make_unique<CLLight>(5);
    
    //Nodo raiz
    unique_ptr<CLNode> smgr = make_unique<CLNode>(entity1.get());
    smgr->SetShaderProgramID(resourceShader->GetProgramID());
    cout << "MODEL MATRIX ID:: " << smgr->GetModelMatrixID() << endl;

    unique_ptr<CLNode> node2 = make_unique<CLNode>(entity2.get());
    node2->SetShaderProgramID(resourceShader->GetProgramID());
    unique_ptr<CLNode> node3 = make_unique<CLNode>(entity3.get());
    node3->SetShaderProgramID(resourceShader->GetProgramID());
    unique_ptr<CLNode> node4 = make_unique<CLNode>(entity4.get());
    node4->SetShaderProgramID(resourceShader->GetProgramID());
    unique_ptr<CLNode> node5 = make_unique<CLNode>(entity5.get());
    node5->SetShaderProgramID(resourceShader->GetProgramID());

        smgr->AddChild(node2.get());
        smgr->AddChild(node3.get());
        node2->AddChild(node4.get());
        node4->AddChild(node5.get());

        smgr->DrawTree(smgr.get());

    //smgr->DFSTree(glm::mat4(1.0));

    static_cast<CLMesh*>(entity2.get())->SetMesh(resourceMesh);
    static_cast<CLMesh*>(entity4.get())->SetMesh(resourceMesh);

    //smgr.get()->SetTranslation(glm::vec3(-20.0f, 0.0f, -30.0f));
    node3.get()->SetTranslation(glm::vec3(0.0f, 7.0f, 60.0f));
    node2.get()->SetScalation(glm::vec3(0.25f, 0.25f, 0.25f));
    node4.get()->SetTranslation(glm::vec3(0.0f, 30.0f, 0.0f));
    node4.get()->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));

    
    

    //Todo preparado, ahora comienza la magia
    // 1. bind Vertex Array Object
    //Todo esto esta muy bien pero lo mejor es tener un array para todos los VBO que queramos dibujar
    // unsigned int VBO,VAO;
    // glGenVertexArrays(1, &VAO); 
    // glGenBuffers(1, &VBO);  //Crea un buffer para VBO(Vertex buffer object) con id unico 
    // glBindVertexArray(VAO);

    // // 2. copy our vertices array in a buffer for OpenGL to use
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);  //Fijamos el tipo de buffer (ARRAY_BUFFER)
   
    // //GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /** glVertexAttribPointer
     * 1º Valor: Como pusimos layout = 0 pues ahora mandamos un 0
     * 2º Valor: Numero de datos por bloque que enviamos 3 = vec3
     * 3º Valor: Si normalizamos o no los datos
     * 4º Valor: El tamaño de cada bloque, al ser 3 floats cada vertice entonces 3*sizeof(float)
     * 5º Valor: offset por el que se empieza a leer
     */

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    

    /**
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

    
    glUseProgram(resourceShader->GetProgramID());
    glUniform1i(glGetUniformLocation(resourceShader->GetProgramID(),"texture1"),0);
    glUniform1i(glGetUniformLocation(resourceShader->GetProgramID(),"texture2"),1);
    *
    */
    
    //glm::vec3 cameraPos   = glm::vec3(0.0f, 7.0f,  60.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    //LUCES Y COLORES
    glm::vec3 color(1.0f, 0.0f, 0.0f);
    glm::vec3 light(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos(node3->GetTranslation());
    float auxColor[3] = {color.x,color.y,color.z};
    float auxLight[3] = {light.x,light.y,light.z};
    float auxLightPos[3] = {lightPos.x,lightPos.y,lightPos.z};
    float auxCameraPos[3] = {node3.get()->GetTranslation().x, node3.get()->GetTranslation().y, node3.get()->GetTranslation().z};
    cout << "Posicion de la camara: " << node3.get()->GetTranslation().x << " - " << node3.get()->GetTranslation().y << " - " << node3.get()->GetTranslation().z << endl;
    float specularStrength = 0.5;
    int shinnines = 32;

    float index = 0.01;
    while (!device->Run()) {

        checkInput(device->GetWindow(), node3.get()->GetTranslation(), cameraFront, cameraUp);

        //Apartir de aqui hacemos cosas, de momento en el main para testear

        device->UpdateViewport(); //Por si reescalamos la ventana

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Modifica ilumnacion"); 
        ImGui::SliderFloat3("Color",auxColor,0,1);
        ImGui::SliderFloat3("Light",auxLight,0,1);
        ImGui::SliderFloat3("LightPos",auxLightPos,-100,100);
        ImGui::SliderFloat3("CameraPos",auxCameraPos,-50,50);
        ImGui::SliderFloat("specularStrength",&specularStrength,0,1);
        ImGui::SliderInt("Shinnines",&shinnines,0,256);
        ImGui::End(); 

        glm::vec3 color(auxColor[0], auxColor[1], auxColor[2]);
        glm::vec3 light(auxLight[0], auxLight[1], auxLight[2]);
        glm::vec3 lightPos(auxLightPos[0], auxLightPos[1], auxLightPos[2]);
        glm::vec3 cameraPos(auxCameraPos[0], auxCameraPos[1], auxCameraPos[2]);

        glUseProgram(resourceShader->GetProgramID());
        //Luces y colores
        glUniform3fv(glGetUniformLocation(resourceShader->GetProgramID(), "objectColor"),1,glm::value_ptr(color));
        glUniform3fv(glGetUniformLocation(resourceShader->GetProgramID(), "lightColor"),1,glm::value_ptr(light));
        glUniform3fv(glGetUniformLocation(resourceShader->GetProgramID(), "lightPos"),1,glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(resourceShader->GetProgramID(), "viewPos"),1,glm::value_ptr(cameraPos));
        glUniform1f(glGetUniformLocation(resourceShader->GetProgramID(), "specularStrength"),specularStrength);
        glUniform1i(glGetUniformLocation(resourceShader->GetProgramID(), "shininess"),shinnines);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);


        // create transformations
        glm::mat4 projection    = static_cast<CLCamera*>(node3.get()->GetEntity())->CalculateProjectionMatrix();
       
        glm::mat4 view;
        // Vector posicion de la camara, vector de posicion destino y vector ascendente en el espacio mundial. 
        view = glm::lookAt(node3.get()->GetTranslation(), node3.get()->GetTranslation() + cameraFront, cameraUp);

        // pass transformation matrices to the shader
        glUniformMatrix4fv(glGetUniformLocation(resourceShader->GetProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(resourceShader->GetProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        //node2->SetTranslation(glm::vec3(index,0.1f,0.1f));
        //smgr->SetRotation(glm::vec3(0.0f,0.0f,index));
        node2->SetRotation(glm::vec3(index,0.0f,0.0f));
        smgr->DFSTree(glm::mat4(1.0));



        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(device->GetWindow());
        index += 0.2;
    }

    terminateInGUI();

    delete device;

    return 0;
}