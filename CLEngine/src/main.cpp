#include <iostream>
#include "./include/glew/glew.h"
#include "./include/sdl/SDL.h"

using namespace std;

int main() {
    SDL_Window *window;                    

    // Inicializa SDL
    SDL_Init(SDL_INIT_VIDEO);
    cout << ">>>>> SDL ON" << endl;

    // TO-DO: Cambiar a la version de OpenGL que usemos
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create an application window with the following settings:
    window = SDL_CreateWindow( 
        "Beast Brawl",                     // Titulo
        SDL_WINDOWPOS_UNDEFINED,           // Posicion inicial de X
        SDL_WINDOWPOS_UNDEFINED,           // Posicion inicial de Y
        1280,                              // Anchura
        720,                               // Altura
        0                                  // Flags - ************************
    );
        

    if (!window) {
        cout << "No se ha podido crear la ventana: " << SDL_GetError() << endl;
        SDL_Quit();
        return -1;
    }

    SDL_GLContext sdlContext = SDL_GL_CreateContext(window);    // Contexto de la ventana
    glViewport(0, 0, 1280, 720);
    
    cout << "    > Ventana SDL abierta" << endl;
    

    bool exit = false;
    SDL_Event e;

    while (!exit) {
        while (SDL_PollEvent(&e)) {
            if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                exit = true;
            }
        }

        // Renderizar...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    //cout << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION)<< endl;

    // Para parar liberar todas las inicializaciones por SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    cout << ">>>>> SDL OFF" << endl;

    return 0;
}