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

    try {

    
        cout << "Antes de crear CLEngine" << endl;
        CLEngine *device = new CLEngine(1280, 720, "Beast Brawl");
        cout << "Después de crear CLEngine" << endl;

        //-------------------Resource manager-------------------
        CLResourceManager* resourceManager = CLResourceManager::GetResourceManager();
        auto resourceShader = resourceManager->GetResourceShader("CLEngine/src/Shaders/shadowMappingShader.vert", "CLEngine/src/Shaders/shadowMappingShader.frag");
        auto resourceShaderCartoon = resourceManager->GetResourceShader("CLEngine/src/Shaders/cartoonShader.vert", "CLEngine/src/Shaders/cartoonShader.frag");
        auto resourceShaderMaterial = resourceManager->GetResourceShader("CLEngine/src/Shaders/materialShader.vert", "CLEngine/src/Shaders/materialShader.frag");
        auto resourceShader3 = resourceManager->GetResourceShader("CLEngine/src/Shaders/debugShader.vert", "CLEngine/src/Shaders/debugShader.frag");
        auto resourceShaderSkybox = resourceManager->GetResourceShader("CLEngine/src/Shaders/skybox.vert", "CLEngine/src/Shaders/skybox.frag");
        auto resourceMeshGround = resourceManager->GetResourceMesh("media/training_ground.obj", true);
        // auto resourceMeshTotem = resourceManager->GetResourceMesh("media/totem_tex.fbx", true);
        auto resourceMesh = resourceManager->GetResourceMesh("media/kart_physics.obj", true);
        // //auto resourceMesh2 = resourceManager->GetResourceMesh("media/kart_physics4.fbx", true);
        // auto resourceMeshOBJ = resourceManager->GetResourceMesh("media/kart.obj", true);
        // auto resourceMaterial = resourceManager->GetResourceMaterial("media/kart.obj", true);

        cout << "+++++++ He compilado los shaders" << endl;

        
        //----------------------------------------------------------------------------------------------------------------SHADER
        
    
        
        //------------------------------------------------------------------------- ARBOLITO

        //Nodo raiz
        //shared_ptr<CLNode> smgr = make_shared<CLNode>(entity1.get());
        CLNode* smgr = device->GetSceneManager();


            auto light1 = smgr->AddPointLight(1);
            light1->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
            static_cast<CLPointLight*>(light1->GetEntity())->SetLightAttributes(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),0.3f,0.004f,0.00016f);

            auto light2 = smgr->AddDirectLight(6);
            light2->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
            static_cast<CLDirectLight*>(light2->GetEntity())->SetLightAttributes(glm::vec3(1.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),0.3f,0.004f,0.00016f);

            auto light3 = smgr->AddSpotLight(7);
            light3->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
            static_cast<CLSpotLight*>(light3->GetEntity())->SetLightAttributes(glm::vec3(1.0f,0.0f,0.0f),glm::cos(glm::radians(12.5f)),glm::cos(glm::radians(15.5f)),glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.5f,0.5f,0.5f),glm::vec3(0.2f,0.3f,0.42f),glm::vec3(0.1f,0.1,0.1f),0.3f,0.004f,0.00016f);

            auto meshes = smgr->AddGroup(10000);

            auto mesh1 = smgr->AddMesh(2);
            mesh1->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
            
            auto camera = smgr->AddCamera(3);
            camera->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

            auto mesh2 = smgr->AddMesh(4);
            mesh2->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

            
            // auto mesh3 = smgr->AddMesh(5);
            // mesh3->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

            // auto mesh4 = smgr->AddMesh(6);
            // mesh4->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
            

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
        // static_cast<CLMesh*>(mesh3->GetEntity())->SetMesh(resourceMesh);
        //static_cast<CLMesh*>(mesh3->GetEntity())->SetMaterial(resourceMaterial); 
        // static_cast<CLMesh*>(mesh4->GetEntity())->SetMesh(resourceMeshBox);

        camera->SetTranslation(glm::vec3(121.127f, 70.42f, -11.9f));
        light1->SetTranslation(glm::vec3(54.9f, 35.2f, -14.08f));
        mesh1->SetScalation(glm::vec3(0.5f, 0.5f, 0.5f));
        mesh1->SetRotation(glm::vec3(0.0f,0.0f,0.0f));
        mesh1->SetTranslation(glm::vec3(50.0f,50.0f,50.0f));
        mesh2->SetScalation(glm::vec3(0.5f, 0.5f, 0.5f));
        mesh2->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh2->SetTranslation(glm::vec3(10.0f,0.0f,0.0f));
        // mesh3->SetTranslation(glm::vec3(-40.0f,0.0f,0.0f));
        // mesh3->SetScalation(glm::vec3(0.5f,0.5f,0.5f));
        // mesh3->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));


        // mesh4->SetScalation(glm::vec3(100.0f, 100.0f, 10.0f));
        // mesh4->SetTranslation(glm::vec3(65.0f,50.0f, 70.0f));


        //LUCES Y COLORES
        float auxCameraPos[3] = {camera->GetTranslation().x, camera->GetTranslation().y, camera->GetTranslation().z};
        float auxLightPos[3] = {light1->GetTranslation().x, light1->GetTranslation().y, light1->GetTranslation().z};
        float auxLightPos2[3] = {light2->GetTranslation().x, light2->GetTranslation().y, light2->GetTranslation().z};
        float auxLightPos3[3] = {light3->GetTranslation().x, light3->GetTranslation().y, light3->GetTranslation().z};
        float auxLineStart[3] = {0.0,0.0,0.0};
        float auxLineEnd[3] = {200.0,200.0,200.0};

        float index = 0.01;

        double previousTime = glfwGetTime();
        int frameCount = 0;
        auto lights = smgr->GetPointLights();

        while (device->Run()) {
            
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
            ImGui::SliderFloat3("Line start",auxLineStart,-400,400);
            ImGui::SliderFloat3("Line end",auxLineEnd,-400,400);
            ImGui::End(); 

            glm::vec3 cameraPos(auxCameraPos[0], auxCameraPos[1], auxCameraPos[2]);
            glm::vec3 lightPos(auxLightPos[0], auxLightPos[1], auxLightPos[2]);
            glm::vec3 lightPos2(auxLightPos2[0], auxLightPos2[1], auxLightPos2[2]);
            glm::vec3 lightPos3(auxLightPos3[0], auxLightPos3[1], auxLightPos3[2]);
            camera->SetTranslation(cameraPos);
            light1->SetTranslation(lightPos);
            light2->SetTranslation(lightPos2);
            light3->SetTranslation(lightPos3);
            

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

            string file = "media/pudin.png";
            device->DrawImage2D(25.0f,25.0f,150.0f,150.0f, 0.2f, file, true);


                //TEXTO -----------------
                string cadena = "Buenos dias, achis achis D':";
                glm::vec3 vect3 = glm::vec3(0.5, 0.8f, 0.2f);
            device->RenderText2D(cadena, 25.0f, 25.0f, 0.05f, 1.0f, vect3);


            device->InputClose();
            device->PollEvents();
            device->RenderImgui();
            device->EndScene();
            index += 0.2;

        } 


        delete device;

    } catch(std::exception &ex) {
        cout << "Hubo una excepción " << ex.what() << endl;
    }

    return 0;
}