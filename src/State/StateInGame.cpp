#include "StateInGame.h"
#include <iostream>

#include "../Components/CWayPointEdges.h"
#pragma region BT
////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
// bool door = false; // imaginemos que door=false es = puerta cerrada "door is open?"
// bool key  = false;  // tenemos una llave
// // ACCION DE ABRIR LA PUERTA
// struct openDoor : public behaviourTree { 
// 		virtual bool run() override {
//            door = true;
//            cout << "Abrimos la puerta" << endl;
//            return true;
// 		}
// };
// //ACCION COGER LA LLAVE
// struct getKey : public behaviourTree { 
// 		virtual bool run() override {
//            key = true;
//            cout << "Cogemos la llave" << endl;
//            return true;
// 		}
// };
// //CONDICION PUERTA ABIERTA?
// struct haveDoorOpen : public behaviourTree { 
// 		virtual bool run() override {
//            cout << "Comprobamos si la puerta esta abierta: " << boolalpha << door << endl;
//            return door;
// 		}
// };
// //CONDICION TENEMOS LLAVE?
// struct haveKey : public behaviourTree { 
// 		virtual bool run() override {
//            cout << "Comprobamos si tenemos la llave: " << boolalpha << key << endl;
// 			return  key;
// 		}
// };
// ///// DECORATORS //////
// struct Minimum : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
//    uint32_t totalTries = 3;
//    uint32_t numTries = 0;
//    virtual bool run() override {
//        if(numTries>=totalTries)
//            return getChild()->run();
//        numTries++;
//        cout << "Fallamos al coger la llave, intento: " << numTries << endl;
//        return false;
       
//    }
// };
// struct Limit : public Decorator {  // Decorator Limit
//    uint32_t totalLimit = 3;
//    uint32_t numLimit = 0;
//    virtual bool run() override {
//        if(numLimit>=totalLimit)
//            return false;
//        numLimit++;
//        return getChild()->run();
//    }
// };
// struct UntilFail : public Decorator {  // Decorator UntilFail
//    virtual bool run() override {
//        while(true){
//            bool result = getChild()->run();
//            if(!result) { 
//                break; 
//            }
//        }
//        return true;
//    }
// };
// struct Inverter : public Decorator {  // Decorator Inverter
//    virtual bool run() override {
//        return !(getChild()->run());
//    }
// };

#pragma endregion



StateInGame::StateInGame(){
    // constructor
    cout << "Estado InGame Creado" << std::endl;
    
    eventManager = EventManager::GetInstance();

    car = make_shared<Car>();
    manPowerUps = make_shared<ManPowerUp>();
    ground = make_shared<GameObject>(glm::vec3(10.0f,10.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(100.0f,1.0f,100.0f), "wall.jpg", "ninja.b3d");
    cam = make_shared<Camera>(glm::vec3(10.0f,40.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    manWayPoint = make_shared<ManWayPoint>();
    carAI = make_shared<CarAI>(glm::vec3(100.0f,20.0f,100.0f));

    auto components = manWayPoint->GetEntities()[3]->GetComponents();
    auto mapWaypoint = components.find(CompType::WayPointComp);
    auto cWayPoint = static_cast<CWayPoint*>(mapWaypoint->second.get());
    carAI->SetWayPoint(cWayPoint->position);


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

    physicsAI = make_shared<PhysicsAI>();
    

#pragma region FL


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
//    shared_ptr<selector> selector1 = make_shared<selector>();
//    shared_ptr<sequence> sequence1 = make_shared<sequence>();

//    shared_ptr<haveDoorOpen> puertaAbiertaSiNo = make_shared<haveDoorOpen>();
//    shared_ptr<haveKey> tengoLlaveSiNo = make_shared<haveKey>();
//    shared_ptr<openDoor>abrirPuerta = make_shared<openDoor>();
//    shared_ptr<getKey> cogerLlave = make_shared<getKey>();

//    shared_ptr<Minimum> tryCatchKey3 = make_shared<Minimum>();

//    selector1->addChild(puertaAbiertaSiNo);
//    selector1->addChild(sequence1);
//    selector1->addChild(tryCatchKey3);

//    sequence1->addChild(tengoLlaveSiNo);
//    sequence1->addChild(abrirPuerta);

//    tryCatchKey3->addChild(cogerLlave);

// 	cout << "--------------------" << endl;
//    while (door==false){
//        selector1->run();
//    } // If the operation starting from the root fails, keep trying until it succeeds.
// 	cout << "--------------------" << endl;
//

#pragma endregion

    for(shared_ptr<WayPoint> way : manWayPoint->GetEntities()){
        cout << "Vamos a crear mini puntos de control -> power ups de mientras" << endl;

        /* EJEMPLO AÑADIR EDGES */
        //way->AddEdge(2,300.0);
        //auto components = way->GetComponents();
        //auto mapEdges = components.find(CompType::WayPointEdgesComp);
        //auto cEdges = static_cast<CWayPointEdges>(mapEdges->second.get());
        //cout << cEdges->edges[0].cost << endl;

        auto components = way->GetComponents();
        auto mapWaypoint = components.find(CompType::WayPointComp);
        auto cWayPoint = static_cast<CWayPoint*>(mapWaypoint->second.get());


        manPowerUps->CreatePowerUp(glm::vec3(cWayPoint->position));
    }
    //cout << "el tamanyo normal es: " << manWayPoint.size() << endl;


    renderEngine->FacadeAddObject(car.get());
    renderEngine->FacadeAddObject(ground.get());
    renderEngine->FacadeAddObject(carAI.get());
    
    for(shared_ptr<PowerUp> pu : manPowerUps->GetEntities()) 
        renderEngine->FacadeAddObject(pu.get());
        
    renderEngine->FacadeAddCamera(cam.get());

    lastFPS = -1;
    then = renderEngine->FacadeGetTime();


    //inicializamos las reglas del cocheIA de velocidad/aceleracion
    //FuzzyLogic flVelocity;
    physicsAI->InitPhysicsIA(carAI.get());
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

    physicsAI->Update(manWayPoint->GetEntities() , carAI.get(), frameDeltaTime);
    inputEngine->CheckInputs(*car);
    renderEngine->FacadeCheckInput(frameDeltaTime,car.get(),cam.get());
    physicsEngine->UpdateCar(car.get(), cam.get());
    physicsEngine->UpdateCarAI(carAI.get());

    

    renderEngine->FacadeDraw();


    int fps = renderEngine->FacadeGetFPS();
    lastFPS = fps;
}