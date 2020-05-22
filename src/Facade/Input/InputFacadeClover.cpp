/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #include "InputFacadeClover.h"

#include <Components/CId.h>
#include <Components/CNavMesh.h>
#include <Systems/Utils.h>

#include <../../include/boost/asio.hpp>

#include <Game.h>
#include <iostream>

InputFacadeClover::InputFacadeClover(){

    //cout << "Creado el input de Clover\n"; 
    auto renderFacade = static_cast<RenderFacadeClover*>(RenderFacadeManager::GetInstance()->GetRenderFacade());
    device = renderFacade->GetDevice();

    device->DisableCursor();
    
    if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1)){
        string name = glfwGetGamepadName(GLFW_JOYSTICK_1);
        //cout << "Mando conectado: ";
        //cout << name << endl;
        int count;
        glfwGetJoystickAxes(GLFW_JOYSTICK_1,&count);
        //cout << "Numero de AXES: " << count << endl;

    }

    for(int i = 0; i != InputXBox::END; ++i){
        inputsPressed.emplace(static_cast<InputXBox>(i), false);
    }

}


/**
 * Comprueba si hay un mando conectado
 */
bool InputFacadeClover::IsConectedGamepad() {
    if (glfwJoystickPresent(GLFW_JOYSTICK_1) && glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
        return true;
    }
    return false;
}


/**
 * Comprueba si el input que le pasamos estaba pulsado
 * @param input - Input a comprobar.
 */
bool InputFacadeClover::IsInputPressed(InputXBox input){
    auto mapByType = inputsPressed.find(input);
    if (mapByType != inputsPressed.end()) {
        return mapByType->second;
    }//else{
       // cout << "no encuentra el boton" << "\n";
    //}
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
bool InputFacadeClover::IsKeyOrGamepadPress(int key, int button1, bool axes, float axe, int button2, bool doubleGamepad) {
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
    if(!axes)
        return ( glfwGetKey(device->GetWindow(), key) || state.buttons[button1] || (doubleGamepad && state.buttons[button2]) ) ;
    if(axe > 0)
        return ( glfwGetKey(device->GetWindow(), key) || state.axes[button1] > axe || (doubleGamepad && state.buttons[button2]));
    return ( glfwGetKey(device->GetWindow(), key) || state.axes[button1] < axe  || (doubleGamepad && state.buttons[button2]));
}

/**
 * Comprueba si se ha pulsado una tecla 
 * @param key - Input 
 */
bool InputFacadeClover::IsKeyPress(int key) {
    return ( glfwGetKey(device->GetWindow(), key)) ;
}

/**
 * Comprueba si se ha pulsado un boton del mando
 * @param key - Input 
 */
bool InputFacadeClover::IsGamepadPress(int key) {
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
    return (state.buttons[key]) ;
}

/**
 * Comprueba si se ha pulsado un joystick
 * @param key - Input 
 */
bool InputFacadeClover::IsGamepadAxisPress(int key, float axes) {
    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
    return ( state.axes[key] > axes ) ;
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
    if( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_BACK, false, 0, 0, false) && HasDelayPassed() && !IsInputPressed(BUTTON_BACK) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_BACK, true);
        device->CloseWindow();

    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_BACK, false, 0, 0, false) ) {
        SetValueInput(BUTTON_BACK, false);
    }

    //ENTRAR
    if ( (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || CheckAnyKey() || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) && HasDelayPassed() && !IsInputPressed(BUTTON_A) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if ( ! (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) ) {
        SetValueInput(BUTTON_A, false);
    }
}

/**
 * Inputs del menu de controles.
 */
void InputFacadeClover::CheckInputController() {

    //ATRAS
    if( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && HasDelayPassed() && !IsInputPressed(BUTTON_B) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if( !IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false)) {
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
    /*if(IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && HasDelayPassed() && !IsInputPressed(BUTTON_B)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INTRO});
        input = 0;

    } else if(!IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false)) {
        SetValueInput(BUTTON_B, false);
    }*/

    //SUBIR
    if ( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) && ( (IsInputPressed(BUTTON_STICK_UP) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_UP) ) ) {
        timeStart = system_clock::now();
        input--;
        if (input < 0) {
            input = maxInput;
        }
        SetValueInput(BUTTON_STICK_UP, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) ){
        SetValueInput(BUTTON_STICK_UP, false);
    }

    //BAJAR
    if (IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) && ( (IsInputPressed(BUTTON_STICK_DOWN) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_DOWN) ) ) {

        timeStart = system_clock::now();
        input++;
        if(input > maxInput) {
            input = 0;
        }
        SetValueInput(BUTTON_STICK_DOWN, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) ){
        SetValueInput(BUTTON_STICK_DOWN, false);
    }

    //ESPACIO
    if ((IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) && HasDelayPassed() && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OK});

        switch (input) {
            case 0: {
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SELECT_CHARACTER});
                multiplayer = false;
                tournamentMode = false;
                break;
            }
            case 1: {
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SELECT_CHARACTER});
                multiplayer = false;
                tournamentMode = true;
                break;
            }
            case 2: {
                //RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_LOBBYMULTI});
                multiplayer = true;
                tournamentMode = false;
                break;
            }
            case 3: {
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_CONTROLS});
                break;
            }
            case 4: {
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_CREDITS});
                break;
            }
            case 5: {
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SETTINGS});
                break;
            }
            case 6: {
                device->CloseWindow();
                break;
            }
            default : 
                break;
        }

    } else if (!(IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) ) {
        SetValueInput(BUTTON_A, false);
    }
}


void InputFacadeClover::CheckInputSelCharUpDown(int &input) {
    if ( ( IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) 
        || IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) )
        && ( (IsInputPressed(BUTTON_STICK_UP) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_UP) )
        && ( (IsInputPressed(BUTTON_STICK_DOWN) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_DOWN) )  ) {

        timeStart = system_clock::now();
        if (input%2 == 0) {
            ++input;
        } else {
            --input;
        }
        SetValueInput(BUTTON_STICK_UP, true);
        SetValueInput(BUTTON_STICK_DOWN, true);
        shared_ptr<DataMap> data = make_shared<DataMap>();
        int num = input;
        (*data)[NUM] = num;
        EventManager::GetInstance().AddEventMulti(Event{EventType::VRANDOM, data});
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !( IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true)
                   || IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) ) ) {
        SetValueInput(BUTTON_STICK_UP, false);
        SetValueInput(BUTTON_STICK_DOWN, false);
    }
}

void InputFacadeClover::CheckInputSelCharLeft(int &input, int maxInput) {
    if( IsKeyOrGamepadPress(GLFW_KEY_LEFT, GLFW_GAMEPAD_AXIS_LEFT_X, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, true)
    && ( (IsInputPressed(BUTTON_STICK_L) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_L) )) {

        timeStart = system_clock::now();
        input -= 2;
        if(input < 0) {
            if (input%2 == 0) {
                input = maxInput-1;
            } else {
                input = maxInput;
            }
        }
        SetValueInput(BUTTON_STICK_L, true);
        shared_ptr<DataMap> data = make_shared<DataMap>();
        int num = input;
        (*data)[NUM] = num;
        EventManager::GetInstance().AddEventMulti(Event{EventType::VRANDOM, data});
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_LEFT, GLFW_GAMEPAD_AXIS_LEFT_X, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, true) ) {
        SetValueInput(BUTTON_STICK_L, false);
    }
}

void InputFacadeClover::CheckInputSelCharRight(int &input, int maxInput) {
    if( IsKeyOrGamepadPress(GLFW_KEY_RIGHT, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, true) 
    && ( (IsInputPressed(BUTTON_STICK_R) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_R) )) {

        timeStart = system_clock::now();
        input += 2;
        if(input > maxInput) {
            if (input%2 == 0) {
                input = 0;
            } else {
                input = 1;
            }
        }
        SetValueInput(BUTTON_STICK_R, true);
        shared_ptr<DataMap> data = make_shared<DataMap>();
        int num = input;
        (*data)[NUM] = num;
        EventManager::GetInstance().AddEventMulti(Event{EventType::VRANDOM, data});
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_RIGHT, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, true)  ) {
        SetValueInput(BUTTON_STICK_R, false);
    }
}

void InputFacadeClover::CheckInputSelCharIntro(int &input) {
    if ( (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) && HasDelayPassed() && !IsInputPressed(BUTTON_A) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OK});

        //Actualiza la vez del personaje a usar
        shared_ptr<DataMap> data = make_shared<DataMap>();
        int num = input;
        (*data)[NUM] = num;
        EventManager::GetInstance().AddEventMulti(Event{EventType::UPDATE_SOUNDCHARACTER, data});
        
        //Registra el personaje a usar
        GameValues::GetInstance()->SetCharacter(input);

        //TODO: Ahora mismo, SELECCIONAR PERSONAJE y VOLVER A JUGAR del EndRace, hacen lo mismo. Falta la gestion online.
        if ( multiplayer ) {
            EventManager::GetInstance().AddEventMulti(Event{EventType::TCP_CHAR_REQUEST});
        } else{
            RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
            if(!tournamentMode)
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_GAME_OPTIONS});
            else
                EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_TOURNAMENT_OPTIONS});
        }

    } else if (!(IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) ) {
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
    if( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && HasDelayPassed() && !IsInputPressed(BUTTON_B)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if(!IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false)) {
        SetValueInput(BUTTON_B, false);
    }

    // SUBIR BAJAR
    CheckInputSelCharUpDown(input);

    // DERECHA
    CheckInputSelCharRight(input, maxInput);

    // IZQUIERDA
    CheckInputSelCharLeft(input, maxInput);

    //ACEPTAR - ESPACIO
    CheckInputSelCharIntro(input);

}



void InputFacadeClover::CheckInputUp(std::vector<int> &input, int& pos){
    if( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true)
    && ( (IsInputPressed(BUTTON_STICK_UP) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_UP) )) {
        timeStart = system_clock::now();
        --pos;
        if(pos < 0) {
            pos = input.size()-1;
        }
        SetValueInput(BUTTON_STICK_UP, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) ) {
        SetValueInput(BUTTON_STICK_UP, false);
    }
}

void InputFacadeClover::CheckInputDown(std::vector<int> &input, int& pos){
    if( IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true)
    && ( (IsInputPressed(BUTTON_STICK_DOWN) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_DOWN) )) {
        timeStart = system_clock::now();
        ++pos;
        if(pos > (static_cast<int>(input.size())-1)) {
            pos = 0;
        }
        SetValueInput(BUTTON_STICK_DOWN, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if( !IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) ) {
        SetValueInput(BUTTON_STICK_DOWN, false);
    }
}

void InputFacadeClover::CheckInputLeft(std::vector<int> &input, int maxInput[], int& pos){
    if ( IsKeyOrGamepadPress(GLFW_KEY_LEFT, GLFW_GAMEPAD_AXIS_LEFT_X, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, true)
        && ( (IsInputPressed(BUTTON_STICK_L) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_L) )) {
        timeStart = system_clock::now();
        --input[pos];
        if(input[pos] < 0) {
            input[pos] = maxInput[pos];
        }
        SetValueInput(BUTTON_STICK_L, true);

        ChangeGameOptions(pos, input[pos]);

        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_LEFT, GLFW_GAMEPAD_AXIS_LEFT_X, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, true) ) {
        SetValueInput(BUTTON_STICK_L, false);
    }
}

void InputFacadeClover::CheckInputRight(std::vector<int> &input, int maxInput[], int& pos){
    if (IsKeyOrGamepadPress(GLFW_KEY_RIGHT, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, true)
    && ( (IsInputPressed(BUTTON_STICK_R) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_R) )) {
        timeStart = system_clock::now();
        ++input[pos];
        if(input[pos] > maxInput[pos]) {
            input[pos] = 0;
        }
        //cout << "El maximo valor es: " << maxInput[pos] << " y el valor que le doy es: " << input[pos] << endl;
        SetValueInput(BUTTON_STICK_R, true);

        ChangeGameOptions(pos, input[pos]);

        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_RIGHT, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, true) ) {
        SetValueInput(BUTTON_STICK_R, false);
    }
}

//Para volver a la seleccion de personajes
void InputFacadeClover::CheckInputBack(){
    if( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && HasDelayPassed() && !IsInputPressed(BUTTON_B)) {
        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SELECT_CHARACTER});
        //pos = 2;

    } else if( !IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false)) {
        SetValueInput(BUTTON_B, false);
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
    CheckInputBack();

    //ACEPTAR - ESPACIO
    if ( (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_START)) && HasDelayPassed() && !IsInputPressed(BUTTON_START) && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_START, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OK});

        //Manera un poco cutre de resetear el CId al empezar el juego
        RenderFacadeManager::GetInstance()->GetRenderFacade()->SetNumEnemyCars(0);

        auto cId = make_shared<CId>();
        cId->ResetNumIds();
        auto cNavMesh = make_shared<CNavMesh>();
        cNavMesh->ResetNumIds();
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMESINGLE});

    } else if ( !(IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_START)) ) {
        SetValueInput(BUTTON_START, false);
        SetValueInput(BUTTON_A, false);
    }

    //DERECHA
    CheckInputRight(input, maxInput, pos);
    //IZQUIERDA
    CheckInputLeft(input, maxInput, pos);
    //ARRIBA
    CheckInputUp(input, pos);
    //BAJAR
    CheckInputDown(input, pos);

}

/**
 * Inputs de la pantalla de opciones de partida
 * @param input - Vector con las opciones. Sus valores son el valor seleccionado
 * @param maxInput - Vector con las opciones. Sus valores son el maximo valor que puede alcanzar cada uno.
 * @param pos - En que opcion nos encontramos.
 * 0 = Tiempo partida, 1 = Tiempo posesion, 2 = Aceptar
 */
void InputFacadeClover::CheckInputTournamentOptions(std::vector<int> &input, int maxInput[], int& pos) {

    //ATRAS
    CheckInputBack();

    //ACEPTAR - ESPACIO
    if ( (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_START)) && HasDelayPassed() && !IsInputPressed(BUTTON_START) && !IsInputPressed(BUTTON_A)) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_START, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OK});

        //Manera un poco cutre de resetear el CId al empezar el juego
        RenderFacadeManager::GetInstance()->GetRenderFacade()->SetNumEnemyCars(0);

        auto cId = make_shared<CId>();
        cId->ResetNumIds();
        auto cNavMesh = make_shared<CNavMesh>();
        cNavMesh->ResetNumIds();
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMESINGLE});

        // GameValues
        GameValues::GetInstance()->ResetCountBattles();
        GameValues::GetInstance()->ResetPoints();
        GameValues::GetInstance()->SetNumBattles(input[4]+3);  // la base es 0 que son 3 partidas

    } else if ( !(IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_START)) ) {
        SetValueInput(BUTTON_START, false);
        SetValueInput(BUTTON_A, false);
    }

    //DERECHA
    CheckInputRight(input, maxInput, pos);
    //IZQUIERDA
    CheckInputLeft(input, maxInput, pos);
    //ARRIBA
    CheckInputUp(input, pos);
    //BAJAR
    CheckInputDown(input, pos);

}




bool InputFacadeClover::CheckInputAnimationsStartEnd() {

    // si están pulsando espacio, enter o A(xbox)
    if (  IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A) ) {
        return true;
    }
    return false;
};


// QUITAR
bool InputFacadeClover::ShowHud(bool inputTable) {

    if( IsKeyPress(GLFW_KEY_F1) &&  !pressF1 ) {
        inputTable = !inputTable;
        pressF1 = true;

    } else if( !IsKeyPress(GLFW_KEY_F1)  ) {
        pressF1 = false;
    }

    return inputTable;
}

void InputFacadeClover::ShowTable(Constants::ShowTableMinimap &mode) {
    if ( (IsKeyPress(GLFW_KEY_R) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_B)) && !IsInputPressed(BUTTON_B) ) {
        switch (mode) {
            case Constants::ShowTableMinimap::NONE:         mode = Constants::ShowTableMinimap::BOTH;       break;
            case Constants::ShowTableMinimap::ONLYMAP:      mode = Constants::ShowTableMinimap::ONLYTABLE;  break;
            case Constants::ShowTableMinimap::ONLYTABLE:    mode = Constants::ShowTableMinimap::NONE;       break;
            case Constants::ShowTableMinimap::BOTH:         mode = Constants::ShowTableMinimap::ONLYMAP;    break;
            default:                                        mode = Constants::ShowTableMinimap::BOTH;       break;
        }
        SetValueInput(BUTTON_B, true);
    } else if ( !(IsKeyPress(GLFW_KEY_R) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_B)) ) {
        SetValueInput(BUTTON_B, false);
    }
}



/**
 * Input de la partida un jugador.
 * TODO: Igual habría que quitar el renderFacade y que eso sean eventos.
 * @return bool que indica si vamos a ir a pausa o no
 */
bool InputFacadeClover::CheckInputSingle(){
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
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_D) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.5) {
        eventManager.AddEventMulti(Event{EventType::PRESS_D});
    } else if (glfwGetKey(device->GetWindow(),GLFW_KEY_A) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.5) {
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
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])  // Camara trasera
            && !invertedCam
            && !totemCamActive) {

        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});
        invertedCam = true;

    } else if((glfwGetKey(device->GetWindow(),GLFW_KEY_O) || state.buttons[GLFW_GAMEPAD_BUTTON_Y])   //Camara totem
            && !invertedCam
            && !totemCamActive) {

        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});
        totemCamActive = true;

    } else if (!(glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
            && !totemCamActive){

        invertedCam = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});

    } else if(!(glfwGetKey(device->GetWindow(),GLFW_KEY_O) || state.buttons[GLFW_GAMEPAD_BUTTON_Y])
            && !invertedCam) {

        totemCamActive = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});
    }

    // POWERUPS
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) && !IsInputPressed(BUTTON_A)) {

        SetValueInput(InputXBox::BUTTON_A, true);
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});

    }else if(!(glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])){
        SetValueInput(InputXBox::BUTTON_A, false);
    }

    //PAUSE
    if ( IsKeyOrGamepadPress(GLFW_KEY_ESCAPE, GLFW_GAMEPAD_BUTTON_START, false, 0, 0, false) && !IsInputPressed(BUTTON_START)) {
        SetValueInput(InputXBox::BUTTON_START, true);
        eventManager.AddEventMulti(Event{EventType::STATE_PAUSE});
        return true;
    } else if (!IsKeyOrGamepadPress(GLFW_KEY_ESCAPE, GLFW_GAMEPAD_BUTTON_START, false, 0, 0, false)) {
        SetValueInput(InputXBox::BUTTON_START, false);
    }
    return false;
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
    if (glfwGetKey(device->GetWindow(),GLFW_KEY_D) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.5) {
        eventManager.AddEventMulti(Event{EventType::PRESS_D});
        inputs.push_back(Constants::InputTypes::RIGHT);
    } else if (glfwGetKey(device->GetWindow(),GLFW_KEY_A) || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] || state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.5) {
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
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])  // Camara trasera
            && !invertedCam
            && !totemCamActive) {

        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});
        invertedCam = true;

    } else if((glfwGetKey(device->GetWindow(),GLFW_KEY_O) || state.buttons[GLFW_GAMEPAD_BUTTON_Y])   //Camara totem
            && !invertedCam
            && !totemCamActive) {

        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});
        totemCamActive = true;

    } else if (!(glfwGetKey(device->GetWindow(),GLFW_KEY_I) || state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
            && !totemCamActive){

        invertedCam = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});

    } else if(!(glfwGetKey(device->GetWindow(),GLFW_KEY_O) || state.buttons[GLFW_GAMEPAD_BUTTON_Y])
            && !invertedCam) {

        totemCamActive = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});
    }

    // POWERUPS
    if ((glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) && !IsInputPressed(BUTTON_A)) {

        SetValueInput(InputXBox::BUTTON_A, true);
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});
        inputs.push_back(Constants::InputTypes::LAUNCH_PU);

    }else if(!(glfwGetKey(device->GetWindow(),GLFW_KEY_SPACE) || state.buttons[GLFW_GAMEPAD_BUTTON_A])){
        SetValueInput(InputXBox::BUTTON_A, false);
    }

    return inputs;
}



void InputFacadeClover::CheckInputPause(int& input, int maxInput){

    // esto es una cutrez que se hace porque si no al volver del menú no se oye el sonido de "salir" del menú. Que conste que yo no quería, Judith me obligó :(
    // No es cierto y tengo pruebas. Le dije que si era una cutrez no hacia falta :'c
    if (WeHaveToGoToMenu) {
        uint64_t now = Utils::getMillisSinceEpoch();
        uint64_t interval = now - timerGoToMenu;
        if(interval > 250) {
            RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
            Game::GetInstance()->SetState(State::MENU);
            input = 0;
            WeHaveToGoToMenu = false;
        }
    } else {
        //SUBIR
        if ( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true)
            && ( (IsInputPressed(BUTTON_STICK_UP) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_UP) ) ) {

            timeStart = system_clock::now();
            input--;
            if(input < 0)
                input = maxInput;
            SetValueInput(BUTTON_STICK_UP, true);
            //EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
            SoundFacadeManager::GetInstance()->GetSoundFacade()->PlayEvent("Menu/cambio_opcion");
        } else if( !IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) ){
            SetValueInput(BUTTON_STICK_UP, false);
        }

        //BAJAR
        if ( IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true)
            && ( (IsInputPressed(BUTTON_STICK_DOWN) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_DOWN) )) {

            timeStart = system_clock::now();
            input++;
            if(input > maxInput)
                input = 0;
            SetValueInput(BUTTON_STICK_DOWN, true);
            //EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
            SoundFacadeManager::GetInstance()->GetSoundFacade()->PlayEvent("Menu/cambio_opcion");
        } else if( !IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) ){
            SetValueInput(BUTTON_STICK_DOWN, false);
        }

        //IN GAME
        if ( IsKeyOrGamepadPress(GLFW_KEY_ESCAPE, GLFW_GAMEPAD_BUTTON_START, false, 0, 0, false) && !IsInputPressed(BUTTON_START)) {
            SetValueInput(InputXBox::BUTTON_START, true);
            //EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
            SoundFacadeManager::GetInstance()->GetSoundFacade()->PlayEvent("Menu/atras");
            Game::GetInstance()->SetState(State::INGAME_SINGLE);
            input = 0;
        } else if (!IsKeyOrGamepadPress(GLFW_KEY_ESCAPE, GLFW_GAMEPAD_BUTTON_START, false, 0, 0, false)) {
            SetValueInput(InputXBox::BUTTON_START, false);
        }

        //IN GAME o exit
        if ( (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) && HasDelayPassed() && !IsInputPressed(BUTTON_A)) {

            timeStart = system_clock::now();
            SetValueInput(BUTTON_A, true);
            //EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OK});
            SoundFacadeManager::GetInstance()->GetSoundFacade()->PlayEvent("Menu/aceptar");

        

            switch (input) {
                case 0: {
                    Game::GetInstance()->SetState(State::INGAME_SINGLE);
                    input = 0;                
                    break;
                }
                case 1: {
                    WeHaveToGoToMenu = true;
                    timerGoToMenu = Utils::getMillisSinceEpoch();
                    break;
                }
            }

        } else if ( !(IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) ) {
            SetValueInput(BUTTON_A, false);
        }
    }
}

void InputFacadeClover::CheckInputEndRace(int& input, int maxInput, bool menu){
    // esto es una cutrez que se hace porque si no al volver al menú no se oye el sonido de "salir". Que conste que yo no quería, Judith me obligó :(
    if (WeHaveToGoToMenu) {
        uint64_t now = Utils::getMillisSinceEpoch();
        uint64_t interval = now - timerGoToMenu;
        if(interval > 250) {
            EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
            //TODO: ¿Deberia resetear al volver al comenzar o al volver al menú?
            RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputGameOptions();
            RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputCharacter();
            WeHaveToGoToMenu = false;
        }
    } else {

        //ESPACIO
        if ( (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) && HasDelayPassed() && !IsInputPressed(BUTTON_A)) {

            timeStart = system_clock::now();
            SetValueInput(BUTTON_A, true);

            if ( !menu ) {
                RenderFacadeManager::GetInstance()->GetRenderFacade()->SetMenuEndRace(true);
            } else {

                EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OK});
                RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
                switch(input) {
                    case 0: {
                        if (multiplayer) {
                            EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_LOBBYMULTI});
                            vector<uint8_t> vacio;
                            GameValues::GetInstance()->SetCharacterSel(vacio);
                        } else {
                            //Manera un poco cutre de resetear el CId al empezar el juego
                            RenderFacadeManager::GetInstance()->GetRenderFacade()->SetNumEnemyCars(0);

                            auto cId = make_shared<CId>();
                            cId->ResetNumIds();
                            auto cNavMesh = make_shared<CNavMesh>();
                            cNavMesh->ResetNumIds();
                            EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMESINGLE});
                        }
                        break;
                    }
                    case 1: {                        
                        if (multiplayer) {
                            WeHaveToGoToMenu = true;
                            timerGoToMenu = Utils::getMillisSinceEpoch();
                            vector<uint8_t> vacio;
                            GameValues::GetInstance()->SetCharacterSel(vacio);
                        } else {
                            EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SELECT_CHARACTER});
                        }
                        break;
                    }
                    case 2: {
                        WeHaveToGoToMenu = true;
                        timerGoToMenu = Utils::getMillisSinceEpoch();
                        //RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputGameOptions();
                        //RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputCharacter();
                        break;
                    }
                }
                input = 0;
                RenderFacadeManager::GetInstance()->GetRenderFacade()->SetMenuEndRace(false);
            }

        } else if( !(IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) ) {
            SetValueInput(BUTTON_A, false);
        }


        if ( menu ) {

            //SUBIR
            if ( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) && ( (IsInputPressed(BUTTON_STICK_UP) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_UP) ) ) {
                timeStart = system_clock::now();
                input--;
                if (input < 0) {
                    input = maxInput;
                }
                if (multiplayer && input == 2) { 
                    input--; 
                }
                SetValueInput(BUTTON_STICK_UP, true);
                EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
            } else if ( !IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) ){
                SetValueInput(BUTTON_STICK_UP, false);
            }

            //BAJAR
            if (IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) && ( (IsInputPressed(BUTTON_STICK_DOWN) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_DOWN) ) ) {

                timeStart = system_clock::now();
                input++;
                if(input > maxInput) {
                    input = 0;
                }
                if (multiplayer && input == 2) { 
                    input = 0; 
                }
                SetValueInput(BUTTON_STICK_DOWN, true);
                EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
            } else if ( !IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) ){
                SetValueInput(BUTTON_STICK_DOWN, false);
            }
        }
    }
    //cout << "ESTOY EN EL INPUT DE ENDRACE NORMAL: " << input << endl;
}

void InputFacadeClover::CheckInputEndTournament(int& input, int maxInput, uint8_t menu){
    // esto es una cutrez que se hace porque si no al volver al menú no se oye el sonido de "salir". Que conste que yo no quería, Judith me obligó :(
    if (WeHaveToGoToMenu) {
        uint64_t now = Utils::getMillisSinceEpoch();
        uint64_t interval = now - timerGoToMenu;
        if(interval > 250) {
            EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
            //TODO: ¿Deberia resetear al volver al comenzar o al volver al menú?
            RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputTournamentOptions();
            RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputCharacter();
            WeHaveToGoToMenu = false;
        }
    } else {

        //ESPACIO
        if ( (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) && HasDelayPassed() && !IsInputPressed(BUTTON_A)) {

            timeStart = system_clock::now();
            SetValueInput(BUTTON_A, true);

            if ( menu==0 ) {
                RenderFacadeManager::GetInstance()->GetRenderFacade()->SetMenuEndTournament(1);
            } else if(menu==1){
                RenderFacadeManager::GetInstance()->GetRenderFacade()->SetMenuEndTournament(2);
            }else{

                EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OK});
                RenderFacadeManager::GetInstance()->GetRenderFacade()->CleanScene();
                switch(input) {
                    case 0: {
                        // se comprueba si hemos realizado todas las carreras
                        if(GameValues::GetInstance()->GetActualBattle() == GameValues::GetInstance()->GetNumBattles()){
                            GameValues::GetInstance()->ResetCountBattles();
                            GameValues::GetInstance()->ResetPoints();
                            //cout << "Reinicio numero de carreras\n";
                        }else{
                            //cout << "Incremento numero de carreras\n";
                            GameValues::GetInstance()->IncrementBattle();
                        }

                        //Manera un poco cutre de resetear el CId al empezar el juego
                        RenderFacadeManager::GetInstance()->GetRenderFacade()->SetNumEnemyCars(0);

                        auto cId = make_shared<CId>();
                        cId->ResetNumIds();
                        auto cNavMesh = make_shared<CNavMesh>();
                        cNavMesh->ResetNumIds();
                        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMESINGLE});
                        break;
                    }
                    case 1: {
                        if (GameValues::GetInstance()->GetNumBattles() != GameValues::GetInstance()->GetActualBattle()) {
                            WeHaveToGoToMenu = true;
                            timerGoToMenu = Utils::getMillisSinceEpoch();
                            RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputTournamentOptions();
                            RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputCharacter();
                            break;
                        }
                        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_SELECT_CHARACTER});
                        tournamentMode = true;
                        multiplayer = false;
                        //TODO: ¿Deberia resetear al volver al comenzar o al volver al menú?
                        break;
                    }
                    case 2: {
                        WeHaveToGoToMenu = true;
                        timerGoToMenu = Utils::getMillisSinceEpoch();
                        RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputTournamentOptions();
                        RenderFacadeManager::GetInstance()->GetRenderFacade()->ResetInputCharacter();
                        break;
                    }
                }
                input = 0;
                RenderFacadeManager::GetInstance()->GetRenderFacade()->SetMenuEndTournament(0);
            }

        } else if( !(IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) ) {
            SetValueInput(BUTTON_A, false);
        }


        if ( menu==2 ) {

            //SUBIR
            if ( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) && ( (IsInputPressed(BUTTON_STICK_UP) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_UP) ) ) {
                timeStart = system_clock::now();
                input--;
                if (input < 0) {
                    input = maxInput;
                } 
                if (input == 2 && GameValues::GetInstance()->GetActualBattle() != GameValues::GetInstance()->GetNumBattles()) {
                    input--;
                }
                SetValueInput(BUTTON_STICK_UP, true);
                EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
            } else if ( !IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) ){
                SetValueInput(BUTTON_STICK_UP, false);
            }

            //BAJAR
            if (IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) && ( (IsInputPressed(BUTTON_STICK_DOWN) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_DOWN) ) ) {

                timeStart = system_clock::now();
                input++;
                if(input > maxInput) {
                    input = 0;
                }
                if (input == 2 && GameValues::GetInstance()->GetActualBattle() != GameValues::GetInstance()->GetNumBattles()) {
                    input = 0;
                }
                SetValueInput(BUTTON_STICK_DOWN, true);
                EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
            } else if ( !IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) ){
                SetValueInput(BUTTON_STICK_DOWN, false);
            }
        }
    }
}

void InputFacadeClover::CheckInputLobbyMultiConnecting() {
    if ( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && !IsInputPressed(BUTTON_B)) {
        SetValueInput(InputXBox::BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0 , 0, false) ) {
        SetValueInput(InputXBox::BUTTON_B, false);
    }
}

void InputFacadeClover::CheckInputLobbyMultiWait() {
    if ( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && !IsInputPressed(BUTTON_B)) {
        SetValueInput(InputXBox::BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::RETURN_TO_SELCHAR});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0 , 0, false) ) {
        SetValueInput(InputXBox::BUTTON_B, false);
    }
}

void InputFacadeClover::CheckInputLobbyMultiExit() {
    // solo esperas a desconectarte
}

void InputFacadeClover::CheckInputLobbyMultiSelChar(int &input, int maxInput) {
    // atras
    if ( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && !IsInputPressed(BUTTON_B)) {
        SetValueInput(InputXBox::BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0 , 0, false) ) {
        SetValueInput(InputXBox::BUTTON_B, false);
    }

    CheckInputSelCharUpDown(input);
    CheckInputSelCharLeft(input, maxInput);
    CheckInputSelCharRight(input, maxInput);

    // intro
    CheckInputSelCharIntro(input);

}

void InputFacadeClover::CheckInputCredits() {

    //ATRAS
    if( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && HasDelayPassed() && !IsInputPressed(BUTTON_B) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});

    } else if( !IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) ) {
        SetValueInput(BUTTON_B, false);
    }
}

void InputFacadeClover::CheckInputSettings(std::vector<int> &inputs, int *maxInputs, int &option) {

    //ATRAS
    if( IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) && HasDelayPassed() && !IsInputPressed(BUTTON_B) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_BACK});
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
        option = 0;

    } else if( !IsKeyOrGamepadPress(GLFW_KEY_BACKSPACE, GLFW_GAMEPAD_BUTTON_B, false, 0, 0, false) ) {
        SetValueInput(BUTTON_B, false);
    }

    //ESPACIO
    if( (option != 0 || option != 1) && (IsKeyPress(GLFW_KEY_ENTER) || IsKeyPress(GLFW_KEY_SPACE) || IsGamepadPress(GLFW_GAMEPAD_BUTTON_A)) && HasDelayPassed() && !IsInputPressed(BUTTON_A) ) {

        timeStart = system_clock::now();
        SetValueInput(BUTTON_A, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
        if (inputs[option] == 0) { inputs[option] = 1; }
        else { inputs[option] = 0; }        
        ChangeSettings(option, inputs[option]);
    } else if( !IsKeyOrGamepadPress(GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_A, false, 0, 0, false) ) {
        SetValueInput(BUTTON_A, false);
    }


    //DERECHA
    if ((option == 0 || option == 1) && IsKeyOrGamepadPress(GLFW_KEY_RIGHT, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, true)
        && ( (IsInputPressed(BUTTON_STICK_R) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_R) )) {

        timeStart = system_clock::now();
        ++inputs[option];
        if(inputs[option] > maxInputs[option]) {
            inputs[option] = 0;
        }
        SetValueInput(BUTTON_STICK_R, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});

        ChangeSettings(option, inputs[option]);

    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_RIGHT, GLFW_GAMEPAD_AXIS_LEFT_X, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, true) ) {
        SetValueInput(BUTTON_STICK_R, false);
    }

    //IZQUIERDA
    if ( (option == 0 || option == 1) && IsKeyOrGamepadPress(GLFW_KEY_LEFT, GLFW_GAMEPAD_AXIS_LEFT_X, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, true)
         && ( (IsInputPressed(BUTTON_STICK_L) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_L) )) {

        timeStart = system_clock::now();
        --inputs[option];
        if(inputs[option] < 0) {
            inputs[option] = maxInputs[option];
        }
        SetValueInput(BUTTON_STICK_L, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});

        ChangeSettings(option, inputs[option]);

    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_LEFT, GLFW_GAMEPAD_AXIS_LEFT_X, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, true) ) {
        SetValueInput(BUTTON_STICK_L, false);
    }

    //ARRIBA
    if( IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true)
        && ( (IsInputPressed(BUTTON_STICK_UP) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_UP) )) {

        timeStart = system_clock::now();
        --option;
        if(option < 0) {
            option = inputs.size() - 1;
        }
        SetValueInput(BUTTON_STICK_UP, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if ( !IsKeyOrGamepadPress(GLFW_KEY_UP, GLFW_GAMEPAD_AXIS_LEFT_Y, true, -0.5, GLFW_GAMEPAD_BUTTON_DPAD_UP, true) ) {
        SetValueInput(BUTTON_STICK_UP, false);
    }

    //BAJAR
    if( IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true)
        && ( (IsInputPressed(BUTTON_STICK_DOWN) && HasDelayPassed() ) || !IsInputPressed(BUTTON_STICK_DOWN) )) {

        timeStart = system_clock::now();
        ++option;
        if(option > (static_cast<int>(inputs.size()) - 1)) {
            option = 0;
        }
        SetValueInput(BUTTON_STICK_DOWN, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::MENU_OPTION});
    } else if( !IsKeyOrGamepadPress(GLFW_KEY_DOWN, GLFW_GAMEPAD_AXIS_LEFT_Y, true, 0.5, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, true) ) {
        SetValueInput(BUTTON_STICK_DOWN, false);
    }

}


///////////////////////
//      CHANGES
///////////////////////

void InputFacadeClover::ChangeGameOptions(int option, int value) {
    switch (option) {
        case 0:
            if (value == 0) GameValues::GetInstance()->SetGameTime(120);
            if (value == 1) GameValues::GetInstance()->SetGameTime(180);
            if (value == 2) GameValues::GetInstance()->SetGameTime(240);
            if (value == 3) GameValues::GetInstance()->SetGameTime(300);
            break;
        case 1:
            if (value == 0) GameValues::GetInstance()->SetTimeTotem(30);
            if (value == 1) GameValues::GetInstance()->SetTimeTotem(45);
            if (value == 2) GameValues::GetInstance()->SetTimeTotem(60);
            break;
        case 2:
            if (value == 0) GameValues::GetInstance()->SetNumPlayers(3);
            if (value == 1) GameValues::GetInstance()->SetNumPlayers(4);
            if (value == 2) GameValues::GetInstance()->SetNumPlayers(5);
            if (value == 3) GameValues::GetInstance()->SetNumPlayers(6);
            //cout << "HE CAMBIADO EL NUMERO DE PERSONAJES WEY" << endl;
            break;
        case 3:
            if (value == 0) GameValues::GetInstance()->SetDifficultAI(DifficultyAI::EASY);
            if (value == 1) GameValues::GetInstance()->SetDifficultAI(DifficultyAI::NORMAL);
            if (value == 2) GameValues::GetInstance()->SetDifficultAI(DifficultyAI::DIFFICULT);
            //cout << "Ahora la dificultad es: " << static_cast<int>(GameValues::GetInstance()->GetDifficultAI()) << endl;
            break;
        default: break;
    }
}

void InputFacadeClover::ChangeSettings(int option, int value) {
    shared_ptr<DataMap> d = make_shared<DataMap>();
    float valueMusic;
    switch (option) {
        case 0:
            valueMusic = value*0.33f;
            (*d)[NUM] = valueMusic;
            EventManager::GetInstance().AddEventMulti(Event{EventType::SET_GLOBAL_VOLUME, d});
            break;
        case 1:
            valueMusic = value*0.33f;
            (*d)[NUM] = valueMusic;
            EventManager::GetInstance().AddEventMulti(Event{EventType::SET_MUSIC_VOLUME, d});
            //cout << "TOY CAMBIANDO EL VOLUMEN DE LA MUSICA (No ta hecho)\n";
            break;
        case 2:
            (*d)[TRUEFALSE] = value;
            //cout << value << endl;
            EventManager::GetInstance().AddEventMulti(Event{EventType::ENABLE_PARTICLES, d});
            //cout << "TOY PONIENDO O QUITANDO PARTICULAS: " << value << "\n";
            break;
        case 3:
            (*d)[TRUEFALSE] = value;
            EventManager::GetInstance().AddEventMulti(Event{EventType::ENABLE_VEGETATION, d});
            //cout << "TOY PONIENDO O QUITANDO VEGETACION: " << value << "\n";
            break;
        case 4:
            (*d)[TRUEFALSE] = value;
            EventManager::GetInstance().AddEventMulti(Event{EventType::ENABLE_SHADOWS, d});
            //cout << "TOY PONIENDO O QUITANDO SOMBRAS: " << value << "\n";
            break;
        default:
            //cout << "Esta opcion de ajustes no me gusta" << endl;
            break;
    }
}


bool InputFacadeClover::CheckAnyKey(){
    if(IsKeyPress(GLFW_KEY_Q) || IsKeyPress(GLFW_KEY_W) || IsKeyPress(GLFW_KEY_E) || IsKeyPress(GLFW_KEY_R) || IsKeyPress(GLFW_KEY_T) || IsKeyPress(GLFW_KEY_Y) || IsKeyPress(GLFW_KEY_U) || IsKeyPress(GLFW_KEY_I) || IsKeyPress(GLFW_KEY_O) || IsKeyPress(GLFW_KEY_P) || IsKeyPress(GLFW_KEY_A) || IsKeyPress(GLFW_KEY_S) || IsKeyPress(GLFW_KEY_D) || IsKeyPress(GLFW_KEY_F) || IsKeyPress(GLFW_KEY_G) || IsKeyPress(GLFW_KEY_H) || IsKeyPress(GLFW_KEY_J) || IsKeyPress(GLFW_KEY_K) || IsKeyPress(GLFW_KEY_L) || IsKeyPress(GLFW_KEY_Z) || IsKeyPress(GLFW_KEY_X) || IsKeyPress(GLFW_KEY_C) || IsKeyPress(GLFW_KEY_V) || IsKeyPress(GLFW_KEY_B) || IsKeyPress(GLFW_KEY_N) || IsKeyPress(GLFW_KEY_M) || IsKeyPress(GLFW_KEY_1) || IsKeyPress(GLFW_KEY_2) || IsKeyPress(GLFW_KEY_3) || IsKeyPress(GLFW_KEY_4) || IsKeyPress(GLFW_KEY_5) || IsKeyPress(GLFW_KEY_6) || IsKeyPress(GLFW_KEY_7) || IsKeyPress(GLFW_KEY_8) || IsKeyPress(GLFW_KEY_9) || IsKeyPress(GLFW_KEY_0)){
        return true;
    }else{
        return false;
    }
}