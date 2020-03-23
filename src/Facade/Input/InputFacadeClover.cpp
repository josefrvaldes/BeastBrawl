#include "InputFacadeClover.h"

#include <Components/CId.h>
#include <Components/CNavMesh.h>

#include <Game.h>


#include <iostream>

InputFacadeClover::InputFacadeClover(){

    cout << "Creado el input de Clover\n"; 
    auto renderFacade = static_cast<RenderFacadeClover*>(RenderFacadeManager::GetInstance()->GetRenderFacade());
    device = renderFacade->GetDevice();
    

}


void InputFacadeClover::CheckInputIntro(){

    //SALIR
    int estado = glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE);
    if(estado == GLFW_RELEASE){
        deletePress = false;
    }
    if(!deletePress && estado == GLFW_PRESS) {
        deletePress = true;
        device->CloseWindow();
    }

    //ENTRAR
    estado = glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE);
    if (estado == GLFW_RELEASE) {
        spacePress = false;
    }
    if (!spacePress && estado == GLFW_PRESS) {
        spacePress = true;
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }
}

void InputFacadeClover::CheckInputController() {
    //ATRAS
    int estado = glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE);
    if(estado == GLFW_RELEASE){
        deletePress = false;
    }
    if(!deletePress && estado == GLFW_PRESS) {
        deletePress = true;
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }
}

void InputFacadeClover::CheckInputMenu(int& input, int maxInput){

    //ATRAS
    int estado = glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE);
    if(estado == GLFW_RELEASE){
        deletePress = false;
    }
    if(!deletePress && estado == GLFW_PRESS) {
        deletePress = true;
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INTRO});
    }

    //SUBIR
    estado = glfwGetKey(device->GetWindow(), GLFW_KEY_UP);
    if (estado == GLFW_RELEASE) {
        upPress = false;
    }
    if (!upPress && estado == GLFW_PRESS) {
        upPress = true;
        input--;
        if(input < 0)
            input = maxInput;
    }

    //BAJAR
    estado = glfwGetKey(device->GetWindow(), GLFW_KEY_DOWN);
    if (estado == GLFW_RELEASE) {
        downPress = false;
    }
    if (!downPress && estado == GLFW_PRESS) {
        downPress = true;
        input++;
        if(input > maxInput) {
            input = 0;
        }
    }

    //ESPACIO
    estado = glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE);
    if (estado == GLFW_RELEASE) {
        spacePress = false;
    }
    if (!spacePress && estado == GLFW_PRESS) {
        spacePress = true;

        switch (input) {
            case 0: {
                RenderFacadeManager::GetInstance()->GetRenderFacade()->SetNumEnemyCars(0);

                //Manera un poco cutre de resetear el CId al empezar el juego
                auto cId = make_shared<CId>();
                cId->ResetNumIds();
                auto cNavMesh = make_shared<CNavMesh>();
                cNavMesh->ResetNumIds();

                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMESINGLE});
                break;
            }
            case 1: {
                RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_LOBBYMULTI});
                break;
            }
            case 2: {
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_CONTROLS});
                break;
            }
            case 3: {
                device->CloseWindow();
                break;
            }
        }
    }
}


vector<Constants::InputTypes> InputFacadeClover::CheckInput(){
    EventManager &eventManager = EventManager::GetInstance();
    auto renderFacade = RenderFacadeManager::GetInstance()->GetRenderFacade();

    vector<Constants::InputTypes> inputs;
    inputs.reserve(4); // para evitar el funcionamiento de cómo se redimensiona
                       // por defecto un vector, como sabemos que como máximo 
                       // va a haber un máximo de 4 inputs en el vector, 
                       // le reservamos directamente ya el espacio

    //SALIMOS
    /*if (glfwGetKey(device->GetWindow(),GLFW_KEY_DELETE)) {
        device->CloseWindow();
    }*/

    //CLAXON
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_P)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_P});
        inputs.push_back(Constants::InputTypes::CLAXON);
    }

    //DERRAPE
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_U)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_SKID});
    } else {
        eventManager.AddEventMulti(Event{EventType::NOT_SKID_PRESS});
    }
    
    //  ACELERAR y MARCHA ATRAS
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_W)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_I});
        inputs.push_back(Constants::InputTypes::FORWARD);
    } else if (glfwGetKey(device->GetWindow(),GLFW_KEY_S)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_O});
        inputs.push_back(Constants::InputTypes::BACK);
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_I_O_PRESS});
    }

    // IZQUIERDA y DERECHA
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_D)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_D});
        inputs.push_back(Constants::InputTypes::RIGHT);
    } else if (glfwGetKey(device->GetWindow(),GLFW_KEY_A)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_A});
        inputs.push_back(Constants::InputTypes::LEFT);
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_A_D_PRESS});
    }

    // MODO DEBUG
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_F3) && !glfwGetKey(device->GetWindow(),GLFW_KEY_LEFT_SHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();
        //showDebug = !showDebug; 
        renderFacade->SetShowDebug(!renderFacade->GetShowDebug());

    }else if(glfwGetKey(device->GetWindow(),GLFW_KEY_F3) && glfwGetKey(device->GetWindow(),GLFW_KEY_LEFT_SHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        renderFacade->SetShowDebugAI(!renderFacade->GetShowDebugAI());
        
    }

    //TODO: Alargar esto para cuando tengamos mas coches para debugear
    // Seleccion de coche para debugear
    if(glfwGetKey(device->GetWindow(),GLFW_KEY_1) && glfwGetKey(device->GetWindow(),GLFW_KEY_LEFT_SHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        renderFacade->SetIDCarAIToDebug(0);

        idCarAIToDebug = 0;
    }else if(glfwGetKey(device->GetWindow(),GLFW_KEY_2) && glfwGetKey(device->GetWindow(),GLFW_KEY_LEFT_SHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        renderFacade->SetIDCarAIToDebug(1);
        
    }else if(glfwGetKey(device->GetWindow(),GLFW_KEY_3) && glfwGetKey(device->GetWindow(),GLFW_KEY_LEFT_SHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        renderFacade->SetIDCarAIToDebug(2);
        
    }else if(glfwGetKey(device->GetWindow(),GLFW_KEY_0) && glfwGetKey(device->GetWindow(),GLFW_KEY_LEFT_SHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        renderFacade->SetIDCarAIToDebug(-1);
        
    }

    // CAMARA TRASERA Y TOTEM
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_I) && !invertedCam && !totemCamActive) {
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});
        invertedCam = true;
    } else if(glfwGetKey(device->GetWindow(),GLFW_KEY_O) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelayCamera) {
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});
        totemCamActive = !totemCamActive;
    } else if (!glfwGetKey(device->GetWindow(),GLFW_KEY_I) && !totemCamActive){
        invertedCam = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});
    }

    // POWERUPS
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});
        inputs.push_back(Constants::InputTypes::LAUNCH_PU);
    }

    //PAUSE
    int estado = glfwGetKey(device->GetWindow(), GLFW_KEY_ESCAPE);
    if (estado == GLFW_RELEASE) {
        scapePress = false;
    }
    if (!scapePress && estado == GLFW_PRESS) {
        eventManager.AddEventMulti(Event{EventType::STATE_PAUSE});
        scapePress = true;
    }
    return inputs;
}



void InputFacadeClover::CheckInputPause(int& input, int maxInput){


    //SUBIR
    int estado = glfwGetKey(device->GetWindow(), GLFW_KEY_UP);
    if (estado == GLFW_RELEASE) {
        upPress = false;
    }
    if (!upPress && estado == GLFW_PRESS) {
        upPress = true;
        input--;
        if(input < 0)
            input = maxInput;
    }

    //BAJAR
    estado = glfwGetKey(device->GetWindow(), GLFW_KEY_DOWN);
    if (estado == GLFW_RELEASE) {
        downPress = false;
    }
    if (!downPress && estado == GLFW_PRESS) {
        downPress = true;
        input++;
        if(input > maxInput) {
            input = 0;
        }
    }

    //IN GAME
    estado = glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE);
    if (estado == GLFW_RELEASE) {
        scapePress = false;
    }
    if (!scapePress && estado == GLFW_PRESS) {
        scapePress = true;
        switch (input) {
            case 0: {
                Game::GetInstance()->SetState(State::INGAME_SINGLE);
                break;
            }
            case 1: {
                RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
                Game::GetInstance()->SetState(State::MENU);
                break;
            }
        }
    }
}


void InputFacadeClover::CheckInputEndRace(){
    int estado = glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE);
    if (estado == GLFW_RELEASE) {
        spacePress = false;
    }
    if (!spacePress && estado == GLFW_PRESS) {
        spacePress = true;
        RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
        //cout << "ENTRAAAAAA ENDRACE\n";
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } /*else if (glfwGetKey(device->GetWindow(),GLFW_KEY_DELETE)) {
        device->CloseWindow();
    }*/
}


void InputFacadeClover::CheckInputLobbyMulti() {
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_DELETE)) {
        device->CloseWindow();
    }
}