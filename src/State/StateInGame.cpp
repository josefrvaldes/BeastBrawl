#include "StateInGame.h"
#include <iostream>

using namespace std;

StateInGame::StateInGame(){
    // constructor
    std::cout << "Estado InGame Creado" << std::endl;
    
    eventManager = EventManager::GetInstance();

    car = new Car();
    manPowerUps = new ManPowerUp();
    ground = new GameObject(10.0,10.0,30.0,    0.0,0.0,0.0,    100.0,1.0,100.0, "wall.jpg", "ninja.b3d", 20,0.15,0.1,0.25);
    cam = new Camera(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);

	renderFacadeManager = RenderFacadeManager::GetInstance();
	renderFacadeManager->InitializeIrrlicht();

    inputFacadeManager = InputFacadeManager::GetInstance();
    inputFacadeManager->InitializeIrrlicht();

    physicsFacadeManager = PhysicsFacadeManager::GetInstance();
    physicsFacadeManager->InitializeIrrlicht();
    
    Physics* physics = new Physics();
    //Almacenamos los motores
	renderEngine   = renderFacadeManager->GetRenderFacade();
    inputEngine    = inputFacadeManager->GetInputFacade();
    physicsEngine  = physicsFacadeManager->GetPhysicsFacade();

    

// --------------------------- FUZZY LOGIC  "COUT TEMPORALES" ----------------------------------
FuzzyLogic fm;

FuzzyVariable& DistToTarget = fm.CreateFLV("DistToTarget");
FzSet Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
FzSet Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
FzSet Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 150, 300, 400);

FuzzyVariable& AmmoStatus = fm.CreateFLV("AmmoStatus");
FzSet Ammo_Low = AmmoStatus.AddLeftShoulderSet("Ammo_Low", 0, 0, 10);
FzSet Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay", 0, 10, 30);
FzSet Ammo_Loads = AmmoStatus.AddRightShoulderSet("Ammo_Loads", 10, 30, 40);

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
fm.AddRule( *(new FzAND(Target_Far, Ammo_Okay)), Undesirable);
fm.AddRule( *(new FzAND(Target_Far, Ammo_Low)), Undesirable);
// seguimos con las pruebas
  fm.Fuzzify("DistToTarget", 200); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
  fm.Fuzzify("AmmoStatus", 8);
  double resultadoDefuzzification = fm.DeFuzzify("Desirability");
// cout de FuzzyLogic
  std::cout << "defuzzificacion: " << resultadoDefuzzification << std::endl;
  std::cout << "------------------------------"<< std::endl;


// --------------------------- FIN FUZZY LOGIC ----------------------------------



    renderEngine->FacadeAddObject(car);
    renderEngine->FacadeAddObject(ground);
    
    for(PowerUp *pu : manPowerUps->GetEntities()) 
        renderEngine->FacadeAddObject(pu);
        
    renderEngine->FacadeAddCamera(cam);

    lastFPS = -1;
    then = renderEngine->FacadeGetTime();
}



StateInGame::~StateInGame(){
    // destructor
    delete car;
    delete ground;
    delete cam;
}



void StateInGame::Render(){

}


void StateInGame::Update()
{    
    eventManager->Update();  
    const uint32_t now = renderEngine->FacadeGetTime();
    
    const float frameDeltaTime = (float)(now - then) / 100.0;
    then = now;

    
    //inputEngine->CheckInputs(*car);
    renderEngine->FacadeCheckInput(frameDeltaTime,car,cam);
    physicsEngine->Update(car, cam);

    renderEngine->FacadeDraw();


    int fps = renderEngine->FacadeGetFPS();
    lastFPS = fps;
}