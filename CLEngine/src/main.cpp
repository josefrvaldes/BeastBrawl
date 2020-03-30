#include <iostream>
#include <memory>
#include <map>

// INCLUDES
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>


//SRC
#include "CLEngine.h"
#include "SceneTree/CLPointLight.h"
#include "SceneTree/CLCamera.h"
#include "SceneTree/CLNode.h"
#include "SceneTree/CLMesh.h"
#include "ResourceManager/CLResourceManager.h"
#include "ResourceManager/CLResourceShader.h"
#include "ResourceManager/CLResourceMesh.h"
#include "ResourceManager/CLResourceMaterial.h"
#include "ResourceManager/CLResource.h"
#include "Built-In-Classes/CLColor.h"



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
    auto resourceShader = resourceManager->GetResourceShader("CLEngine/src/Shaders/shadowMappingShader.vert", "CLEngine/src/Shaders/shadowMappingShader.frag");
    auto resourceShaderCartoon = resourceManager->GetResourceShader("CLEngine/src/Shaders/cartoonShader.vert", "CLEngine/src/Shaders/cartoonShader.frag");
    auto resourceShaderMaterial = resourceManager->GetResourceShader("CLEngine/src/Shaders/materialShader.vert", "CLEngine/src/Shaders/materialShader.frag");
    auto resourceShader3 = resourceManager->GetResourceShader("CLEngine/src/Shaders/debugShader.vert", "CLEngine/src/Shaders/debugShader.frag");
    auto resourceShaderSkybox = resourceManager->GetResourceShader("CLEngine/src/Shaders/skybox.vert", "CLEngine/src/Shaders/skybox.frag");
    auto resourceMeshGround = resourceManager->GetResourceMesh("media/training_ground.obj", true);
    auto resourceMeshTotem = resourceManager->GetResourceMesh("media/totem_tex.fbx", true);
    auto resourceMesh = resourceManager->GetResourceMesh("media/kart_physics.fbx", true);
    auto resourceMeshBox = resourceManager->GetResourceMesh("media/TEST_BOX.fbx", true);
    // auto resourceMeshOBJ = resourceManager->GetResourceMesh("media/kart.obj", true);
    // auto resourceMaterial = resourceManager->GetResourceMaterial("media/kart.obj", true);


    
    //----------------------------------------------------------------------------------------------------------------SHADER
    
 
    
    //------------------------------------------------------------------------- ARBOLITO

    //Nodo raiz
    //shared_ptr<CLNode> smgr = make_shared<CLNode>(entity1.get());
    CLNode* smgr = device->GetSceneManager();


        auto light1 = smgr->AddPointLight(1);
        light1->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        static_cast<CLPointLight*>(light1->GetEntity())->SetLightAttributes(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.1f,0.1,0.1f),1.0f,0.00002f,0.00000006f);

        // auto light2 = smgr->AddPointLight(6);
        // light2->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        // static_cast<CLPointLight*>(light2->GetEntity())->SetLightAttributes(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),1.0f,0.0014f,0.000003f);

        // auto light2 = smgr->AddDirectLight(6);
        // light2->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        // static_cast<CLDirectLight*>(light2->GetEntity())->SetLightAttributes(glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),1.0f,0.000002f,0.00000002f);

        // auto light3 = smgr->AddSpotLight(7);
        // light3->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        // static_cast<CLSpotLight*>(light3->GetEntity())->SetLightAttributes(glm::vec3(1.0f,0.0f,0.0f),glm::cos(glm::radians(12.5f)),glm::cos(glm::radians(15.5f)),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),0.3f,0.004f,0.00016f);

        auto meshes = smgr->AddGroup(10000);

        auto mesh1 = smgr->AddMesh(2);
        mesh1->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        
        auto camera = smgr->AddCamera(3);
        camera->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

        auto mesh2 = smgr->AddMesh(4);
        mesh2->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

        
        auto mesh3 = smgr->AddMesh(54212);
        mesh3->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

        auto mesh4 = mesh3->AddMesh(102123);
        mesh4->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        

        static_cast<CLCamera*>(camera->GetEntity())->SetCameraTarget(mesh2->GetTranslation());


        smgr->AddSkybox("media/skybox/right.jpg",
        "media/skybox/left.jpg",
        "media/skybox/top.jpg",
        "media/skybox/bottom.jpg",
        "media/skybox/front.jpg",
        "media/skybox/back.jpg");

        smgr->AddShadowMapping(light1->GetEntity()->GetID());



    static_cast<CLMesh*>(mesh1->GetEntity())->SetMesh(resourceMeshGround);
    static_cast<CLMesh*>(mesh2->GetEntity())->SetMesh(resourceMesh);
    static_cast<CLMesh*>(mesh3->GetEntity())->SetMesh(resourceMeshBox);
    static_cast<CLMesh*>(mesh4->GetEntity())->SetMesh(resourceMeshTotem);

    camera->SetTranslation(glm::vec3(400.127f, 400.42f, 0.9f));
    light1->SetTranslation(glm::vec3(75.9f, 1000.2f, 15.08f));

    mesh1->SetScalation(glm::vec3(1.0f, 1.0f, 1.0f));
    mesh1->SetRotation(glm::vec3(0.0f,0.0f,0.0f));
    mesh1->SetTranslation(glm::vec3(50.0f,50.0f,50.0f));

    mesh2->SetScalation(glm::vec3(3.5f, 3.5f, 3.5f));
    mesh2->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    mesh2->SetTranslation(glm::vec3(50.0f,80.0f,-50.0f));

    mesh3->SetTranslation(glm::vec3(240.0f,200.0f,0.0f));
    mesh3->SetScalation(glm::vec3(10.0f,10.0f,10.0f));
    mesh3->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    // light2->SetTranslation(mesh2->GetGlobalTranslation());

    mesh4->SetScalation(glm::vec3(2.0f, 2.0f, 2.0f));
    mesh4->SetTranslation(glm::vec3(0.0f,0.0f, -10.0f));


    //LUCES Y COLORES
    float auxCameraPos[3] = {camera->GetTranslation().x, camera->GetTranslation().y, camera->GetTranslation().z};
    float auxLightPos[3] = {light1->GetTranslation().x, light1->GetTranslation().y, light1->GetTranslation().z};
    // float auxLightPos2[3] = {light2->GetTranslation().x, light2->GetTranslation().y, light2->GetTranslation().z};

    float index = 0.01;

    double previousTime = glfwGetTime();
    int frameCount = 0;

    CLNode* light3 = nullptr;
        
    bool deleteMesh3 = false;
    while (device->Run()) {
        
        //Apartir de aqui hacemos cosas, de momento en el main para testear
        device->UpdateViewport(); //Por si reescalamos la ventana
        device->BeginScene();

        if(glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE) && light3 == nullptr){
            light3 = smgr->AddSpotLight(7);
            light3->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
            static_cast<CLSpotLight*>(light3->GetEntity())->SetLightAttributes(glm::vec3(0.0f,-1.0f,0.0f),glm::cos(glm::radians(12.5f)),glm::cos(glm::radians(20.0f)),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),1.0f,0.0000002f,0.000000009f);
            light3->SetTranslation(glm::vec3(mesh1->GetGlobalTranslation().x,1000.0f,mesh1->GetGlobalTranslation().z+50));
            // auxCameraPos[0] = 40;
            // auxCameraPos[1] = 400;
            // auxCameraPos[2] = 0;

            // light1->SetTranslation(glm::vec3(10000.0f,10000.0f,100000.0f));
            auxLightPos[0] = 10000.0f;
            auxLightPos[1] = 10000.0f;
            auxLightPos[2] = 10000.0f;
        }

        if(glfwGetKey(device->GetWindow(),GLFW_KEY_R) && deleteMesh3 == false){
            smgr->DeleteNode(mesh3->GetEntity()->GetID());
            deleteMesh3 = true;
        }
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Modifica ilumnacion"); 
        ImGui::SliderFloat3("CameraPos",auxCameraPos,-600,600);
        ImGui::SliderFloat3("LightPos",auxLightPos,-1000,1000);
        // ImGui::SliderFloat3("LightPos2",auxLightPos2,-1000,1000);
        ImGui::End(); 

        glm::vec3 cameraPos(auxCameraPos[0], auxCameraPos[1], auxCameraPos[2]);
        glm::vec3 lightPos(auxLightPos[0], auxLightPos[1], auxLightPos[2]);
        // glm::vec3 lightPos2(auxLightPos2[0], auxLightPos2[1], auxLightPos2[2]);
        camera->SetTranslation(cameraPos);
        light1->SetTranslation(lightPos);
        // light2->SetTranslation(lightPos2); 
        
        if(!deleteMesh3){
            mesh3->SetRotation(glm::vec3(0.0f,0.0f,index));
            mesh4->SetTranslation(glm::vec3(sin(index/10.0)*10,mesh4->GetTranslation().y,cos(index/10.0)*10));

        }
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



        device->DrawObjects();

        string file = "media/pudin.png";
        device->DrawImage2D(25.0f,25.0f,150.0f,150.0f, 0.2f, file, true);


            //TEXTO -----------------
            string cadena = "Demo tecnica CLEngine";
            glm::vec3 vect3 = glm::vec3(0.5, 0.8f, 0.2f);
        device->RenderText2D(cadena, 25.0f, 25.0f, 0.05f, 1.0f, vect3);


        device->InputClose();
        device->PollEvents();
        device->RenderImgui();
        device->EndScene();
        index += 0.2;

    } 


    delete device;

    return 0;
}