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
#include "ResourceManager/CLResourceMaterial.h"
#include "ResourceManager/CLResource.h"



#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"


#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string.h>       /* string */

using namespace std;
using namespace CLE;




int main() {
    CLEngine *device = new CLEngine(1280, 720, "Beast Brawl");

    

    
    //-------------------Resource manager-------------------
    CLResourceManager* resourceManager = CLResourceManager::GetResourceManager();
    auto resourceShader = resourceManager->GetResourceShader("CLEngine/src/Shaders/lightMapping.vert", "CLEngine/src/Shaders/lightMapping.frag");
    auto resourceShaderMaterial = resourceManager->GetResourceShader("CLEngine/src/Shaders/materialShader.vert", "CLEngine/src/Shaders/materialShader.frag");
    auto resourceShader3 = resourceManager->GetResourceShader("CLEngine/src/Shaders/debugShader.vert", "CLEngine/src/Shaders/debugShader.frag", "CLEngine/src/Shaders/debugShader.geom");
    auto resourceShaderSkybox = resourceManager->GetResourceShader("CLEngine/src/Shaders/skybox.vert", "CLEngine/src/Shaders/skybox.frag");
    auto resourceMeshBox = resourceManager->GetResourceMesh("media/TEST_BOX.fbx");
    auto resourceMeshTotem = resourceManager->GetResourceMesh("media/totem_tex.fbx");
    auto resourceMesh = resourceManager->GetResourceMesh("media/kart_physics.fbx");
    auto resourceMeshOBJ = resourceManager->GetResourceMesh("media/kart.obj");
    auto resourceMaterial = resourceManager->GetResourceMaterial("media/kart.obj");

    
    //----------------------------------------------------------------------------------------------------------------SHADER
    
 
    
    //------------------------------------------------------------------------- ARBOLITO

    //Nodo raiz
    //shared_ptr<CLNode> smgr = make_shared<CLNode>(entity1.get());
    CLNode* smgr = device->GetSceneManager();


        auto light1 = smgr->AddLight(1);
        light1->SetShaderProgramID(resourceShader->GetProgramID());
        static_cast<CLLight*>(light1->GetEntity())->SetLightAttributes(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),0.3f,0.004f,0.00016f);

        auto light2 = smgr->AddLight(6);
        light2->SetShaderProgramID(resourceShader->GetProgramID());
        static_cast<CLLight*>(light2->GetEntity())->SetLightAttributes(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),0.3f,0.004f,0.00016f);

        auto light3 = smgr->AddLight(7);
        light3->SetShaderProgramID(resourceShader->GetProgramID());
        static_cast<CLLight*>(light3->GetEntity())->SetLightAttributes(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),0.3f,0.004f,0.00016f);

        auto meshes = smgr->AddGroup(10000);

        auto mesh1 = smgr->AddMesh(2);
        mesh1->SetShaderProgramID(resourceShader->GetProgramID());
        
        auto camera = smgr->AddCamera(3);
        camera->SetShaderProgramID(resourceShader->GetProgramID());

        auto mesh2 = mesh1->AddMesh(4);
        mesh2->SetShaderProgramID(resourceShader->GetProgramID());

        
        auto mesh3 = mesh2->AddMesh(5);
        mesh3->SetShaderProgramID(resourceShaderMaterial->GetProgramID());

        static_cast<CLCamera*>(camera->GetEntity())->SetCameraTarget(mesh1->GetTranslation());

        smgr->AddSkybox("media/skybox/right.jpg",
        "media/skybox/left.jpg",
        "media/skybox/top.jpg",
        "media/skybox/bottom.jpg",
        "media/skybox/front.jpg",
        "media/skybox/back.jpg");

    //smgr->DFSTree(glm::mat4(1.0));
    vector<shared_ptr<CLEntity>> mallas;
    vector<CLNode*> nodes;

    int max = 200;
    int min = -200;
    int j = 0;
    // for(int i = 50; i<100; i++){
    //     nodes.push_back(meshes->AddMesh(i));
    //     nodes[j]->SetShaderProgramID(resourceShader->GetProgramID());

    //     int randNumX = rand()%(max-min + 1) + min;
    //     int randNumY = rand()%(max-min + 1) + min;
    //     int randNumZ = rand()%(max-min + 1) + min;
    //     static_cast<CLMesh*>(nodes[j]->GetEntity())->SetMesh(resourceMeshTotem);
    //     nodes[j]->SetTranslation(glm::vec3(randNumX,randNumY,randNumZ));
    //     j++;
    // }

    //      smgr->DrawTree(smgr);


    static_cast<CLMesh*>(mesh1->GetEntity())->SetMesh(resourceMeshBox);
    static_cast<CLMesh*>(mesh2->GetEntity())->SetMesh(resourceMesh);
    static_cast<CLMesh*>(mesh3->GetEntity())->SetMesh(resourceMeshOBJ);
    static_cast<CLMesh*>(mesh3->GetEntity())->SetMaterial(resourceMaterial); 

    camera->SetTranslation(glm::vec3(80.0f, 5.0f, -9.0f));
    mesh1->SetScalation(glm::vec3(2.0f, 2.0f, 2.0f));
    mesh1->SetRotation(glm::vec3(0.0f,0.0f,0.0f));
    mesh1->SetTranslation(glm::vec3(50.0f,0.0f,0.0f));
    mesh2->SetScalation(glm::vec3(0.2f, 0.2f, 0.2f));
    mesh2->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    mesh2->SetTranslation(glm::vec3(10.0f,0.0f,0.0f));
    mesh3->SetTranslation(glm::vec3(-40.0f,0.0f,0.0f));
    mesh3->SetScalation(glm::vec3(0.8f,0.8f,0.8f));

    mesh2->GetGlobalTranslation();


    cout << "Borra bien\n";
    //LUCES Y COLORES
    float auxCameraPos[3] = {camera->GetTranslation().x, camera->GetTranslation().y, camera->GetTranslation().z};
    float auxLightPos[3] = {light1->GetTranslation().x, light1->GetTranslation().y, light1->GetTranslation().z};
    float auxLightPos2[3] = {light2->GetTranslation().x, light2->GetTranslation().y, light2->GetTranslation().z};
    float auxLightPos3[3] = {light3->GetTranslation().x, light3->GetTranslation().y, light3->GetTranslation().z};

    float index = 0.01;

    double previousTime = glfwGetTime();
    int frameCount = 0;
    auto lights = smgr->GetLights();

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
        ImGui::SliderFloat3("LightPos2",auxLightPos2,-300,400);
        ImGui::SliderFloat3("LightPos3",auxLightPos3,-300,400);
        ImGui::End(); 

        glm::vec3 cameraPos(auxCameraPos[0], auxCameraPos[1], auxCameraPos[2]);
        glm::vec3 lightPos(auxLightPos[0], auxLightPos[1], auxLightPos[2]);
        glm::vec3 lightPos2(auxLightPos2[0], auxLightPos2[1], auxLightPos2[2]);
        glm::vec3 lightPos3(auxLightPos3[0], auxLightPos3[1], auxLightPos3[2]);
        camera->SetTranslation(cameraPos);
        light1->SetTranslation(lightPos);
        light2->SetTranslation(lightPos2);
        light3->SetTranslation(lightPos3);
        
        static_cast<CLCamera*>(camera->GetEntity())->SetCameraTarget(mesh3->GetGlobalTranslation());


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