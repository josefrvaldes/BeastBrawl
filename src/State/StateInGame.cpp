#include "StateInGame.h"
#include <iostream>


#pragma region BT
////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
bool door = false; // imaginemos que door=false es = puerta cerrada "door is open?"
bool key  = false;  // tenemos una llave
// ACCION DE ABRIR LA PUERTA
struct openDoor : public behaviourTree { 
		virtual bool run() override {
           door = true;
           cout << "Abrimos la puerta" << endl;
           return true;
		}
};
//ACCION COGER LA LLAVE
struct getKey : public behaviourTree { 
		virtual bool run() override {
           key = true;
           cout << "Cogemos la llave" << endl;
           return true;
		}
};
//CONDICION PUERTA ABIERTA?
struct haveDoorOpen : public behaviourTree { 
		virtual bool run() override {
           cout << "Comprobamos si la puerta esta abierta: " << boolalpha << door << endl;
           return door;
		}
};
//CONDICION TENEMOS LLAVE?
struct haveKey : public behaviourTree { 
		virtual bool run() override {
           cout << "Comprobamos si tenemos la llave: " << boolalpha << key << endl;
			return  key;
		}
};
///// DECORATORS //////
struct Minimum : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
   uint32_t totalTries = 3;
   uint32_t numTries = 0;
   virtual bool run() override {
       if(numTries>=totalTries)
           return getChild()->run();
       numTries++;
       cout << "Fallamos al coger la llave, intento: " << numTries << endl;
       return false;
       
   }
};
struct Limit : public Decorator {  // Decorator Limit
   uint32_t totalLimit = 3;
   uint32_t numLimit = 0;
   virtual bool run() override {
       if(numLimit>=totalLimit)
           return false;
       numLimit++;
       return getChild()->run();
   }
};
struct UntilFail : public Decorator {  // Decorator UntilFail
   virtual bool run() override {
       while(true){
           bool result = getChild()->run();
           if(!result) { 
               break; 
           }
       }
       return true;
   }
};
struct Inverter : public Decorator {  // Decorator Inverter
   virtual bool run() override {
       return !(getChild()->run());
   }
};

#pragma endregion



StateInGame::StateInGame(){
    // constructor
    cout << "Estado InGame Creado" << std::endl;
    
    eventManager = EventManager::GetInstance();

    car = make_shared<Car>();
    manPowerUps = make_shared<ManPowerUp>();
    ground = make_shared<GameObject>(glm::vec3(10.0f,10.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(100.0f,1.0f,100.0f), "wall.jpg", "ninja.b3d");
    cam = make_shared<Camera>(glm::vec3(10.0f,40.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));

	renderFacadeManager = RenderFacadeManager::GetInstance();
	renderFacadeManager->InitializeIrrlicht();

    inputFacadeManager = InputFacadeManager::GetInstance();
    inputFacadeManager->InitializeIrrlicht();

    physicsFacadeManager = PhysicsFacadeManager::GetInstance();
    physicsFacadeManager->InitializeIrrlicht();
    
    unique_ptr<Physics> physics = make_unique<Physics>();
    //Almacenamos los motores
	renderEngine   = renderFacadeManager->GetRenderFacade();
    inputEngine    = inputFacadeManager->GetInputFacade();
    physicsEngine  = physicsFacadeManager->GetPhysicsFacade();


#pragma region FL

// --------------------------- FUZZY LOGIC  "COUT TEMPORALES" ----------------------------------
    shared_ptr<FuzzyLogic> fm = make_shared<FuzzyLogic>();

    shared_ptr<FuzzyVariable> DistToTarget = fm->CreateFLV("DistToTarget");
    shared_ptr<FzSet> Target_Close = DistToTarget->AddLeftShoulderSet("Target_Close", 0, 25, 150);
    shared_ptr<FzSet> Target_Medium = DistToTarget->AddTriangularSet("Target_Medium", 25, 150, 300);
    shared_ptr<FzSet> Target_Far = DistToTarget->AddRightShoulderSet("Target_Far", 150, 300, 400);

    shared_ptr<FuzzyVariable> AmmoStatus = fm->CreateFLV("AmmoStatus");
    shared_ptr<FzSet> Ammo_Low = AmmoStatus->AddLeftShoulderSet("Ammo_Low", 0, 0, 10);
    shared_ptr<FzSet> Ammo_Okay = AmmoStatus->AddTriangularSet("Ammo_Okay", 0, 10, 30);
    shared_ptr<FzSet> Ammo_Loads = AmmoStatus->AddRightShoulderSet("Ammo_Loads", 10, 30, 40);

    shared_ptr<FuzzyVariable> Desirability = fm->CreateFLV("Desirability");
    shared_ptr<FzSet> Undesirable = Desirability->AddLeftShoulderSet("Undesirable", 0, 25, 50);
    shared_ptr<FzSet> Desirable = Desirability->AddTriangularSet("Desirable", 25, 50, 75);
    shared_ptr<FzSet> VeryDesirable = Desirability->AddRightShoulderSet("VeryDesirable", 50, 75, 100);
    // To-Do: revisar el new por que no se tiene que hacer
    fm->AddRule( (make_shared<FzAND>(Target_Close, Ammo_Loads)), Undesirable);
    fm->AddRule( (make_shared<FzAND>(Target_Close, Ammo_Okay)), Undesirable);
    fm->AddRule( (make_shared<FzAND>(Target_Close, Ammo_Low)), Undesirable);
    fm->AddRule( (make_shared<FzAND>(Target_Medium, Ammo_Loads)), VeryDesirable);
    fm->AddRule( (make_shared<FzAND>(Target_Medium, Ammo_Okay)), VeryDesirable);
    fm->AddRule( (make_shared<FzAND>(Target_Medium, Ammo_Low)), Desirable);
    fm->AddRule( (make_shared<FzAND>(Target_Far, Ammo_Loads)), Desirable);
    fm->AddRule( (make_shared<FzAND>(Target_Far, Ammo_Okay)), Undesirable);
    fm->AddRule( (make_shared<FzAND>(Target_Far, Ammo_Low)), Undesirable);
    // seguimos con las pruebas
    fm->Fuzzify("DistToTarget", 200); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
    fm->Fuzzify("AmmoStatus", 8);
    double resultadoDefuzzification = fm->DeFuzzify("Desirability");
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
   shared_ptr<selector> selector1 = make_shared<selector>();
   shared_ptr<sequence> sequence1 = make_shared<sequence>();

   shared_ptr<haveDoorOpen> puertaAbiertaSiNo = make_shared<haveDoorOpen>();
   shared_ptr<haveKey> tengoLlaveSiNo = make_shared<haveKey>();
   shared_ptr<openDoor>abrirPuerta = make_shared<openDoor>();
   shared_ptr<getKey> cogerLlave = make_shared<getKey>();

   shared_ptr<Minimum> tryCatchKey3 = make_shared<Minimum>();

   selector1->addChild(puertaAbiertaSiNo);
   selector1->addChild(sequence1);
   selector1->addChild(tryCatchKey3);

   sequence1->addChild(tengoLlaveSiNo);
   sequence1->addChild(abrirPuerta);

   tryCatchKey3->addChild(cogerLlave);

	cout << "--------------------" << endl;
   while (door==false){
       selector1->run();
   } // If the operation starting from the root fails, keep trying until it succeeds.
	cout << "--------------------" << endl;
//

#pragma endregion



    renderEngine->FacadeAddObject(car.get());
    renderEngine->FacadeAddObject(ground.get());
    
    for(shared_ptr<PowerUp> pu : manPowerUps->GetEntities()) 
        renderEngine->FacadeAddObject(pu.get());
        
    renderEngine->FacadeAddCamera(cam.get());

    lastFPS = -1;
    then = renderEngine->FacadeGetTime();
}



StateInGame::~StateInGame(){
    // destructor
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
    renderEngine->FacadeCheckInput(frameDeltaTime,car.get(),cam.get());
    physicsEngine->Update(car.get(), cam.get());

    renderEngine->FacadeDraw();


    int fps = renderEngine->FacadeGetFPS();
    lastFPS = fps;
}