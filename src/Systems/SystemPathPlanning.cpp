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
 
 
 #include "SystemPathPlanning.h"

#include <Entities/CarHuman.h>
#include <Entities/CarAI.h>

#include <EventManager/Event.h>
#include <EventManager/EventManager.h>

#include <Components/CBrainAI.h>
#include <Components/CWayPointEdges.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CNavMesh.h>
#include <Managers/ManNavMesh.h>


#include <Facade/Render/RenderFacadeManager.h>
#include <Game.h>

#include <stack>
#include <iostream>
#include <climits>
#include <algorithm>
#include <cstdlib>     /* srand, rand */




SystemPathPlanning::SystemPathPlanning(){
    SubscribeToEvents();
}

void SystemPathPlanning::AddManager(Manager &m) {
    managers.push_back(&m);
}


void SystemPathPlanning::SubscribeToEvents() {

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::CALCULATE_PATH_TO_NAVMESH,
        bind(&SystemPathPlanning::CalculatePathToNavMesh, this, placeholders::_1),
        "CalculatePathToNavMesh"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::CHANGE_DESTINATION,
        bind(&SystemPathPlanning::ChangePosDestination, this, placeholders::_1),
        "ChangePosDestination"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::MOVE_RANDOM_POWERUP,
        bind(&SystemPathPlanning::MoveRandomPowerUp, this, placeholders::_1),
        "MoveRandomPowerUp"));

}

void SystemPathPlanning::MoveRandomPowerUp(DataMap* data){
    //std::cout << " -entramoooos o que beibeeeeeeee ???????????????????????\n";
    auto carAI = any_cast<CarAI*>((*data)[ACTUAL_CAR]);
    auto graph = static_cast<ManWayPoint*>(managers[4]);
    auto manNavMesh = static_cast<ManNavMesh*>(managers[5]);
    
    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carAI->GetComponent(CompType::CurrentNavMeshComp).get());
    auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());
    //auto cTransformableCar = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());

    // Finalmente cogemos un camino de un nodo a otro cualquiera del mapa
    if(cBrainAI->stackPath.empty()){
        //cout << "PATH ALEATORIO: "; 
        // cogemos el primer nodo (nodo de referencia de nuestro NavMesh)
        auto navMesh = manNavMesh->GetEntities()[cCurrentNavMesh->currentNavMesh]; //NavMesh en el que esta el coche
        auto cNavMesh = static_cast<CNavMesh*>(navMesh->GetComponent(CompType::NavMeshComp).get());
        int wayPointReference = -1;  //ID del waypoint mas cercano
        for(auto& waypointID : cNavMesh->waypoints){
            auto waypoint = graph->GetEntities()[waypointID]; //Cogemos el waypoint del manager de waypoints
            auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());
            if(cWaypoint->type == 2){
                wayPointReference = waypointID;
            }
        }
        //cout << " referencia es: " << wayPointReference;

        int wayPointDestination = -1;
        do{
            auto waypoint = graph->GetEntities()[rand() % graph->GetEntities().size()]; //Cogemos el waypoint del manager de waypoints
            auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());
            if(cWaypoint->type == 1){
                wayPointDestination = cWaypoint->id;
                //std::cout << "El indice nuevo es:  " << wayPointDestination << std::endl;
            }  
        }while(wayPointDestination == -1);

        //cout << " destino es: " << wayPointDestination << endl;


        //COMPROBAMOS DIJKSTRA
        auto path = Dijkstra(graph,wayPointReference,wayPointDestination);
        carAI->SetPath(path);

        //auto path2 = path;
        //while(!path2.empty()){
        //std::cout << "nodo del path x= " << path2.top() << std::endl;
        //    path2.pop();
        //}

        auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[path.top()]->GetComponent(CompType::WayPointComp).get());
        carAI->SetWayPoint(cWayPoint);

        auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
        cPosDestination->position = cWayPoint->position;
        carAI->SetDestination(cPosDestination);

    }       
}

void SystemPathPlanning::CalculatePathToNavMesh(DataMap* data){
    //static_cast<ManWayPoint*>(managers[4]), static_cast<ManNavMesh*>(managers[5])
    auto graph = static_cast<ManWayPoint*>(managers[4]);
    auto manNavMesh = static_cast<ManNavMesh*>(managers[5]);
    auto carAI = any_cast<CarAI*>((*data)[ACTUAL_CAR]);
    auto targetNavMesh = any_cast<int>((*data)[ID_DESTINATION]);
    auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());
    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carAI->GetComponent(CompType::CurrentNavMeshComp).get());
    //cout << "Mi NavMesh actual es el: " << cCurrentNavMesh->currentNavMesh << endl;
    // auto cPosDestination = static_cast<CPosDestination*>(any_cast<CarAI*>(data[ACTUAL_CAR])->GetComponent(CompType::PosDestination).get());

    //Vaciamos el Path
    while(!cBrainAI->stackPath.empty()){
        cBrainAI->stackPath.pop();
    }

    //cout << "PATH TO NAVMESH: ";

    //Buscamos el waypoint de referencia == 2
    auto navMesh = manNavMesh->GetEntities()[cCurrentNavMesh->currentNavMesh]; //NavMesh en el que esta el coche
    auto cNavMesh = static_cast<CNavMesh*>(navMesh->GetComponent(CompType::NavMeshComp).get());
    // Cogemos el wayPoint de tipo 2, que es el de referencia
    int wayPointReference = -1;  //ID del waypoint mas cercano
    for(auto& waypointID : cNavMesh->waypoints){
        auto waypoint = graph->GetEntities()[waypointID]; //Cogemos el waypoint del manager de waypoints
        auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());
        if(cWaypoint->type == 2){
            wayPointReference = waypointID;
        }
    }
    //cout << " referencia es: " << wayPointReference;

    auto cNavMeshTarget = static_cast<CNavMesh*>(manNavMesh->GetEntities()[targetNavMesh]->GetComponent(CompType::NavMeshComp).get()); //Cogemos el componente NavMesh del TargetNavMesh
    // recorremos todos los waypoints del NavMesh al que vamos
    int wayPointDestination = -1;
    //std::cout << "vamos a entrar al for para coger el waypoint destino" << std::endl;
    for(auto& waypointID : cNavMeshTarget->waypoints){
        auto waypoint = graph->GetEntities()[waypointID]; //Cogemos el waypoint del manager de waypoints
        auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());
        if(cWaypoint->type == 2){
            wayPointDestination = waypointID;
        }  
    }
    //cout << " destino es: " << wayPointDestination << endl;
    //int waypointTargetNavMesh = cNavMeshTarget->waypoints.at(wayPointDestination);
    //COMPROBAMOS DIJKSTRA
    auto path = Dijkstra(graph,wayPointReference,wayPointDestination);
    carAI->SetPath(path);

    auto path2 = path;
    while(!path2.empty()){
        //std::cout << "nodo del path x= " << path2.top() << std::endl;
        path2.pop();
    }


    auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[path.top()]->GetComponent(CompType::WayPointComp).get());
    carAI->SetWayPoint(cWayPoint);

    auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
    cPosDestination->position = cWayPoint->position;
    carAI->SetDestination(cPosDestination);
}

void SystemPathPlanning::ChangePosDestination(DataMap* data){
    auto carAI = any_cast<CarAI*>((*data)[ACTUAL_CAR]); 
    auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
    cPosDestination->position = any_cast<glm::vec3>((*data)[POS_DESTINATION]);
    cPosDestination->radious = 1.0f;
    carAI->SetDestination(cPosDestination);

    // ya que ponemos posicion fija, limpiamos el path
    auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());
    while(!cBrainAI->stackPath.empty()){
        cBrainAI->stackPath.pop();
    }
    // siempre que se cambia el destino el target va fuera
    cBrainAI->targetNavMesh = -1;
    //CleanBrainAI(carAI);
}

void SystemPathPlanning::update(CarAI* carAI){
    UpdateDijkstra(carAI, static_cast<ManWayPoint*>(managers[4]), static_cast<ManNavMesh*>(managers[5]));    
}

void SystemPathPlanning::UpdateDijkstra(CarAI* carAI, ManWayPoint* graph, ManNavMesh* manNavMesh){
    //Guardamos en varAIbles los componentes
	auto cTransformable = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());
    auto cPosDestination     = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
    float radious = 10.0f;
    if(cPosDestination->radious >=10)
        radious = cPosDestination->radious;

    //Vamos a comprobar si esta en el rango del waypoint
    if((cPosDestination->position.z - radious) < cTransformable->position.z && (cPosDestination->position.z + radious) >= cTransformable->position.z 
        && (cPosDestination->position.x - radious) < cTransformable->position.x && (cPosDestination->position.x + radious) >= cTransformable->position.x
        && (cPosDestination->position.y - radious - 10.0) < cTransformable->position.y && (cPosDestination->position.y + radious + 10.0) >= cTransformable->position.y){
        //Tenemos que comprobar si le quedan mas nodos que visitar en el path
        auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());

        if(!cBrainAI->stackPath.empty()){
            //auto actualNode = cBrainAI->stackPath.top(); //Id del waypoint en el que estamos
            cBrainAI->stackPath.pop();
            if(!cBrainAI->stackPath.empty()){
                auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[cBrainAI->stackPath.top()]->GetComponent(CompType::WayPointComp).get());
                cPosDestination->position = cWayPoint->position;
                carAI->SetDestination(cPosDestination);
            }else{
                this->CleanBrainAI(carAI);
            }
        }else{
            this->CleanBrainAI(carAI);
        }
    }
}

void SystemPathPlanning::CleanBrainAI(CarAI* carAI){
    auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());
    cBrainAI->targetCar = nullptr;
    cBrainAI->targetBoxPowerUp = nullptr;
}


void SystemPathPlanning::InitMapGraph(ManWayPoint* _graph){
    graphSize = _graph->GetEntities().size();
    graph = new float*[graphSize];

    for(uint16_t i = 0; i <graphSize; ++i){
        graph[i] = new float[graphSize];
    }

    //Rellenamos de 0 el grafo
    for(uint16_t i = 0; i<graphSize; ++i){
        for(uint16_t j = 0; j<graphSize; ++j){
            graph[i][j] = INT_MAX;
        }
    }

    //Rellenamos el graph con los waypoints
    for(auto& node : _graph->GetEntities()){
        auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdges = static_cast<CWayPointEdges*>(node->GetComponent(CompType::WayPointEdgesComp).get());

        for(auto& edge : cWayPointEdges->edges){
            graph[cWayPoint->id][edge.to] = edge.cost;
        }

    }

    graphCreated = true;
}

std::stack<int> SystemPathPlanning::Dijkstra(ManWayPoint* _graph, const uint16_t start, const uint16_t end) {
    //cout << "----------------------------------\n";
    //Convertir ManWayPoint en una matriz de adyacencia
    if(!graphCreated)
        InitMapGraph(_graph);
    

    //Ponemos los costes pertinentes en la matriz de adyacencia
    //TODO: Cambiar esto para tenerlo guardado en una entidad o algo y no hacerlo cada calculo de Dijkstra
    for(auto& node : _graph->GetEntities()){
        auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdges = static_cast<CWayPointEdges*>(node->GetComponent(CompType::WayPointEdgesComp).get());

        for(auto& edge : cWayPointEdges->edges){
            if(cWayPoint->type == 2){   // si el tipo es 2 significa que el nodo es nodo de referencia... no es lo mas conveniente ir ahi
                graph[cWayPoint->id][edge.to] = edge.cost*2.0;
            }else{
                graph[cWayPoint->id][edge.to] = edge.cost;
            }
        }
    }

    //Comenzamos Dijkstra
    float distanceFromStart[graphSize],pred[graphSize];
    int visited[graphSize],count,minDistanceFromStart,nextClosestNode = -1,i;

    for(i=0;i<graphSize;i++) {
        distanceFromStart[i] = graph[start][i];  //Metemos las ponderaciones a los nodos desde el que iniciamos(Si no tiene es = INT_MAX)
        pred[i] = start;                
        visited[i] = 0;
    }

    //La distancia a si mismo es siempre 0
    distanceFromStart[start]=0; 
    visited[start]=1;
    count=1;

    while(count<graphSize-1) {
        minDistanceFromStart=INT_MAX;
        for(i=0;i<graphSize;i++){
            if(distanceFromStart[i] < minDistanceFromStart && !visited[i]) {
                //Si la distancia al nodo i es menor que la minDistanceFromStart y no esta visitado
                //Recordatorio: Si nuestro nodo start no esta conectado con i entonces distanceFromStart[1] = INT_MAX y no entrará aquí
                minDistanceFromStart=distanceFromStart[i]; // Distancia al nodo adyacente mas cercano
                nextClosestNode=i; //Siguiente nodo adyacente mas cercano
            }
        }

        visited[nextClosestNode]=1;

        for(i=0;i<graphSize;i++){
            if(!visited[i]){
                //Si la distancia entre (start y nodo i) es mayor que (start y su nodo adyacente) + (su nodo adyacente hasta i)
                //P.E: ¿De 1 -> 3 es mayor que de 1 -> 2 -> 3?
                if(minDistanceFromStart + graph[nextClosestNode][i] < distanceFromStart[i]) {
                    distanceFromStart[i]=minDistanceFromStart + graph[nextClosestNode][i];
                    pred[i]=nextClosestNode; //Nos guardamos en pred[i] el nodo por el que mas rapido se llega a él (nextClosestNode)
                }
            }
        }
        count++;
    }

    stack<int> path;
    int aux = end;
    path.push(aux); //Para guardarnos el final del path
    //Recorremos pred recursivamente hasta que pred[aux] sea el nodo start
    while(aux!=start){
        if(pred[aux]==start) break; //Para que no nos añada el nodo start
        path.push(pred[aux]);
        aux = pred[aux];
    }
    return path;
}

// DOBLE FREE OR CORRUPTION
SystemPathPlanning::~SystemPathPlanning(){
    if(graph){
        for(int i = 0; i<graphSize; ++i){
            delete[] graph[i];
        }
        delete[] graph;
    }
}