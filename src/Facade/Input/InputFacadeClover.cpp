#include "InputFacadeClover.h"

#include <Components/CId.h>
#include <Components/CNavMesh.h>

#include <Game.h>


#include <iostream>

InputFacadeClover::InputFacadeClover(){

    cout << "Creado el input de Clover\n"; 
    auto renderFacade = static_cast<RenderFacadeClover*>(RenderFacadeManager::GetInstance()->GetRenderFacade());
    device = renderFacade->GetDevice();
    
    if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1)){
        string name = glfwGetGamepadName(GLFW_JOYSTICK_1);
        cout << "Mando conectado: ";
        cout << name << endl;
        int count;
        glfwGetJoystickAxes(GLFW_JOYSTICK_1,&count);
        cout << "Numero de AXES: " << count << endl;

    }

}


void InputFacadeClover::CheckInputIntro(){

    
    
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //cout << state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] << endl;
    //SALIR
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();

        deletePress = true;
        device->CloseWindow();
    }

    //ENTRAR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();

        spacePress = true;
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }
}

void InputFacadeClover::CheckInputController() {

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //ATRAS
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();

        deletePress = true;
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }
}

void InputFacadeClover::CheckInputMenu(int& input, int maxInput){

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //ATRAS
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();
        // deletePress = true;
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INTRO});
    }

    //SUBIR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_UP ) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();

        upPress = true;
        input--;
        if(input < 0)
            input = maxInput;
    }

    //BAJAR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_DOWN ) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay ) {
        timeStart = system_clock::now();

        downPress = true;
        input++;
        if(input > maxInput) {
            input = 0;
        }
    }

    //ESPACIO
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay ) {
        timeStart = system_clock::now();

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
                //EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SELECT_CHARACTER});
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

void InputFacadeClover::CheckInputSelectCharacter(int &, int) {
    cout << "JEJE" << endl;
}


vector<Constants::InputTypes> InputFacadeClover::CheckInput(){
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

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
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_P) || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]) {
        eventManager.AddEventMulti(Event{EventType::PRESS_P});
        inputs.push_back(Constants::InputTypes::CLAXON);
    }

    //DERRAPE
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_U) || state.buttons[GLFW_GAMEPAD_BUTTON_X]) {
        eventManager.AddEventMulti(Event{EventType::PRESS_SKID});
        inputs.push_back(Constants::InputTypes::DRIFT);
    } else {
        eventManager.AddEventMulti(Event{EventType::NOT_SKID_PRESS});
    }
    
    //  ACELERAR y MARCHA ATRAS
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_W) || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0) {
        eventManager.AddEventMulti(Event{EventType::PRESS_I});
        inputs.push_back(Constants::InputTypes::FORWARD);
    } else if (glfwGetKey(device->GetWindow(),GLFW_KEY_S) || state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0) {
        eventManager.AddEventMulti(Event{EventType::PRESS_O});
        inputs.push_back(Constants::InputTypes::BACK);
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_I_O_PRESS});
    }

    // IZQUIERDA y DERECHA
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_D) || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.5) {
        eventManager.AddEventMulti(Event{EventType::PRESS_D});
        inputs.push_back(Constants::InputTypes::RIGHT);
    } else if (glfwGetKey(device->GetWindow(),GLFW_KEY_A) || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.5) {
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
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]) && !invertedCam && !totemCamActive) {
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});
        invertedCam = true;
    } else if((glfwGetKey(device->GetWindow(),GLFW_KEY_O) || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelayCamera) {
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});
        totemCamActive = !totemCamActive;
    } else if (!(glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]) && !totemCamActive){
        invertedCam = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});
    }

    // POWERUPS
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) {
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});
        inputs.push_back(Constants::InputTypes::LAUNCH_PU);
    }

    //PAUSE
    if (glfwGetKey(device->GetWindow(), GLFW_KEY_ESCAPE) || state.buttons[GLFW_GAMEPAD_BUTTON_START]) {
        eventManager.AddEventMulti(Event{EventType::STATE_PAUSE});
        scapePress = true;
    }
    return inputs;
}



void InputFacadeClover::CheckInputPause(int& input, int maxInput){

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //SUBIR
    // int estado = glfwGetKey(device->GetWindow(), GLFW_KEY_UP);
    // if (estado == GLFW_RELEASE) {
    //     upPress = false;
    // }
    //SUBIR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_UP ) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();

        upPress = true;
        input--;
        if(input < 0)
            input = maxInput;
    }

    //BAJAR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_DOWN ) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay ) {
        timeStart = system_clock::now();

        downPress = true;
        input++;
        if(input > maxInput) {
            input = 0;
        }
    }

    //IN GAME
    if (glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A] && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();

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
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);


    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();

        spacePress = true;
        RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
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