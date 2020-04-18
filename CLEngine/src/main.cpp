#include <iostream>
#include <map>
#include <memory>
#include <string>

// INCLUDES
#include <ft2build.h>
#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include FT_FREETYPE_H

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

//SRC
#include <stdio.h>  /* printf, scanf, puts, NULL */
#include <stdlib.h> /* srand, rand */
#include <string.h> /* string */
#include <time.h>   /* time */

#include "../../src/Systems/Utils.h"
#include "Built-In-Classes/CLColor.h"
#include "CLEngine.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ResourceManager/CLResource.h"
#include "ResourceManager/CLResourceManager.h"
#include "ResourceManager/CLResourceMaterial.h"
#include "ResourceManager/CLResourceMesh.h"
#include "ResourceManager/CLResourceShader.h"
#include "SceneTree/CLCamera.h"
#include "SceneTree/CLMesh.h"
#include "SceneTree/CLNode.h"
#include "SceneTree/CLParticleSystem.h"
#include "SceneTree/CLPointLight.h"

using namespace std;
using namespace CLE;

int main() {
    auto flags = 0x1 | 0x2;

    if (flags & 0x3) {
        cout << "El OR ha funcionado\n";
    }
    CLEngine* device = new CLEngine(1280, 720, "Beast Brawl");

    try {
        //-------------------Resource manager-------------------
        CLResourceManager* resourceManager = CLResourceManager::GetResourceManager();
        auto resourceShader = resourceManager->GetResourceShader("CLEngine/src/Shaders/shadowMappingShader.vert", "CLEngine/src/Shaders/shadowMappingShader.frag");
        auto resourceShaderCartoon = resourceManager->GetResourceShader("CLEngine/src/Shaders/cartoonShader.vert", "CLEngine/src/Shaders/cartoonShader.frag");
        auto resourceShaderLightMapping = resourceManager->GetResourceShader("CLEngine/src/Shaders/lightMapping.vert", "CLEngine/src/Shaders/lightMapping.frag");
        auto resourceShaderHud = resourceManager->GetResourceShader("CLEngine/src/Shaders/spriteShader.vert", "CLEngine/src/Shaders/spriteShader.frag");

        auto resourceShaderMaterial = resourceManager->GetResourceShader("CLEngine/src/Shaders/materialShader.vert", "CLEngine/src/Shaders/materialShader.frag");
        auto resourceShader3 = resourceManager->GetResourceShader("CLEngine/src/Shaders/debugShader.vert", "CLEngine/src/Shaders/debugShader.frag");
        auto resourceShaderSkybox = resourceManager->GetResourceShader("CLEngine/src/Shaders/skybox.vert", "CLEngine/src/Shaders/skybox.frag");
        auto resourceMeshGround = resourceManager->GetResourceMesh("media/training_ground.obj", true);
        auto resourceMeshTotem = resourceManager->GetResourceMesh("media/totem.obj", true);
        auto resourceMesh = resourceManager->GetResourceMesh("media/kart_physics.obj", true);
        auto resourceMeshBox = resourceManager->GetResourceMesh("media/TEST_BOX.obj", true);

        auto resourceMeshCubeAnim1 = resourceManager->GetResourceMesh("media/animations/cube/001cube.obj", true);
        auto resourceMeshCubeAnim2 = resourceManager->GetResourceMesh("media/animations/cube/002cube.obj", true);
        auto resourceMeshCubeAnim3 = resourceManager->GetResourceMesh("media/animations/cube/003cube.obj", true);
        auto resourceMeshCubeAnim4 = resourceManager->GetResourceMesh("media/animations/cube/004cube.obj", true);

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
        static_cast<CLPointLight*>(light1->GetEntity())->SetLightAttributes(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1, 0.1f), 1.0f, 0.00005f, 0.0000014f);

        auto light2 = smgr->AddPointLight(123451);
        light2->SetShaderProgramID(resourceShaderCartoon->GetProgramID());
        static_cast<CLPointLight*>(light2->GetEntity())->SetLightAttributes(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1, 0.1f), 1.0f, 0.00007f, 0.00008f);

        auto meshes = smgr->AddGroup(10000);

        auto mesh1 = smgr->AddMesh(2);
        mesh1->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

        auto camera = smgr->AddCamera(3);
        camera->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

        auto mesh2 = smgr->AddMesh(4);
        mesh2->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

        auto nodeCubeAnim = smgr->AddMesh(5);
        nodeCubeAnim->SetShaderProgramID(resourceShaderCartoon->GetProgramID());

        // //Lo paso de momento todo a pillon, luego pongo pasar los valores por el metodo
        // int nParticles = 50;
        // glm::vec3 velocity = glm::vec3(0.0f,50.0f,0.0f);
        // string texture = "media/particle_test.png";
        // int width = 10;
        // int height= 10;
        // int spawnDelay = 100;
        // int particlesToSpawn = 2;
        // int lifeSpan = 2000;
        // glm::vec3 offset = glm::vec3(50.0f,50.0f,50.0f);
        // glm::vec3 orientation = glm::vec3(1.0f,1.0f,0.0f);
        // std::uint_fast8_t flags = EFFECT_FADING;

        auto ps1 = mesh2->AddParticleSystem(123940, 30, glm::vec3(500.0f, 500.0f, 500.0f), "media/particle_test.png", 10, 10, 100, 30, 250, EFFECT_DIR_ALEATORITY | EFFECT_FADING);

        static_cast<CLCamera*>(camera->GetEntity())->SetCameraTarget(mesh2->GetTranslation());

        smgr->AddGrass(300.0, 200.0, glm::vec3(140.0f, 55.0f, -50.0f), glm::vec3(20.0, 20.0, 20.0), false);
        smgr->AddGrass(100.0, 100.0, glm::vec3(140.0f, 55.0f, -300.0f), glm::vec3(10.0, 10.0, 10.0), true);

        string fileBillBoard = "media/mrPinguin.png";
        mesh2->AddBillBoard(2468, fileBillBoard, false, 100.0, 50.0);

        smgr->AddSkybox("media/skybox/right.jpg",
                        "media/skybox/left.jpg",
                        "media/skybox/top.jpg",
                        "media/skybox/bottom.jpg",
                        "media/skybox/front.jpg",
                        "media/skybox/back.jpg");

        smgr->AddShadowMapping(light2->GetEntity()->GetID());

        static_cast<CLMesh*>(mesh1->GetEntity())->SetMesh(resourceMeshGround);
        static_cast<CLMesh*>(mesh2->GetEntity())->SetMesh(resourceMesh);
        static_cast<CLMesh*>(nodeCubeAnim->GetEntity())->SetMesh(resourceMeshCubeAnim3);
        // static_cast<CLMesh*>(mesh7->GetEntity())->SetMesh(resourceMeshCochesito);

        camera->SetTranslation(glm::vec3(160.0f, 92.42f, -60.9f));
        light1->SetTranslation(glm::vec3(75.9f, 1000.2f, 15.08f));
        light2->SetTranslation(glm::vec3(295.9f, 300.2f, 15.08f));

        mesh1->SetScalation(glm::vec3(1.0f, 1.0f, 1.0f));
        mesh1->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        mesh1->SetTranslation(glm::vec3(50.0f, 50.0f, 50.0f));

        mesh2->SetScalation(glm::vec3(3.5f, 3.5f, 3.5f));
        mesh2->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
        mesh2->SetTranslation(glm::vec3(50.0f, 80.0f, -50.0f));

        nodeCubeAnim->SetScalation(glm::vec3(1.0f, 1.0f, 1.0f));
        nodeCubeAnim->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        nodeCubeAnim->SetTranslation(glm::vec3(150.0f, 80.0f, -50.0f));

        ps1->SetTranslation(glm::vec3(mesh2->GetTranslation().x, mesh2->GetTranslation().y + 80, mesh2->GetTranslation().z));
        ps1->SetScalation(glm::vec3(10.0f, 10.0f, 10.0f));
        // mesh7->SetScalation(glm::vec3(10.5f, 10.5f, 10.5f));
        // mesh7->SetTranslation(glm::vec3(140.0f,100.0f,-50.0f));
        // mesh7->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));

        //LUCES Y COLORES
        float auxCameraPos[3] = {camera->GetTranslation().x, camera->GetTranslation().y, camera->GetTranslation().z};
        float auxLightPos[3] = {light1->GetTranslation().x, light1->GetTranslation().y, light1->GetTranslation().z};
        float auxLightPos2[3] = {light2->GetTranslation().x, light2->GetTranslation().y, light2->GetTranslation().z};

        float index = 0.01;

        double previousTime = glfwGetTime();
        int frameCount = 0;

        const uint64_t ANIMATION_DURATION = 5000;
        auto start = Utils::getMillisSinceEpoch();

        CLNode* light3 = nullptr;
        while (device->Run()) {
            //Apartir de aqui hacemos cosas, de momento en el main para testear
            device->UpdateViewport();  //Por si reescalamos la ventana
            device->BeginScene();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Modifica ilumnacion");
            ImGui::SliderFloat3("CameraPos", auxCameraPos, -600, 600);
            ImGui::SliderFloat3("LightPos", auxLightPos, -1000, 1000);
            ImGui::SliderFloat3("LightPos2", auxLightPos2, -1000, 1000);
            ImGui::End();

            glm::vec3 cameraPos(auxCameraPos[0], auxCameraPos[1], auxCameraPos[2]);
            glm::vec3 lightPos(auxLightPos[0], auxLightPos[1], auxLightPos[2]);
            glm::vec3 lightPos2(auxLightPos2[0], auxLightPos2[1], auxLightPos2[2]);

            camera->SetTranslation(cameraPos);
            light1->SetTranslation(lightPos);
            light2->SetTranslation(lightPos2);

            static_cast<CLCamera*>(camera->GetEntity())->SetCameraTarget(nodeCubeAnim->GetGlobalTranslation());

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_E)) {
                static_cast<CLParticleSystem*>(ps1->GetEntity())->SetLoop(true);
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_Q)) {
                static_cast<CLParticleSystem*>(ps1->GetEntity())->SetLoop(false);
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_W)) {
                static_cast<CLParticleSystem*>(ps1->GetEntity())->Start();
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_R)) {
                static_cast<CLParticleSystem*>(ps1->GetEntity())->StartOneIteration();
                // cout << "Realizamos una iteracion\n";
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_LEFT)) {
                mesh2->SetTranslation(mesh2->GetTranslation() + glm::vec3(-1.0f, -1.0f, -1.0f));
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_RIGHT)) {
                mesh2->SetTranslation(mesh2->GetTranslation() + glm::vec3(1.0f, 1.0f, 1.0f));
            }

            if (glfwGetKey(device->GetWindow(), GLFW_KEY_K)) {
                mesh2->SetVisible(false);
            }

            // Measure speed
            double currentTime = glfwGetTime();
            frameCount++;
            // If a second has passed.
            if (currentTime - previousTime >= 1.0) {
                // Display the frame count here any way you want.
                device->SetTitle("BeastBrawl <" + to_string(frameCount) + ">");

                frameCount = 0;
                previousTime = currentTime;
            }

            device->DrawObjects();

            string file = "media/logo_clover.png";
            device->DrawImage2D(10.0f, 10.0f, 200.0f, 200.0f, 0.2f, file, true);

            //TEXTO -----------------
            string cadena = "Demo tecnica CLEngine";
            glm::vec3 vect3 = glm::vec3(1.0f, 0.8f, 0.2f);
            device->RenderText2D(cadena, 25.0f, 25.0f, 0.05f, 1.0f, vect3);

            // animaciones
            auto now = Utils::getMillisSinceEpoch();
            uint64_t timeElapsed = now - start;
            if (timeElapsed < ANIMATION_DURATION) {
                double percentTick = std::min(1.0f, (static_cast<float>(timeElapsed) / ANIMATION_DURATION));

                for (size_t i = 0; i < 3000; i++) {
                    for (size_t idxMesh = 0; idxMesh < resourceMeshCubeAnim1->GetvectorMesh().size(); idxMesh++) {
                        auto& prevMesh = resourceMeshCubeAnim1->GetvectorMeshPtr()->at(idxMesh);
                        auto& nextMesh = resourceMeshCubeAnim2->GetvectorMeshPtr()->at(idxMesh);
                        auto& currMesh = resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh);
                        // cout << "Las mallas " << idxMesh << " tienen " << prevMesh.vertices.size() << ", " << currMesh.vertices.size() << " y " << nextMesh.vertices.size() << " vértices respectivamente" << endl;
                        for (size_t idxVertex = 0; idxVertex < prevMesh.vertices.size(); idxVertex++) {
                            auto& prevVertex = prevMesh.vertices[idxVertex];
                            auto& nextVertex = nextMesh.vertices[idxVertex];
                            auto& currVertex = currMesh.vertices[idxVertex];

                            currVertex.position = mix(prevVertex.position, nextVertex.position, percentTick);
                            currVertex.normal = mix(prevVertex.normal, nextVertex.normal, percentTick);
                            currVertex.animationOffsetPos = currVertex.position - prevVertex.position;
                            currVertex.animationOffsetNormal = currVertex.normal - prevVertex.normal;

                            // cout << "Tenemos un vértice en " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl;
                            // cout << "El next un vértice es " << nextVertex.position.x << "," << nextVertex.position.y << "," << nextVertex.position.z << endl;
                            if (idxMesh == 0 && idxVertex == 0) {
                                // cout << "El timeElapsed es " << timeElapsed << " y el percentTick es " << percentTick << ". La nueva pos es " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl;
                                // cout << "La pos variable local es " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl
                                //     << "La pos variable miembro es " << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.x
                                //     << "," << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.y
                                //     << "," << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.z << endl << endl;
                            }
                        }
                    }
                }
            }

            device->InputClose();
            device->PollEvents();
            device->RenderImgui();
            device->EndScene();
            index += 0.2;
        }

        delete device;

    } catch (std::exception& ex) {
        cout << "Hubo una excepción " << ex.what() << endl;
    }

    return 0;
}