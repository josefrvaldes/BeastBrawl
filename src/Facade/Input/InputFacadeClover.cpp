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


/**
 * Comprueba si el input que le pasamos estaba pulsado
 * @param input - Input a comprobar.
 */
bool InputFacadeClover::IsInputPressed(InputXBox input){
    auto mapByType = inputsPressed.find(input);
    if (mapByType != inputsPressed.end()) {
        return mapByType->second;
    }else{
        cout << "no encuentra el boton" << "\n";
    }
    return true; // en caso de que no exista
}

/**
 * Cambia el estado del input que se le pasa
 * @param input - Input a buscar
 * @param valuePressed - Valor a actualizar
 */
void InputFacadeClover::SetValueInput(InputXBox input, bool valuePressed){
    auto mapByType = inputsPressed.find(input);
    if (mapByType != inputsPressed.end()) {
        mapByType->second = valuePressed;
    }
}

/**
 * Comprueba si se ha pulsado una tecla o boton del teclado
 * @param key - Input de teclado
 * @param button - Input de mando
 */
bool InputFacadeClover::IsKeyOrGamepadPress(int key, int button, bool axes, float axe) {
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
    if(!axes)
        return ( glfwGetKey(device->GetWindow(), key) || state.buttons[button] ) ;
    if(axe > 0)
        return ( glfwGetKey(device->GetWindow(), key) || state.axes[button] > axe);
    return ( glfwGetKey(device->GetWindow(), key) || state.axes[button] < axe);
}

/**
 * Comprueba si se ha pasado el tiempo de delay desde la ultima vez que se pulso el boton.
 */
bool InputFacadeClover::HasDelayPassed() {
    return duration_cast<milliseconds>(system_clock::now() - timeStart).count() > inputDelay;
}

//////////////////////
// CHECKS DE MENUS  //
//////////////////////


/**
 * Inputs de la pantalla de inicio.
 */
void InputFacadeClover::CheckInputIntro(){

    //SALIR
    if( IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_BACK, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_BACK) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_BACK, true);
        device->CloseWindow();

    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_BACK, false, 0) ) {
        SetValueInput(BUTTON_BACK, false);
    }

    //ENTRAR
    if ( IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_A) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) ) {
        SetValueInput(BUTTON_A, false);
    }
}

/**
 * Inputs del menu de controles.
 */
void InputFacadeClover::CheckInputController() {

    //ATRAS
    if( IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_B) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if( !IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0)) {
        SetValueInput(BUTTON_B, false);
    }
}

/**
 * Inputs del menu.
 * @param input - Seleccion.
 * @param maxInput - ID maximo de seleccion.
 */
void InputFacadeClover::CheckInputMenu(int& input, int maxInput){

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    //ATRAS
    if(IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_B)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INTRO});

    } else if(!IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0)) {
        SetValueInput(BUTTON_B, false);
    }

    //SUBIR
    if ( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5) && HasDelayPassed() ) {

        timeStart = system_clock::now();
        input--;
        if(input < 0) {
            input = maxInput;
        }
    }

    //BAJAR
    if (IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5) && HasDelayPassed() ) {

        timeStart = system_clock::now();
        input++;
        if(input > maxInput) {
            input = 0;
        }
    }

    //ESPACIO
    if (IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_A)) {

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
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_CREDITS});
                break;
            }
            case 4: {
                device->CloseWindow();
                break;
            }
            default : cout << "ESTE CODIGO DE INPUT NO EXISTE\n";
        }

    } else if (!IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) ) {
        SetValueInput(BUTTON_A, false);
    }
}

/**
 * Input de la pantalla de seleccion de personaje.
 * @param input - Seleccion.
 * @param maxInput - ID maximo de seleccion.
 */
void InputFacadeClover::CheckInputSelectCharacter(int &input, int maxInput) {

    //ATRAS
    if( IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_B)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if(!IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0)) {
        SetValueInput(BUTTON_B, false);
    }

    //IZQUIERDA Y DERECHA
    if ( (IsKeyOrGamepadPress(GLFW_KEY_RIGHT, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5) || IsKeyOrGamepadPress(GLFW_KEY_LEFT, GLFW_GAMEPAD_AXIS_LEFT_X, true, -0.5) ) && HasDelayPassed() ) {

        timeStart = system_clock::now();
        if (input%2 == 0) {
            ++input;
        } else {
            --input;
        }
    }

    //BAJAR
    if( IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5) && HasDelayPassed() ) {

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
    if( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5) && HasDelayPassed() ) {

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
    if ( IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_A) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_GAME_OPTIONS});

    } else if (!IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) ) {
        SetValueInput(BUTTON_A, false);
    }

}

/**
 * Inputs de la pantalla de opciones de partida
 * @param input - Vector con las opciones. Sus valores son el valor seleccionado
 * @param maxInput - Vector con las opciones. Sus valores son el maximo valor que puede alcanzar cada uno.
 * @param pos - En que opcion nos encontramos.
 * 0 = Tiempo partida, 1 = Tiempo posesion, 2 = Aceptar
 */
void InputFacadeClover::CheckInputGameOptions(std::vector<int> &input, int maxInput[], int& pos) {

    //ATRAS
    if( IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_B)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SELECT_CHARACTER});

    } else if( !IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0)) {
        SetValueInput(BUTTON_B, false);
    }

    //ACEPTAR - ESPACIO
    if ( pos == 2 && IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);

        //Manera un poco cutre de resetear el CId al empezar el juego
        RenderFacadeManager::GetInstance()->GetRenderFacade()->SetNumEnemyCars(0);

        auto cId = make_shared<CId>();
        cId->ResetNumIds();
        auto cNavMesh = make_shared<CNavMesh>();
        cNavMesh->ResetNumIds();
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMESINGLE});

    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) ) {
        SetValueInput(BUTTON_A, false);
    }

    //DERECHA
    if (IsKeyOrGamepadPress(GLFW_KEY_RIGHT, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5) && HasDelayPassed() ) {

        timeStart = system_clock::now();
        ++input[pos];
        if(input[pos] > maxInput[pos]) {
            input[pos] = 0;
        }
    }

    //IZQUIERDA
    if ( IsKeyOrGamepadPress(GLFW_KEY_LEFT, GLFW_GAMEPAD_AXIS_LEFT_X, true, -0.5) && HasDelayPassed()) {

        timeStart = system_clock::now();
        --input[pos];
        if(input[pos] < 0) {
            input[pos] = maxInput[pos];
        }
    }

    //ARRIBA
    if( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5) && HasDelayPassed() ) {

        timeStart = system_clock::now();
        --pos;
        if(pos < 0) {
            pos = input.size()-1;
        }
    }

    //BAJAR
    if( IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5) && HasDelayPassed() ) {

        timeStart = system_clock::now();
        ++pos;
        if(pos > (input.size()-1)) {
            pos = 0;
        }
    }

    if(pos != 2) {
        input[2] = 1;
    } else {
        input[2] = 0;
    }

}




/**
 * Input de la partida un jugador.
 */
void InputFacadeClover::CheckInputSingle(){
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    EventManager &eventManager = EventManager::GetInstance();
    auto renderFacade = RenderFacadeManager::GetInstance()->GetRenderFacade();


    //CLAXON
    if ( glfwGetKey(device->GetWindow(),GLFW_KEY_P) || state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] ) {
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

/**
 * Inputs de la partida multijugador
 */
vector<Constants::InputTypes> InputFacadeClover::CheckInputMulti(){

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

    EventManager &eventManager = EventManager::GetInstance();
    auto renderFacade = RenderFacadeManager::GetInstance()->GetRenderFacade();

    vector<Constants::InputTypes> inputs;
    /**
     * Para evitar el funcionamiento de cómo se redimensiona por defecto un vector,
     * como sabemos que como máximo va a haber un máximo de 4 inputs en el vector,
     * le reservamos directamente ya el espacio
     */
    inputs.reserve(4);

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

    //SUBIR
    if ( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5) && HasDelayPassed() ) {

        timeStart = system_clock::now();
        input--;
        if(input < 0)
            input = maxInput;
    }

    //BAJAR
    if ( IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5) && HasDelayPassed() ) {

        timeStart = system_clock::now();
        input++;
        if(input > maxInput)
            input = 0;
    }

    //IN GAME
    if ( IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_A)) {

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

    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) ) {
        SetValueInput(BUTTON_A, false);
    }
}

void InputFacadeClover::CheckInputEndRace(){

    if ( IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if( !IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) ) {
        SetValueInput(BUTTON_A, false);
    }
}

void InputFacadeClover::CheckInputLobbyMulti() {
    //TODO: Input temporal. Hay que poner el atras, no salir
    if ( IsKeyOrGamepadPress(GLFW_KEY_DELETE, GLFW_GAMEPAD_BUTTON_B, false, 0)) {
        device->CloseWindow();
    }
}

void InputFacadeClover::CheckInputCredits() {

    //ATRAS
    if( IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) && HasDelayPassed() && !IsInputPressed(BUTTON_A) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if( !IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0) ) {
        SetValueInput(BUTTON_A, false);
    }
}

void InputFacadeClover::CheckInputSettings(std::vector<int> &inputs, int *maxInputs, int &option) {
    cout << "HOLI JEJE" << endl;
}