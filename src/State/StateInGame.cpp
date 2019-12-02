#include "StateInGame.h"
#include <iostream>

using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
//bool door = false; // imaginemos que door=false es = puerta cerrada "door is open?"
//bool key  = false;  // tenemos una llave
//// ACCION DE ABRIR LA PUERTA
//struct openDoor : public behaviourTree { 
//		virtual bool run() override {
//            door = true;
//            cout << "Abrimos la puerta" << endl;
//            return true;
//		}
//};
////ACCION COGER LA LLAVE
//struct getKey : public behaviourTree { 
//		virtual bool run() override {
//            key = true;
//            cout << "Cogemos la llave" << endl;
//            return true;
//		}
//};
////CONDICION PUERTA ABIERTA?
//struct haveDoorOpen : public behaviourTree { 
//		virtual bool run() override {
//            cout << "Comprobamos si la puerta esta abierta: " << boolalpha << door << endl;
//            return door;
//		}
//};
////CONDICION TENEMOS LLAVE?
//struct haveKey : public behaviourTree { 
//		virtual bool run() override {
//            cout << "Comprobamos si tenemos la llave: " << boolalpha << key << endl;
//			return  key;
//		}
//};
/////// DECORATORS //////
//struct Minimum : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
//    uint32_t totalTries = 3;
//    uint32_t numTries = 0;
//    virtual bool run() override {
//        if(numTries>=totalTries)
//            return getChild()->run();
//        numTries++;
//        cout << "Fallamos al coger la llave, intento: " << numTries << endl;
//        return false;
//        
//    }
//};
//struct Limit : public Decorator {  // Decorator Limit
//    uint32_t totalLimit = 3;
//    uint32_t numLimit = 0;
//    virtual bool run() override {
//        if(numLimit>=totalLimit)
//            return false;
//        numLimit++;
//        return getChild()->run();
//    }
//};
//struct UntilFail : public Decorator {  // Decorator UntilFail
//    virtual bool run() override {
//        while(true){
//            bool result = getChild()->run();
//            if(!result) { 
//                break; 
//            }
//        }
//        return true;
//    }
//};
//struct Inverter : public Decorator {  // Decorator Inverter
//    virtual bool run() override {
//        return !(getChild()->run());
//    }
//};





StateInGame::StateInGame(){
    // constructor
    std::cout << "Estado InGame Creado" << std::endl;
    
    eventManager = EventManager::GetInstance();

    car = new Car();
    manPowerUps = new ManPowerUp();
    ground = new GameObject(glm::vec3(10.0f,10.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(100.0f,1.0f,100.0f), "wall.jpg", "ninja.b3d");
    cam = new Camera(glm::vec3(10.0f,40.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));

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

// --------------------------- BEHAVIOR TREE ----------------------------------

    //BehaviorTree BASICO
                    // SELECTOR1
                        // 
        ///////////////////////////////////////////////////////////////////////////////////
        //                                      //                                       //
// La pueta esta abierta?                     SEQUENCE                               DECORATOR (minimum) (3 intentos)
                                    ///////////////////////////////                      //
                                    //                          //                       //
//                                // tengo llave?             //abrir puerta        // coger llave
//    selector* selector1 = new selector;
//    sequence* sequence1 = new sequence;
//
//    haveDoorOpen* puertaAbiertaSiNo = new haveDoorOpen;
//    haveKey* tengoLlaveSiNo = new haveKey;
//    openDoor* abrirPuerta = new openDoor;
//    getKey* cogerLlave = new getKey;
//
//    Minimum* tryCatchKey3 = new Minimum;
//
//    selector1->addChild(puertaAbiertaSiNo);
//    selector1->addChild(sequence1);
//    selector1->addChild(tryCatchKey3);
//
//    sequence1->addChild(tengoLlaveSiNo);
//    sequence1->addChild(abrirPuerta);
//
//    tryCatchKey3->addChild(cogerLlave);
//
//	cout << "--------------------" << endl;
//    while (door==false){
//        selector1->run();
//    } // If the operation starting from the root fails, keep trying until it succeeds.
//	cout << "--------------------" << endl;
//




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