#include "StateInGame.h"
#include <chrono>
#include <iostream>
#include <numeric>

#include "../Components/CTransformable.h"

typedef std::chrono::high_resolution_clock Clock;

using namespace std;
using namespace chrono;

#pragma region BT
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

#pragma endregion

StateInGame::StateInGame() {
    // constructor
    deltaTime = make_shared<float>(1.2);
    deltas.push_back(1);
    deltas.push_back(1);
    deltas.push_back(1);
    deltas.push_back(1);
    deltas.push_back(1);
    cout << "Hemos inicializado el stateInGame" << endl;
    physics = make_unique<Physics>(deltaTime.get());

    eventManager = EventManager::GetInstance();

    manPowerUps = make_shared<ManPowerUp>();
    ground = make_shared<GameObject>(glm::vec3(10.0f, 10.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 1.0f, 100.0f), "wall.jpg", "ninja.b3d");
    cam = make_shared<Camera>(glm::vec3(10.0f, 40.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    carAI = make_shared<CarAI>(glm::vec3(100.0f, 20.0f, 100.0f));

    manWayPoint = make_shared<ManWayPoint>();
    manWayPoint->CreateWayPoint(glm::vec3(-10.0f, 25.0f, -150.0f), 0, 0);
    manWayPoint->CreateWayPoint(glm::vec3(150.0f, 25.0f, -150.0f), 0, 0);
    manWayPoint->CreateWayPoint(glm::vec3(150.0f, 25.0f, 150.0f), 0, 0);
    manWayPoint->CreateWayPoint(glm::vec3(-150.0f, 25.0f, 150.0f), 0, 0);

    //Le asignamos el waypoint inicial, momentaneo
    auto cWayPoint = static_cast<CWayPoint*>(manWayPoint->GetEntities()[3]->GetComponent(CompType::WayPointComp).get());
    carAI->SetWayPoint(cWayPoint->position);

    manCars = make_shared<ManCar>(physics.get(), cam.get());

    // Inicializamos las facadas
    renderFacadeManager = RenderFacadeManager::GetInstance();
    renderFacadeManager->InitializeIrrlicht();

    inputFacadeManager = InputFacadeManager::GetInstance();
    inputFacadeManager->InitializeIrrlicht();

    physicsFacadeManager = PhysicsFacadeManager::GetInstance();
    physicsFacadeManager->InitializeIrrlicht();

    //Almacenamos los motores
    renderEngine = renderFacadeManager->GetRenderFacade();
    inputEngine = inputFacadeManager->GetInputFacade();
    physicsEngine = physicsFacadeManager->GetPhysicsFacade();

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

    //Posicionamos todos los powerups donde hay waypoints, momentaneo
    for (shared_ptr<WayPoint> way : manWayPoint->GetEntities()) {
        cout << "Vamos a crear mini puntos de control -> power ups de mientras" << endl;

        /* EJEMPLO AÑADIR EDGES */
        //way->AddEdge(2,300.0);
        //auto components = way->GetComponents();
        //auto mapEdges = components.find(CompType::WayPointEdgesComp);
        //auto cEdges = static_cast<CWayPointEdges>(mapEdges->second.get());
        //cout << cEdges->edges[0].cost << endl;

        auto cWayPoint = static_cast<CWayPoint*>(way->GetComponent(CompType::WayPointComp).get());

        manPowerUps->CreatePowerUp(glm::vec3(cWayPoint->position));
    }

    renderEngine->FacadeAddObjectCar(manCars.get()->GetCar().get());  //Añadimos el coche
    renderEngine->FacadeAddObject(ground.get());                      //Añadimos el suelo

    //Añadimos todos los power ups
    for (shared_ptr<Entity> pu : manPowerUps->GetEntities())
        renderEngine->FacadeAddObject(pu.get());

    renderEngine->FacadeAddObject(carAI.get());
    renderEngine->FacadeAddCamera(cam.get());

    lastFPS = -1;
    //then = renderEngine->FacadeGetTime();
    then = system_clock::now();

    //inicializamos las reglas del cocheIA de velocidad/aceleracion
    //FuzzyLogic flVelocity;
    physicsAI->InitPhysicsIA(carAI.get());
}

StateInGame::~StateInGame() {
    // destructor
}

void StateInGame::InitState() {
    soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    soundEngine->LoadBanks(2);
}

void StateInGame::Input() {
    renderEngine->FacadeCheckInput();
}

void StateInGame::Update() {
    eventManager->Update();

    // actualizamos el deltatime
    time_point<system_clock> now = system_clock::now();
    int64_t milis = duration_cast<milliseconds>(now - then).count();
    //const uint32_t now = renderEngine->FacadeGetTime();

    // con media
    float currentDelta = (float)(milis) / 100.0;
    *deltaTime.get() = CalculateDelta(currentDelta);

    // sin media
    // *deltaTime.get() = (float)(milis) / 100.0;

    then = now;

    physicsAI->Update(manWayPoint->GetEntities(), carAI.get(), *deltaTime.get());
    renderEngine->UpdateCamera(cam.get());
    physicsEngine->UpdateCar(manCars.get()->GetCar().get(), cam.get());
    physicsEngine->UpdateCarAI(carAI.get());
    //physicsEngine->UpdateCar(car.get(), cam.get());

    //renderEngine->FacadeDraw();

    int fps = renderEngine->FacadeGetFPS();
    lastFPS = fps;
}

void StateInGame::Render() {
    renderEngine->FacadeDraw();
}

float StateInGame::CalculateDelta(float currentDelta) {
    deltas.push_back(currentDelta);  // añadimos uno
    deltas.erase(deltas.begin());    // borramos el primero

    // hace la media de las últimas 5 deltas
    return accumulate(deltas.begin(), deltas.end(), 0.0) / deltas.size();
}