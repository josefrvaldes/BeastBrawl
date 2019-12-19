#include "StateInGame.h"
#include <chrono>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <limits.h>
#include <algorithm>

#include "../Components/CTransformable.h"
#include "../Components/CWayPointEdges.h"
#include "../Components/CTotem.h"
#include "../Components/CPath.h"

typedef std::chrono::high_resolution_clock Clock;

using namespace std;
using namespace chrono;

#pragma region BT
/*
////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
bool door = false;  // imaginemos que door=false es = puerta cerrada "door is open?"
bool key = false;   // tenemos una llave
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
        return key;
    }
};
///// DECORATORS //////
struct Minimum : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
    uint32_t totalTries = 3;
    uint32_t numTries = 0;
    virtual bool run() override {
        if (numTries >= totalTries)
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
        if (numLimit >= totalLimit)
            return false;
        numLimit++;
        return getChild()->run();
    }
};
struct UntilFail : public Decorator {  // Decorator UntilFail
    virtual bool run() override {
        while (true) {
            bool result = getChild()->run();
            if (!result) {
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
*/
#pragma endregion


StateInGame::StateInGame() {
    
    physics = make_unique<Physics>(deltaTime);

    eventManager = EventManager::GetInstance();

    manPowerUps = make_shared<ManPowerUp>();
    phisicsPowerUp = make_shared<PhysicsPowerUp>();
    manBoxPowerUps = make_shared<ManBoxPowerUp>();
    manTotems = make_shared<ManTotem>();
    ground = make_shared<GameObject>(glm::vec3(10.0f, -0.5f, 150.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.5f, 1.5f, 1.5f), "", "training_ground.obj");
    cam = make_shared<Camera>(glm::vec3(10.0f, 40.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    manWayPoint = make_shared<ManWayPoint>(); //Se crean todos los waypoints y edges

    auto cWayPoint = static_cast<CWayPoint*>(manWayPoint->GetEntities()[2]->GetComponent(CompType::WayPointComp).get());


    manCars = make_shared<ManCar>(physics.get(), cam.get());
    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars->CreateCarAI(glm::vec3(50.0f, 20.0f, 100.0f), cWayPoint);
    stack<int> pathInit;
    pathInit.push(0);
    pathInit.push(1);
    pathInit.push(2);
    manCars->GetEntitiesAI()[0]->SetPath(pathInit);

    //auto cPath = static_cast<CPath*>(manCars->GetEntitiesAI()[0]->GetComponent(CompType::PathComp).get());
   // auto cActualPowerUp = static_cast<CPowerUp*>(manCars->GetEntitiesAI()[0]->GetComponent(CompType::PowerUpComp).get());
   // cActualPowerUp->typePowerUp = typeCPowerUp::TeleBanana;
    // while(!cPath->stackPath.empty()){
    //     auto node = cPath->stackPath.top();
    //     cPath->stackPath.pop();
    //     cout << node << " - ";
    // }

    auto cWayPointAI2 = static_cast<CWayPoint*>(manWayPoint->GetEntities()[1]->GetComponent(CompType::WayPointComp).get());
   //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars->CreateCarAI(glm::vec3(40.0f, 20.0f, 20.0f), cWayPointAI2);
    stack<int> pathInit2;
    pathInit2.push(4);
    pathInit2.push(0);
    pathInit2.push(2);
    manCars->GetEntitiesAI()[1]->SetPath(pathInit2);


    auto cWayPointAI3 = static_cast<CWayPoint*>(manWayPoint->GetEntities()[0]->GetComponent(CompType::WayPointComp).get());
   //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars->CreateCarAI(glm::vec3(0.0f, 20.0f, 0.0f), cWayPointAI3);
    stack<int> pathInit3;
    pathInit3.push(1);
    pathInit3.push(3);
    pathInit3.push(4);
    manCars->GetEntitiesAI()[2]->SetPath(pathInit3);

    // Inicializamos las facadas
    renderFacadeManager = RenderFacadeManager::GetInstance();
    //renderFacadeManager->InitializeIrrlicht();

    inputFacadeManager = InputFacadeManager::GetInstance();
    //inputFacadeManager->InitializeIrrlicht();

    physicsFacadeManager = PhysicsFacadeManager::GetInstance();
    //physicsFacadeManager->InitializeIrrlicht();

    //Almacenamos los motores
    renderEngine = renderFacadeManager->GetRenderFacade();
    inputEngine = inputFacadeManager->GetInputFacade();
    physicsEngine = physicsFacadeManager->GetPhysicsFacade();

    physicsAI = make_shared<PhysicsAI>();
    collisions = make_shared<Collisions>();

    sysBoxPowerUp = make_shared<SystemBoxPowerUp>();

    // manPowerUps->CreatePowerUp(glm::vec3(cWayPoint->position));

    renderEngine->FacadeAddObjectCar(manCars.get()->GetCar().get());  //Anyadimos el coche
    for (shared_ptr<Entity> carAI : manCars->GetEntitiesAI())         // Anyadimos los coche IA
        renderEngine->FacadeAddObject(carAI.get());
    renderEngine->FacadeAddObject(ground.get());  //Anyadimos el suelo

    for (shared_ptr<WayPoint> way : manWayPoint->GetEntities()) {
        auto components = way->GetComponents();
        auto mapWaypoint = components.find(CompType::WayPointComp);
        auto cWayPoint = static_cast<CWayPoint*>(mapWaypoint->second.get());

        // solo debemos crear las Box si el type del waypoint es "1"
        if(cWayPoint->type == 1){
            manBoxPowerUps->CreateBoxPowerUp(glm::vec3(cWayPoint->position));
        }
    }
    //cout << "el tamanyo normal es: " << manWayPoint.size() << endl;
    //Añadimos todos los power ups
    for (shared_ptr<Entity> bpu : manBoxPowerUps->GetEntities())
        renderEngine->FacadeAddObject(bpu.get());

    renderEngine->FacadeAddCamera(cam.get());

    //lastFPS = -1;
    // CREAMOS EL TOTEM
    //manTotems->CreateTotem(glm::vec3(0.0f,20.0f,0.0f));
    manTotems->CreateTotem();
    renderEngine->FacadeAddObject(manTotems->GetEntities()[0].get());

    //then = renderEngine->FacadeGetTime();
    //then = system_clock::now();

    //inicializamos las reglas del cocheIA de velocidad/aceleracion
    //FuzzyLogic flVelocity;
    physicsAI->InitPhysicsIA(manCars->GetEntitiesAI()[0].get());  // To-Do: hacer que se le pasen todos los coches IA
    cout << "después de init physics ai" << endl;

    // BehaivourTree
    systemBtPowerUp = make_shared<SystemBtPowerUp>();
    
    //systemBtPowerUp->update(manCars->GetEntitiesAI()[1].get(), manCars.get(), manPowerUps.get(), manBoxPowerUps.get(), manTotems.get(), manWayPoint.get());
    
}

StateInGame::~StateInGame() {
    // destructor
}

void StateInGame::InitState() {
    soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    soundEngine->SetState(2);
}

void StateInGame::Input() {
    renderEngine->FacadeCheckInput();
}

void StateInGame::Update() {
    eventManager->Update();

    // actualizamos el deltatime
    //time_point<system_clock> now = system_clock::now();
    //int64_t milis = duration_cast<milliseconds>(now - then).count();
    //const uint32_t now = renderEngine->FacadeGetTime();

    // con media
    //float currentDelta = (float)(milis) / 100.0;
    //*deltaTime.get() = CalculateDelta(currentDelta);

    // sin media
    // *deltaTime.get() = (float)(milis) / 100.0;

    // BEHAIVOUR TREE
    for(auto actualAI : manCars->GetEntitiesAI()){
        systemBtPowerUp->update(actualAI.get(), manCars.get(), manPowerUps.get(), manBoxPowerUps.get(), manTotems.get(), manWayPoint.get());
    }
   

    physics->update(manCars->GetCar().get(), cam.get());

    for(auto actualAI : manCars->GetEntitiesAI()){
        physicsAI->Update(manWayPoint.get(),actualAI.get(), deltaTime);
    }

    sysBoxPowerUp->update(manBoxPowerUps.get());
    phisicsPowerUp->update(manPowerUps->GetEntities());

    // COMPORBACION A PELO COLISIONES ENTRE COCHES-POWERUPS
    // para hacerlo sencillo - la colision siemre sera entre el coche del jugador y el powerUp 1

    renderEngine->UpdateCamera(cam.get());
    physicsEngine->UpdateCar(manCars.get()->GetCar().get(), cam.get());
    for (shared_ptr<Entity> carAI : manCars->GetEntitiesAI())  // actualizamos los coche IA
        physicsEngine->UpdateCarAI(carAI.get());

    for (shared_ptr<Entity> actualPowerUp : manPowerUps->GetEntities())  // actualizamos los powerUp en irrlich
        physicsEngine->UpdatePowerUps(actualPowerUp.get());

    //physicsEngine->UpdateCar(car.get(), cam.get());




    // COLISIONES entre BoxPowerUp y player
    collisions->IntersectPlayerBoxPowerUp(manCars.get()->GetCar().get(), manBoxPowerUps.get());
    // COLISIONES entre BoxPowerUp y IA                                                            
    collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES entre powerUp y player
    collisions->IntersectPlayerPowerUps(manCars.get()->GetCar().get(), manPowerUps.get());
    // COLISIONES entre powerUp y IA
    collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get());
    // COLISIONES entre el Player y el Totem
    collisions->IntersectPlayerTotem(manCars.get()->GetCar().get(), manTotems.get());
    // COLISIONES  entre la IA y el Totem
    collisions->IntersectCarsTotem(manCars.get(), manTotems.get());
}



void StateInGame::Render() {
    auto carAI = manCars->GetEntitiesAI()[0].get();
    bool isColliding = collisions->Intersects(manCars.get()->GetCar().get(), carAI);

    renderEngine->FacadeBeginScene();

    // renderEngine->FacadeDraw();  //Para dibujar primitivas debe ir entre el drawAll y el endScene
    renderEngine->FacadeDrawAll();
    renderEngine->FacadeDrawGraphEdges(manWayPoint.get());
    renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), isColliding);
    renderEngine->FacadeDrawBoundingBox(carAI, isColliding);

    for(auto actualPowerUp : manPowerUps->GetEntities())
        renderEngine->FacadeDrawBoundingBox(actualPowerUp.get(), false);

    renderEngine->FacadeEndScene();
    int fps = renderEngine->FacadeGetFPS();
}

/*


#pragma region BT

    // --------------------------- BEHAVIOR TREE ----------------------------------

    //BehaviorTree BASICO
    //                                  SELECTOR1
                                            //
            ///////////////////////////////////////////////////////////////////////////////////
            //                                      //                                       //
    // La pueta esta abierta?                     SEQUENCE                               DECORATOR (minimum) (3 intentos)
                                        ///////////////////////////////                      //
                                        //                          //                       //
    //                              // tengo llave?             //abrir puerta        // coger llave

    shared_ptr<selector> selector1 = make_shared<selector>();
    shared_ptr<sequence> sequence1 = make_shared<sequence>();

    shared_ptr<haveDoorOpen> puertaAbiertaSiNo = make_shared<haveDoorOpen>();
    shared_ptr<haveKey> tengoLlaveSiNo = make_shared<haveKey>();
    shared_ptr<openDoor> abrirPuerta = make_shared<openDoor>();
    shared_ptr<getKey> cogerLlave = make_shared<getKey>();

    shared_ptr<Minimum> tryCatchKey3 = make_shared<Minimum>();

    selector1->addChild(puertaAbiertaSiNo);
    selector1->addChild(sequence1);
    selector1->addChild(tryCatchKey3);

    sequence1->addChild(tengoLlaveSiNo);
    sequence1->addChild(abrirPuerta);

    tryCatchKey3->addChild(cogerLlave);

    cout << "--------------------" << endl;
    while (door == false) {
        selector1->run();
    }  // If the operation starting from the root fails, keep trying until it succeeds.
    cout << "--------------------" << endl;
    //

#pragma endregion


*/