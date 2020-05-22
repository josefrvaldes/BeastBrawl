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
 
 
 #include "PhysicsAI.h"

// #include <Entities/CarAI.h>
// #include <Components/CShield.h>
// #include <Components/CTotem.h>
// #include <Components/CNitro.h>
// #include "../Entities/CarHuman.h"
// 
// #include <Facade/Render/RenderFacadeManager.h>



PhysicsAI::PhysicsAI(){

}


/*
void PhysicsAI::Update(CarAI* carAI, ManWayPoint* graph){
    UpdateCarPowerUps(carAI);
}


void PhysicsAI::UpdateCarPowerUps(Entity* carAI){
    auto cTotem = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());
    if(cTotem->active){
        cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
    }

    if(cTotem->accumulatedTime/1000.0 > cTotem->durationTime/1000.0){
        cout << "Has ganado IA\n";
        //Game::GetInstance()->SetState(State::ENDRACE);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});

    }

    // Actualiza el componente nitro
    auto cNitro = static_cast<CNitro *>(carAI->GetComponent(CompType::NitroComp).get());
    if(cNitro->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime){  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }

    // Actualiza el componente escudo
    auto cShield = static_cast<CShield *>(carAI->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime){  // comprueba el tiempo desde que se lanzo
        cShield->deactivePowerUp();
    }
}*/

