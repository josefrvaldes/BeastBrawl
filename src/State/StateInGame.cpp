#include "StateInGame.h"
#include <chrono>
#include <iostream>
#include <numeric>

#include "../Components/CTransformable.h"
#include "../Components/CWayPointEdges.h"

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
    phisicsPowerUp = make_shared<PhysicsPowerUp>();
    manBoxPowerUps = make_shared<ManBoxPowerUp>();
    ground = make_shared<GameObject>(glm::vec3(10.0f, 10.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 1.0f, 100.0f), "wall.jpg", "ninja.b3d");
    cam = make_shared<Camera>(glm::vec3(10.0f, 40.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    manWayPoint = make_shared<ManWayPoint>();

    auto cWayPoint = static_cast<CWayPoint*>(manWayPoint->GetEntities()[3]->GetComponent(CompType::WayPointComp).get());


    manCars = make_shared<ManCar>(physics.get(), cam.get());

    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars->CreateCarAI(glm::vec3(100.0f, 20.0f, 100.0f), cWayPoint->position);

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

        manBoxPowerUps->CreateBoxPowerUp(glm::vec3(cWayPoint->position));
    }
    //cout << "el tamanyo normal es: " << manWayPoint.size() << endl;
    //Añadimos todos los power ups
    for (shared_ptr<Entity> bpu : manBoxPowerUps->GetEntities())
        renderEngine->FacadeAddObject(bpu.get());

    renderEngine->FacadeAddCamera(cam.get());


    // CREAMOS EL TOTEM
    totem = make_shared<Totem>();
    renderEngine->FacadeAddObject(totem.get());

    lastFPS = -1;
    //then = renderEngine->FacadeGetTime();
    then = system_clock::now();

    //inicializamos las reglas del cocheIA de velocidad/aceleracion
    //FuzzyLogic flVelocity;
    physicsAI->InitPhysicsIA(manCars->GetEntitiesAI()[0].get());  // To-Do: hacer que se le pasen todos los coches IA
    cout << "después de init physics ai" << endl;
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
    time_point<system_clock> now = system_clock::now();
    int64_t milis = duration_cast<milliseconds>(now - then).count();
    //const uint32_t now = renderEngine->FacadeGetTime();

    // con media
    float currentDelta = (float)(milis) / 100.0;
    *deltaTime.get() = CalculateDelta(currentDelta);

    // sin media
    // *deltaTime.get() = (float)(milis) / 100.0;

    then = now;

    physics->update(manCars->GetCar().get(), cam.get());
    physicsAI->Update(manWayPoint->GetEntities(), manCars->GetEntitiesAI()[0].get(), *deltaTime.get());
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

    // COGER POWERUPS - DE MOMENTO SOLO CON EL PLAYER
    //for(shared_ptr<Entity> actualCar : manCars->GetEntities()){                                                                   // recorremos todos los coches
        auto cPowerUpCar = static_cast<CPowerUp*>(manCars.get()->GetCar().get()->GetComponent(CompType::PowerUpComp).get());        // debemos acceder al componente PowerUpComp
        if(cPowerUpCar->typePowerUp == typeCPowerUp::None){                                                                         // solo si no tenemos powerUp podemos coger uno
            for(shared_ptr<Entity> actualBoxPowerUp: manBoxPowerUps->GetEntities()){                                                // recorremos los powerUps
                auto cBoxPowerUp = static_cast<CBoxPowerUp*>(actualBoxPowerUp.get()->GetComponent(CompType::BoxPowerUpComp).get()); // debemos acceder al componente BoxPowerUp
                if(cBoxPowerUp->active == true){                                                                                    // Vemos si efectivamente esta activo o no, para poder cogerlo
                     if( collisions->Intersects(manCars.get()->GetCar().get(), actualBoxPowerUp.get()) ){                           // Finalmente comprobamos las colisiones entre el coche y el powerUp
                        //std::cout << "HAY COLISION ENTRE COCHE Y POWERUP" << std::endl;
                        DataMap dataCollisonCarBoxPowerUp;                                                                             // Mejor definirlo en el .h
                        dataCollisonCarBoxPowerUp["BoxPowerUpComp"] = cBoxPowerUp;                                                     // necesitamos el componente
                        dataCollisonCarBoxPowerUp["actualBox"] = actualBoxPowerUp;                                                     // y tambien la caja actual (para eliminarla de irrlicht)
                        eventManager->AddEventMulti(Event{EventType::PRESS_C, dataCollisonCarBoxPowerUp});                             // llamamos al evento --- COMO ODIO QUE SE LLAME ADD Y NO TARGET
                    }
                }
            }
        }
    //}
    //collisions->IntersectPlayerPowerUps(manCars->GetCar().get(), manPowerUps->GetEntities());
    // llamamos a comprobar las colisiones entre los coches (actualmente solo el prota) y los powerUps lanzados

    // ELIMINAMOS POWERUPS - DE MOMENTO SOLO CON EL PLAYER
    //for(shared_ptr<Entity> actualCar : manCars->GetEntities()){   
        for(shared_ptr<Entity> actualPowerUp : manPowerUps->GetEntities()){
            auto cPowerUp = static_cast<CPowerUp*>(actualPowerUp->GetComponent(CompType::PowerUpComp).get());
            if(cPowerUp->effectActive == true){                                                                 // SI HACE DANYO
                if(collisions->Intersects(manCars.get()->GetCar().get(), actualPowerUp.get())){   //TRUE
                    // debemos eliminar el powerUp y hacer danyo al jugador
                    DataMap dataCollisonCarPowerUp;                                                                           
                    dataCollisonCarPowerUp["PowerUp"] = actualPowerUp;              // nos guardamos el puntero para eliminar el powerUp                                             
                    eventManager->AddEventMulti(Event{EventType::COLLISION_ENTITY_POWERUP, dataCollisonCarPowerUp}); 
                }
            }
        }
    //}


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
    renderEngine->FacadeEndScene();
    int fps = renderEngine->FacadeGetFPS();
    lastFPS = fps;
}

float StateInGame::CalculateDelta(float currentDelta) {
    deltas.push_back(currentDelta);  // añadimos uno
    deltas.erase(deltas.begin());    // borramos el primero

    // hace la media de las últimas 5 deltas
    return accumulate(deltas.begin(), deltas.end(), 0.0) / deltas.size();
}





/*


#pragma region BT

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


*/