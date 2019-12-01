#include "entities/hero.h"
#include "components/position.h"
#include "components/speed.h"
#include "systems/physics.h"
#include "EventManager/EventManager.h"
#include "fuzzyLogic/fuzzyLogic.h"

#include "Game.h"
#include <iostream>
#include <list>
#include <string>





#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;



void pruebaEvent1(Data d){
    
    std::cout << "Soy pruebaEvent1"<< "\n";
}
void pruebaEvent2(Data d){
    std::cout << "Soy pruebaEvent2"<< "\n";
}



class MyEventReceiver : public IEventReceiver
{
public:
  // This is the one method that we have to implement
  virtual bool OnEvent(const SEvent& event)
  {
    // Remember whether each key is down or up
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
      KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
  }
  // This is used to check whether a key is being held down
  virtual bool IsKeyDown(EKEY_CODE keyCode) const
  {
    return KeyIsDown[keyCode];
  }
  MyEventReceiver()
  {
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
      KeyIsDown[i] = false;
  }
private:
  // We use this array to store the current state of each key
  bool KeyIsDown[KEY_KEY_CODES_COUNT];
};




int main()
{
    Game *game = Game::GetInstance();
    
    game->SetState(State::States::INGAME);
    game->InitGame();


    EventManager eventManager = EventManager::GetInstance();
    eventManager.Suscribe(Listener {EventType::PRIORIDAD1,pruebaEvent1, "suscriptor1"});
    eventManager.Suscribe(Listener {EventType::PRIORIDAD1,pruebaEvent1, "suscriptor2"});
    eventManager.Suscribe(Listener {EventType::PRIORIDAD2,pruebaEvent2, "suscriptor3"});

    Data d;

    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD2,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
    
    eventManager.Update();  
    cout << "------------------------------\n";
    eventManager.UnSuscribe(EventType::PRIORIDAD1,"suscriptor2");

    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD2,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});

    
    eventManager.Update();  

// --------------------------- FUZZY LOGIC ----------------------------------
//
FuzzyLogic fm;

FuzzyVariable& DistToTarget = fm.CreateFLV("DistToTarget");
FzSet Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
FzSet Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
FzSet Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 150, 300, 400);

FuzzyVariable& AmmoStatus = fm.CreateFLV("AmmoStatus");
FzSet Ammo_Low = AmmoStatus.AddLeftShoulderSet("Ammo_Low", 0, 0, 10);
FzSet Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay", 0, 10, 30);
FzSet Ammo_Loads = AmmoStatus.AddRightShoulderSet("Ammo_Loads", 10, 30, 40);

// resultDO FINAL DE 0 A 100
FuzzyVariable& Desirability = fm.CreateFLV("Desirability");
FzSet Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);
FzSet Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
FzSet VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);


// To-Do: revisar el new por que no se tiene que hacer
fm.AddRule( *(new FzAND(Target_Close, Ammo_Loads)), Undesirable);
fm.AddRule( *(new FzAND(Target_Close, Ammo_Okay)), Undesirable);
fm.AddRule( *(new FzAND(Target_Close, Ammo_Low)), Undesirable);
fm.AddRule( *(new FzAND(Target_Medium, Ammo_Loads)), VeryDesirable);
fm.AddRule( *(new FzAND(Target_Medium, Ammo_Okay)), VeryDesirable);
fm.AddRule( *(new FzAND(Target_Medium, Ammo_Low)), Desirable);
fm.AddRule( *(new FzAND(Target_Far, Ammo_Loads)), Desirable);
fm.AddRule( *(new FzAND(Target_Far, Ammo_Okay)), Desirable);
fm.AddRule( *(new FzAND(Target_Far, Ammo_Low)), Undesirable);


  std::cout << "La fuzzificacion de DistToTarget deberia de ser: 0 - 0.Alto - 0.33"<< std::endl;
  std::cout << "La fuzzificacion de AmmoStatus deberia de ser  : 0 - 0.2    - 0.78"<< std::endl;





// seguimos con las pruebas
  fm.Fuzzify("DistToTarget", 200); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
  fm.Fuzzify("AmmoStatus", 8);
  double resultadoDefuzzification = fm.DeFuzzify("Desirability");

// cout de FuzzyLogic
  std::cout << "defuzzificacion: " << resultadoDefuzzification << std::endl;
  std::cout << "------------------------------"<< std::endl;


  std::cout << "Fallan el triangulo por la derecha y posiblemente por la izquierda"<< std::endl;





















// --------------------------- IRLICH ---------------------------------------
// ask user for driver
  video::E_DRIVER_TYPE driverType=driverChoiceConsole();
  if (driverType==video::EDT_COUNT)
    return 1;


  // create device
  MyEventReceiver receiver;
  IrrlichtDevice* device = createDevice(driverType,
      core::dimension2d<u32>(1280, 720), 16, false, false, false, &receiver);

  if (device == 0)
    return 1; // could not create selected driver.

  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();

  /*
  Create the node which will be moved with the WSAD keys. We create a
  sphere node, which is a built-in geometry primitive. We place the node
  at (0,0,30) and assign a texture to it to let it look a little bit more
  interesting. Because we have no dynamic lights in this scene we disable
  lighting for each model (otherwise the models would be black).
  */
  scene::ISceneNode * node = smgr->addCubeSceneNode();
  if (node)
  {
    node->setPosition(core::vector3df(0,0,30));
    node->setMaterialTexture(0, driver->getTexture("media/particle.bmp"));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
  }

  /*
  To be able to look at and move around in this scene, we create a first
  person shooter style camera and make the mouse cursor invisible.
  */
  smgr->addCameraSceneNodeFPS();
  device->getCursorControl()->setVisible(false);

  /*
  Add a colorful irrlicht logo
  */
// aqui podria ir un marcador o algo 
  device->getGUIEnvironment()->addImage(
    driver->getTexture("media/irrlichtlogoalpha2.tga"),
    core::position2d<s32>(10,20));

  gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(
    L"", core::rect<s32>(10, 10, 400, 20));
  diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

  /*
  We have done everything, so lets draw it. We also write the current
  frames per second and the name of the driver to the caption of the
  window.
  */
  int lastFPS = -1;

  // In order to do framerate independent movement, we have to know
  // how long it was since the last frame
  u32 then = device->getTimer()->getTime();

  // This is the movemen speed in units per second.
  const f32 MOVEMENT_SPEED = 5.f;




while(device->run())
  {
    // Work out a frame delta time.
    const u32 now = device->getTimer()->getTime();
    const f32 frameDeltaTime = (f32)(now - then) / 100.f; // Time in seconds
    then = now;

    /* Check if keys W, S, A or D are being held down, and move the
    sphere node around respectively. */
    core::vector3df nodePosition = node->getPosition();

    if(receiver.IsKeyDown(irr::KEY_KEY_W))
      nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
    else if(receiver.IsKeyDown(irr::KEY_KEY_S))
      nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;

    if(receiver.IsKeyDown(irr::KEY_KEY_A))
      nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
    else if(receiver.IsKeyDown(irr::KEY_KEY_D))
      nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

    node->setPosition(nodePosition);

    driver->beginScene(true, true, video::SColor(255,113,113,133));

    smgr->drawAll(); // draw the 3d scene
    device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

    driver->endScene();

    int fps = driver->getFPS();

    if (lastFPS != fps)
    {
      core::stringw tmp(L"Movement Example - Irrlicht Engine [");
      tmp += driver->getName();
      tmp += L"] fps: ";
      tmp += fps;

      device->setWindowCaption(tmp.c_str());
      lastFPS = fps;
    }
  }

  /*
  In the end, delete the Irrlicht device.
  */
  device->drop();
  
  return 0;
}

/*
That's it. Compile and play around with the program.
**/



    //return 0;
    /*
    // creamos a nuestro hero
    Hero *hero = new Hero();
    cout << "Nuestro hero tiene position? " << hero->HasComponent(CompType::Position) << endl;


    // creamos nuestro componente position
    CPosition *pos = new CPosition(1.2, 4.3, -3.1);
    hero->AddComponent(pos);

    cout << "Nuestro hero tiene position? " << hero->HasComponent(CompType::Position) << endl;

    // ahora obtenemos la posición a partir de la entidad que hemos creado
    //CPosition *posObtenida = (CPosition *) h->getComponents().at(CompType::Position); // alternativa 2 a obtener el componente
    CPosition *posObtenida = (CPosition *) hero->getComponents()[CompType::Position];
    cout << "La posición de nuestro hero es " << *posObtenida << endl;

    // También se podría hacer de una forma más compacta directamente usando el hero
    cout << "La posición de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Position]) << endl;

    
    // ahora creamos un componente velocidad
    CSpeed *speed = new CSpeed(2.7, 7.1, -1.1);

    // y se la agregamos a nuestro hero igual que antes
    hero->AddComponent(speed);

    // y la mostramos por pantalla
    cout << "La velocidad de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Speed]) << endl;


    // Ahora movemos a nuestro hero usando un sistema physics
    // requiere que le mandemos el componente speed y el componente position, y él hace el cálculo.
    // como veis, no necesita de la entidad, por tanto es totalmente reutilizable para cualquier 
    // entidad que tenga speed y position
    Physics::move(
        (CSpeed *)hero->getComponents()[CompType::Speed], 
        (CPosition *)hero->getComponents()[CompType::Position]
    );

    // y mostramos su nueva posición por pantalla
    cout << "La posición de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Position]) << endl;


    delete speed;
    delete posObtenida;
    delete hero;*/
//}