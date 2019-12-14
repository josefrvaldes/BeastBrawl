#include "StateInGame.h"
#include <chrono>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <limits.h>

#include "../Components/CTransformable.h"
#include "../Components/CWayPointEdges.h"

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



// int minDistance(float dist[], bool sptSet[], int size){
//     int min = INT_MAX, min_index;

//     for(int v = 0; v<size; v++){
//         if(sptSet[v] == false && dist[v] <= min){
//             min = dist[v];
//             min_index = v;
//         }
//     }

//     return min_index;
// }

// void printPath(int parent[], int j){
//     if(parent[j] == -1){
//         return;
//     }
//     printPath(parent,parent[j]);

//     cout << j;
// }

// void printSolution(float dist[], int size, int parent[], int src){
//     cout << "Vertex\t Distance\t Path";
//     for(int i = 0; i < size; i++){
//         cout << "\n" << src << " -> " << i << "\t\t" << dist[i] << "\t" << src;
//         printPath(parent,i);
//     }
// }

// void StateInGame::Dijkstra(ManWayPoint* _graph, int start, int end){
//     cout << "----------------------------------\n";
//     //Convertir ManWayPoint en una matriz de adyacencia
//     int size = _graph->GetEntities().size();
//     float graph[size][size];

//     //Distancia desde start hasta todos los nodos
//     float dist[size];

//     //sptSet[i] sera true si el vertice i esta incluido en el arbol de minima distancia
//     bool sptSet[size];

//     //Arbol de caminos mas cortos
//     int parent[size];

//     //Rellenamos de 0 el grafo
//     for(int i = 0; i < size; ++i){
//         for(int j = 0; j < size; ++j){
//             graph[i][j] = 0;
//         }
//     }

//     //Ponemos los costes pertinentes en la matriz de adyacencia
//     for(auto node : _graph->GetEntities()){
//         auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
//         auto cWayPointEdges = static_cast<CWayPointEdges*>(node->GetComponent(CompType::WayPointEdgesComp).get());

//         for(auto edge : cWayPointEdges->edges){
//             graph[cWayPoint->id][edge.to] = edge.cost;
//         }
//     }

//     //Dibujamos el grafo
//     for(int i = 0; i < size; ++i){
//         for(int j = 0; j < size; ++j){
//             cout << (int)graph[i][j] << "\t";
//         }
//         cout <<endl;
//     }

//     //Inicializamos todas las distancias como infinito y stpSet a falso
//     for(int i = 0; i < size; ++i){
//         parent[start] = -1;
//         dist[i] = INT_MAX;
//         sptSet[i] = false;
//     }

//     //Distancia a si mismo es 0
//     dist[start] = 0;

//     //Encontramos el minimo path para todos los nodos
//     for(int count = 0; count < size-1; ++count){
//         //Cogemos el nodo de minima distancia del array de los que no han sido procesados
//         //En la primera iteracion "u" siempre es igual a start
//         int u = minDistance(dist,sptSet,size);

//         //Ponemos el nodo seleccionado como marcado
//         sptSet[u] = true;

//         //Actualizamos los valores de dist de los nodos adyacentes del nodo escogido
//         for(int v = 0; v < size; ++v){
//             //Actualizamos dist[v] solo si no esta ya en el array sptSet de marcados,
//             //si hay un edge desde u a v y si el peso total del path desde start a v por u es menor que el actual en dist[v]

//             if(!sptSet[v] && graph[u][v] && (dist[u] + graph[u][v]) < dist[v]){
//                 parent[v] = u;
//                 dist[v] = dist[u] + graph[u][v];
//             }
//         }
//     }

//     printSolution(dist, size, parent, start);
//     cout << "\n";

// }


void StateInGame::Dijkstra(ManWayPoint* _graph, int start, int end) {


    cout << "----------------------------------\n";
    //Convertir ManWayPoint en una matriz de adyacencia
    int size = _graph->GetEntities().size();
    float graph[size][size];

    //Rellenamos de 0 el grafo
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            graph[i][j] = INT_MAX;
        }
    }

    //Ponemos los costes pertinentes en la matriz de adyacencia
    for(auto node : _graph->GetEntities()){
        auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdges = static_cast<CWayPointEdges*>(node->GetComponent(CompType::WayPointEdgesComp).get());

        for(auto edge : cWayPointEdges->edges){
            graph[cWayPoint->id][edge.to] = edge.cost;
        }
    }

    //Dibujamos el grafo
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            cout << (int)graph[i][j] << "\t";
        }
        cout <<endl;
    }

    //Comenzamos Dijkstra
    float distance[size],pred[size];
    int visited[size],count,minDistance,nextNode,i,j;


    for(i=0;i<size;i++) {
        distance[i] = graph[start][i];  //Metemos las ponderaciones a los nodos desde el que iniciamos(Si no tiene es = INT_MAX)
        pred[i] = start;                // 
        visited[i] = 0;
    }

    //La distancia a si mismo es siempre 0
    distance[start]=0; 
    visited[start]=1;
    count=1;

    while(count<size-1) {
        minDistance=INT_MAX;

        for(i=0;i<size;i++){
            if(distance[i] < minDistance && !visited[i]) {
                //Si la distancia al nodo i es menor que la minDistance y no esta visitado
                //Recordatorio: Si nuestro nodo start no esta conectado con i entonces distance[1] = INT_MAX y no entrará aquí
                minDistance=distance[i]; // Distancia al nodo adyacente mas cercano
                nextNode=i; //Siguiente nodo adyacente mas cercano
            }
        }

        visited[nextNode]=1;

        for(i=0;i<size;i++){
            if(!visited[i]){
                //Si la distancia entre (start y nodo i) es mayor que 
                if(minDistance + graph[nextNode][i] < distance[i]) {
                    distance[i]=minDistance + graph[nextNode][i];
                    pred[i]=nextNode;
                }
            }
        }

        //if(visited[end]==1) count = size;
        count++;
    }

    for(i=0;i<size;i++){

        if(i!=start){

            cout<<"\nDistance of node "<<i<<"= "<<distance[i];
            cout<<"\nPath = "<<i;
            j=i;
            do{
                j=pred[j];
                cout<<" <- "<<j;
            }while(j!=start);
        }
    }

    cout << "\n\n\n";
}

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

    //COMPROBAMOS DIJKSTRA
    Dijkstra(manWayPoint.get(),11,1);

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