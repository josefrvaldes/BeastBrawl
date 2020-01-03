#include "InputFacadeClover.h"

InputFacadeClover::InputFacadeClover(){
    //eventManager = EventManager::GetInstance();
	//RenderFacadeManager::GetInstance().GetRenderFacade()->AddReceiver(receiver);
    cout << "Creado el input de Clover\n"; 
    
    

}

void InputFacadeClover::CheckInput(){
    SDL_Event event;

  
  /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
  /* more events on the event queue, our while loop will exit when */
  /* that occurs.                                                  */
    while( SDL_PollEvent( &event ) != 0){
      cout << "ENTRAAAAAAA\n";

      if(event.type == SDL_KEYDOWN){
          cout << "BROOOO\n";
      }
   
    }
    

}


void InputFacadeClover::CheckInputMenu(){
}


void InputFacadeClover::CheckInputPause(){
}


void InputFacadeClover::CheckInputEndRace(){
}



InputFacadeClover::~InputFacadeClover(){
	
}
