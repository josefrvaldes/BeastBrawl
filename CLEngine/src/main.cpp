#include <iostream>
#include <memory>
#include <math.h>

// INCLUDES
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>


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


#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;
using namespace CLE;




int main() {
    CLEngine *device = new CLEngine(1280, 720, "Beast Brawl");

    

    
    //-------------------Resource manager-------------------
    shared_ptr<CLResourceManager> resourceManager = make_shared<CLResourceManager>();
    auto resourceShader = resourceManager->GetResourceShader("CLEngine/src/Shaders/vertex.glsl", "CLEngine/src/Shaders/fragment.glsl");
    auto resourceShader2 = resourceManager->GetResourceShader("CLEngine/src/Shaders/phongMaterialVert.glsl", "CLEngine/src/Shaders/phongMaterialFrag.glsl");
    auto resourceMeshBox = resourceManager->GetResourceMesh("media/TEST_BOX.fbx");
    auto resourceMeshTotem = resourceManager->GetResourceMesh("media/totem_tex.fbx");
    auto resourceMesh = resourceManager->GetResourceMesh("media/kart_physics.fbx");
    auto resourceMeshOBJ = resourceManager->GetResourceMesh("media/kart.obj");

    //----------------------------------------------------------------------------------------------------------------SHADER
    
 
    
    //------------------------------------------------------------------------- ARBOLITO

    //Nodo raiz
    //shared_ptr<CLNode> smgr = make_shared<CLNode>(entity1.get());
    CLNode* smgr = device->GetSceneManager();


        auto light1 = smgr->AddLight(1);
        light1->SetShaderProgramID(resourceShader->GetProgramID());
        static_cast<CLLight*>(light1->GetEntity())->SetLightAttributes(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),1.0f,0.09f,0.032f);

        auto meshes = smgr->AddGroup(10000);

        auto mesh1 = smgr->AddMesh(2);
        mesh1->SetShaderProgramID(resourceShader->GetProgramID());

        auto camera = smgr->AddCamera(3);
        camera->SetShaderProgramID(resourceShader->GetProgramID());

        auto mesh2 = mesh1->AddMesh(4);
        mesh2->SetShaderProgramID(resourceShader->GetProgramID());

        
        auto mesh3 = mesh2->AddMesh(5);
        mesh3->SetShaderProgramID(resourceShader2->GetProgramID());

        static_cast<CLCamera*>(camera->GetEntity())->SetCameraTarget(mesh1->GetTranslation());

    //smgr->DFSTree(glm::mat4(1.0));
    vector<shared_ptr<CLEntity>> mallas;
    vector<CLNode*> nodes;

    int max = 200;
    int min = -200;
    int j = 0;
    for(int i = 50; i<100; i++){
        nodes.push_back(meshes->AddMesh(i));
        nodes[j]->SetShaderProgramID(resourceShader->GetProgramID());

        int randNumX = rand()%(max-min + 1) + min;
        int randNumY = rand()%(max-min + 1) + min;
        int randNumZ = rand()%(max-min + 1) + min;
        static_cast<CLMesh*>(nodes[j]->GetEntity())->SetMesh(resourceMeshTotem);
        nodes[j]->SetTranslation(glm::vec3(randNumX,randNumY,randNumZ));
        j++;
    }

    //      smgr->DrawTree(smgr);


    static_cast<CLMesh*>(mesh1->GetEntity())->SetMesh(resourceMeshBox);
    static_cast<CLMesh*>(mesh2->GetEntity())->SetMesh(resourceMesh);
    static_cast<CLMesh*>(mesh3->GetEntity())->SetMesh(resourceMeshOBJ);

    camera->SetTranslation(glm::vec3(0.0f, 7.0f, 60.0f));
    mesh1->SetScalation(glm::vec3(2.0f, 2.0f, 2.0f));
    mesh1->SetRotation(glm::vec3(0.0f,0.0f,0.0f));
    mesh1->SetTranslation(glm::vec3(50.0f,0.0f,0.0f));
    mesh2->SetScalation(glm::vec3(0.2f, 0.2f, 0.2f));
    mesh2->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    mesh2->SetTranslation(glm::vec3(10.0f,0.0f,0.0f));
    mesh3->SetTranslation(glm::vec3(-40.0f,0.0f,0.0f));
    mesh3->SetScalation(glm::vec3(2.0f,2.0f,2.0f));

    mesh2->GetGlobalTranslation();

    
    
    #pragma region Movidas

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
    #pragma endregion
    
    


    //LUCES Y COLORES
    float auxCameraPos[3] = {camera->GetTranslation().x, camera->GetTranslation().y, camera->GetTranslation().z};
    float auxLightPos[3] = {light1->GetTranslation().x, light1->GetTranslation().y, light1->GetTranslation().z};

    float index = 0.01;

    double previousTime = glfwGetTime();
    int frameCount = 0;


    //static_cast<CLCamera*>(camera->GetEntity())->SetPerspective(false);
    while (device->Run()) {

        //checkInput(device->GetWindow(), cameraPos, cameraFront, cameraUp);

        //Apartir de aqui hacemos cosas, de momento en el main para testear

        device->UpdateViewport(); //Por si reescalamos la ventana

        device->BeginScene();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Modifica ilumnacion"); 
        ImGui::SliderFloat3("CameraPos",auxCameraPos,-50,400);
        ImGui::SliderFloat3("LightPos",auxLightPos,-300,400);
        ImGui::End(); 

        glm::vec3 cameraPos(auxCameraPos[0], auxCameraPos[1], auxCameraPos[2]);
        glm::vec3 lightPos(auxLightPos[0], auxLightPos[1], auxLightPos[2]);
        camera->SetTranslation(cameraPos);
        light1->SetTranslation(lightPos);


        //Luces y colores
        //glUniform3fv(glGetUniformLocation(resourceShader->GetProgramID(), "objectColor"),1,glm::value_ptr(color));
        //glUniform3fv(glGetUniformLocation(resourceShader->GetProgramID(), "lightColor"),1,glm::value_ptr(light));
        //glUniform3fv(glGetUniformLocation(resourceShader->GetProgramID(), "viewPos"),1,glm::value_ptr(camera->GetTranslation()));

        // glUniform3f(glGetUniformLocation(resourceShader2->GetProgramID(), "material.ambient"), 0.0215,0.175,0.0215);
        // glUniform3f(glGetUniformLocation(resourceShader2->GetProgramID(), "material.diffuse2"), 0.075,0.614,0.075);
        // glUniform3f(glGetUniformLocation(resourceShader2->GetProgramID(), "material.specular"), 0.633,0.727,0.633);
        glUniform1i(glGetUniformLocation(resourceShader->GetProgramID(), "material.shininess"), 76.8);

        glUniform3fv(glGetUniformLocation(resourceShader->GetProgramID(), "light.position"),1,glm::value_ptr(lightPos));
        glUniform3f(glGetUniformLocation(resourceShader->GetProgramID(),  "light.ambient"), 0.2,0.2,0.2);
        glUniform3f(glGetUniformLocation(resourceShader->GetProgramID(),  "light.diffuse"), 0.5,0.5,0.5);
        glUniform3f(glGetUniformLocation(resourceShader->GetProgramID(),  "light.specular"), 1.0,1.0,1.0);
        glUniform1f(glGetUniformLocation(resourceShader->GetProgramID(),  "light.constant"),1.0);
        glUniform1f(glGetUniformLocation(resourceShader->GetProgramID(),  "light.linear"), 0.09);
        glUniform1f(glGetUniformLocation(resourceShader->GetProgramID(),  "light.quadratic"), 0.032);

        
        //meshes->SetRotation(glm::vec3(0.0f,0.0f,index));
        // auto trans1 = mesh1->GetTranslation();
        // mesh1->SetTranslation(glm::vec3(trans1.x+index,trans1.y,trans1.z));
        static_cast<CLCamera*>(camera->GetEntity())->SetCameraTarget(mesh2->GetGlobalTranslation());



        // Measure speed
        double currentTime = glfwGetTime();
        frameCount++;
        // If a second has passed.
        if ( currentTime - previousTime >= 1.0 )
        {
            // Display the frame count here any way you want.
            cout << frameCount << endl;

            frameCount = 0;
            previousTime = currentTime;
        }


        if (glfwGetKey(device->GetWindow(), GLFW_KEY_F1)) {
            smgr->DeleteNode(mesh2->GetEntity()->GetID());
        }
        
        device->DrawObjects();
        device->InputClose();
        device->PollEvents();
        device->RenderImgui();
        device->EndScene();
        index += 0.2;
    } 


    delete device;

    return 0;
}