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

    for(int i = 0; i != InputXBox::END; ++i){
        inputsPressed.emplace(static_cast<InputXBox>(i), false);
    }

}


// mira si el input se encuentra pulsado
bool InputFacadeClover::IsInputPressed(InputXBox input){
    auto mapByType = inputsPressed.find(input);
    if (mapByType != inputsPressed.end()) {
        return mapByType->second;
    }else{
        cout << "no encuentra el boton" << "\n";
    }
    return true; // en caso de que no exista
}

void InputFacadeClover::SetValueInput(InputXBox input, bool valuePressed){
    auto mapByType = inputsPressed.find(input);
    if (mapByType != inputsPressed.end()) {
        mapByType->second = valuePressed;
    }
}

void InputFacadeClover::CheckInputIntro(){

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //SALIR
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_BACK])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count() > inputDelay
            && !IsInputPressed(BUTTON_BACK)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_BACK, true);
        device->CloseWindow();

    } else if (!(glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_BACK])) {
        SetValueInput(BUTTON_BACK, false);
    }

    //ENTRAR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay
            && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if (!(glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])) {
        SetValueInput(BUTTON_A, false);
    }
}

void InputFacadeClover::CheckInputController() {

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //ATRAS
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay
            && !IsInputPressed(BUTTON_B)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if((glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B])) {
        SetValueInput(BUTTON_B, false);
    }
}

void InputFacadeClover::CheckInputMenu(int& input, int maxInput){

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //ATRAS
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay
            && !IsInputPressed(BUTTON_B)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INTRO});

    } else if(!(glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B])) {
        SetValueInput(BUTTON_B, false);
    }

    //SUBIR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_UP ) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {

        timeStart = system_clock::now();
        input--;
        if(input < 0) {
            input = maxInput;
        }
    }

    //BAJAR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_DOWN ) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay ) {

        timeStart = system_clock::now();
        input++;
        if(input > maxInput) {
            input = 0;
        }
    }

    //ESPACIO
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay
            && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);

        switch (input) {
            case 0: {
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SELECT_CHARACTER});
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
            default : cout << "ESTE CODIGO DE INPUT NO EXISTE\n";
        }

    } else if (!(glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) ) {
        SetValueInput(BUTTON_A, false);
    }
}

void InputFacadeClover::CheckInputSelectCharacter(int &input, int maxInput) {

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //ATRAS
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B])
       && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay
       && !IsInputPressed(BUTTON_B)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if(!(glfwGetKey(device->GetWindow(), GLFW_KEY_DELETE) || state.buttons[GLFW_GAMEPAD_BUTTON_B])) {
        SetValueInput(BUTTON_B, false);
    }

    //IZQUIERDA Y DERECHA
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_RIGHT ) || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]
            || glfwGetKey(device->GetWindow(), GLFW_KEY_LEFT ) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB])
        && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {

        timeStart = system_clock::now();
        if (input%2 == 0) {
            ++input;
        } else {
            --input;
        }
    }

    //BAJAR
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_DOWN) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay ) {

        timeStart = system_clock::now();
        input += 2;
        if(input > maxInput) {
            if (input%2 == 0) {
                input = 0;
            } else {
                input = 1;
            }
        }
    }

    //SUBIR
    if((glfwGetKey(device->GetWindow(), GLFW_KEY_UP) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
       && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay ) {

        timeStart = system_clock::now();
        input -= 2;
        if(input < 0) {
            if (input%2 == 0) {
                input = maxInput-1;
            } else {
                input = maxInput;
            }
        }
    }


    //ACEPTAR - ESPACIO
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])
        && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay
        && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);

        RenderFacadeManager::GetInstance()->GetRenderFacade()->SetNumEnemyCars(0);

        //Manera un poco cutre de resetear el CId al empezar el juego
        auto cId = make_shared<CId>();
        cId->ResetNumIds();
        auto cNavMesh = make_shared<CNavMesh>();
        cNavMesh->ResetNumIds();
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMESINGLE});

    } else if (!(glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) ) {
        SetValueInput(BUTTON_A, false);
    }

}

void InputFacadeClover::CheckInputSingle(){
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    EventManager &eventManager = EventManager::GetInstance();
    auto renderFacade = RenderFacadeManager::GetInstance()->GetRenderFacade();


    //CLAXON
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_P) || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]) {
        eventManager.AddEventMulti(Event{EventType::PRESS_P});
    }

    //DERRAPE
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_U) || state.buttons[GLFW_GAMEPAD_BUTTON_X]) {
        eventManager.AddEventMulti(Event{EventType::PRESS_SKID});
    } else {
        eventManager.AddEventMulti(Event{EventType::NOT_SKID_PRESS});
    }
    
    //  ACELERAR y MARCHA ATRAS
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_W) || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0) {
        eventManager.AddEventMulti(Event{EventType::PRESS_I});
    } else if (glfwGetKey(device->GetWindow(),GLFW_KEY_S) || state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0) {
        eventManager.AddEventMulti(Event{EventType::PRESS_O});
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_I_O_PRESS});
    }

    // IZQUIERDA y DERECHA
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_D) || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.5) {
        eventManager.AddEventMulti(Event{EventType::PRESS_D});
    } else if (glfwGetKey(device->GetWindow(),GLFW_KEY_A) || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.5) {
        eventManager.AddEventMulti(Event{EventType::PRESS_A});
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
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
            && !invertedCam
            && !totemCamActive) {

        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});
        invertedCam = true;

    } else if((glfwGetKey(device->GetWindow(),GLFW_KEY_O) || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER])
            && !invertedCam
            && !totemCamActive) {

        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});
        totemCamActive = true;

    } else if (!(glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
            && !totemCamActive){

        invertedCam = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});

    } else if(!(glfwGetKey(device->GetWindow(),GLFW_KEY_O) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
            && !invertedCam) {

        totemCamActive = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});
    }

    // POWERUPS
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])
            && !IsInputPressed(BUTTON_A)) {

        SetValueInput(InputXBox::BUTTON_A, true);
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});

    }else if(!(glfwGetKey(device->GetWindow(),GLFW_KEY_ESCAPE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])){
        SetValueInput(InputXBox::BUTTON_A, false);
    }

    //PAUSE
    //No se vuelve del pause ya dandole otra vez, por lo que es inutil comprobar si lo has soltado
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_ESCAPE) || state.buttons[GLFW_GAMEPAD_BUTTON_START])) {
        eventManager.AddEventMulti(Event{EventType::STATE_PAUSE});
    }

}

vector<Constants::InputTypes> InputFacadeClover::CheckInputMulti(){
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
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
            && !invertedCam
            && !totemCamActive) {

        invertedCam = true;
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});

    } else if((glfwGetKey(device->GetWindow(),GLFW_KEY_O) || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER])
            && !invertedCam ) {

        totemCamActive = true;
        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});

    } else if (!(glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
            && !totemCamActive){

        invertedCam = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});

    } else if (!(glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
            && !invertedCam ) {

        totemCamActive = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});
    }

    // POWERUPS
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])
            && !IsInputPressed(BUTTON_A)) {

        SetValueInput(InputXBox::BUTTON_A, true);
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});
        inputs.push_back(Constants::InputTypes::LAUNCH_PU);

    }else if(!(glfwGetKey(device->GetWindow(),GLFW_KEY_ESCAPE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])){
        SetValueInput(InputXBox::BUTTON_A, false);
    }

    return inputs;
}


void InputFacadeClover::CheckInputPause(int& input, int maxInput){

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //SUBIR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_UP ) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count() > inputDelay) {

        timeStart = system_clock::now();
        input--;
        if(input < 0)
            input = maxInput;
    }

    //BAJAR
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_DOWN ) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count() > inputDelay ) {

        timeStart = system_clock::now();
        input++;
        if(input > maxInput)
            input = 0;
    }

    //IN GAME
    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count() > inputDelay
            && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);

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

    } else if ( !(glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) ) {
        SetValueInput(BUTTON_A, false);
    }
}


void InputFacadeClover::CheckInputEndRace(){
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);


    if ((glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])
            && duration_cast<milliseconds>(system_clock::now() - timeStart).count() > inputDelay
            && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if( !(glfwGetKey(device->GetWindow(), GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) ) {
        SetValueInput(BUTTON_A, false);
    }
}


void InputFacadeClover::CheckInputLobbyMulti() {
    //TODO: Input temporal. Hay que poner el atras, no salir
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_DELETE)) {
        device->CloseWindow();
    }
}