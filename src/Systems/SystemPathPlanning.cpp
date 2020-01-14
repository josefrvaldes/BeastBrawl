#include "SystemPathPlanning.h"

#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Entities/WayPoint.h"

#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

#include "../Components/CPath.h"
#include "../Components/CWayPointEdges.h"
#include "../Components/CTargetNavMesh.h"
#include "../Components/CCurrentNavMesh.h"
#include "../Components/CNavMesh.h"
#include "../Components/CPosDestination.h"

#include "../Facade/Render/RenderFacadeManager.h"
#include "../Game.h"
#include "../Managers/Manager.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManWayPoint.h"
#include "../Managers/ManNavMesh.h"

#include <math.h>



SystemPathPlanning::SystemPathPlanning(){
    SubscribeToEvents();
}

void SystemPathPlanning::SubscribeToEvents() {

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CALCULATE_PATH_TO_NAVMESH,
        bind(&SystemPathPlanning::CalculatePathToNavMesh, this, placeholders::_1),
        "CalculatePathToNavMesh"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CHANGE_DESTINATION,
        bind(&SystemPathPlanning::ChangePosDestination, this, placeholders::_1),
        "ChangePosDestination"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::MOVE_TO_POWERUP,
        bind(&SystemPathPlanning::MoveRandomPowerUp, this, placeholders::_1),
        "MoveRandomPowerUp"));

}


void SystemPathPlanning::MoveRandomPowerUp(DataMap data){
    //std::cout << " -entramoooos o que beibeeeeeeee ???????????????????????\n";
    ManNavMesh* manNavMesh = any_cast<ManNavMesh*>(data["manNavMesh"]);
    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(any_cast<CarAI*>(data["actualCar"])->GetComponent(CompType::CurrentNavMeshComp).get());
    auto cPath = static_cast<CPath*>(any_cast<CarAI*>(data["actualCar"])->GetComponent(CompType::PathComp).get());
    ManWayPoint* graph = any_cast<ManWayPoint*>(data["manWayPoints"]);
    //auto cTransformableCar = static_cast<CTransformable*>(any_cast<CarAI*>(data["actualCar"])->GetComponent(CompType::TransformableComp).get());

    //Buscamos el waypoint mas cercano del navmesh en el que estamos
    auto navMesh = manNavMesh->GetEntities()[cCurrentNavMesh->currentNavMesh]; //NavMesh en el que esta el coche
    auto cNavMesh = static_cast<CNavMesh*>(navMesh->GetComponent(CompType::NavMeshComp).get());
    //std::cout << " -Que diceees tio!!!!!!!!!!!\n";
    // Cogemos el wayPoint de tipo 2, que es el de referencia
    int wayPointReference = -1;  //ID del waypoint mas cercano
    for(auto waypointID : cNavMesh->waypoints){
        auto waypoint = graph->GetEntities()[waypointID]; //Cogemos el waypoint del manager de waypoints
        auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());
        if(cWaypoint->type == 2){
            wayPointReference = waypointID;
        }
    }
    //std::cout << " -Todo bien Papi???????????'\n";
    // Finalmente cogemos un camino de un nodo a otro cualquiera del mapa
    if(cPath->stackPath.empty()){
        //Si esta vacio es que ha acabado el path y recalculamos otro
        int wayPointDestination = -1;
        do{
            auto waypoint = graph->GetEntities()[rand() % graph->GetEntities().size()]; //Cogemos el waypoint del manager de waypoints
            auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());
            if(cWaypoint->type == 1){
                wayPointDestination = cWaypoint->id;
                //std::cout << "El indice nuevo es:  " << wayPointDestination << std::endl;
            }  
        }while(wayPointDestination == -1);

        //std::cout << "salimos " << std::endl;

        //COMPROBAMOS DIJKSTRA
        auto path = Dijkstra(graph,wayPointReference,wayPointDestination);
        any_cast<CarAI*>(data["actualCar"])->SetPath(path);
        auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[path.top()]->GetComponent(CompType::WayPointComp).get());
        any_cast<CarAI*>(data["actualCar"])->SetWayPoint(cWayPoint);
    }       
    //std::cout << " -Eso parece querido mike'\n"; 
    // TODO: No dejar la posiicon destino vacia, asignarle la misma o algo
    // Asi con todo, siempre que se suelte algo comprobar que efectivamente esta dentro de un NavMesh
}





void SystemPathPlanning::ChangePosDestination(DataMap data){
    auto carAI = any_cast<CarAI*>(data["actualCar"]); 
    auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
    cPosDestination->position = any_cast<glm::vec3>(data["posDestination"]);
    carAI->SetDestination(cPosDestination);

    // ya que ponemos posicion fija, limpiamos el path
    auto cPath = static_cast<CPath*>(carAI->GetComponent(CompType::PathComp).get());
    while(!cPath->stackPath.empty()){
        cPath->stackPath.pop();
    }

}



void SystemPathPlanning::CalculatePathToNavMesh(DataMap data){

    //cout << "ENTRA A PATH TO NAVMESH\n";

    ManNavMesh* manNavMesh = any_cast<ManNavMesh*>(data["manNavMesh"]);
    ManWayPoint* graph = any_cast<ManWayPoint*>(data["manWayPoints"]);
    auto carAI = any_cast<CarAI*>(data["actualCar"]);
    auto cPath = static_cast<CPath*>(carAI->GetComponent(CompType::PathComp).get());
    //auto cTransformableCar = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());
    auto cTargetNavMesh = static_cast<CTargetNavMesh*>(carAI->GetComponent(CompType::TargetNavMeshComp).get());
    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carAI->GetComponent(CompType::CurrentNavMeshComp).get());
    // auto cPosDestination = static_cast<CPosDestination*>(any_cast<CarAI*>(data["actualCar"])->GetComponent(CompType::PosDestination).get());

    //Vaciamos el Path
    while(!cPath->stackPath.empty()){
        cPath->stackPath.pop();
    }

    //Buscamos el waypoint mas cercano del navmesh en el que estamos
    auto navMesh = manNavMesh->GetEntities()[cCurrentNavMesh->currentNavMesh]; //NavMesh en el que esta el coche
    auto cNavMesh = static_cast<CNavMesh*>(navMesh->GetComponent(CompType::NavMeshComp).get());

/*
    //Recorremos todos los waypoints del navmesh en el que estamos
    int closestWayPoint = -1;  //ID del waypoint mas cercano
    int minDistance = 999999;
    for(auto waypointID : cNavMesh->waypoints){
        auto waypoint = graph->GetEntities()[waypointID]; //Cogemos el waypoint del manager de waypoints
        auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());

        //Ahora hacemos distancia euclidea
        float disX = cWaypoint->position.x - cTransformableCar->position.x;
        float disY = cWaypoint->position.y - cTransformableCar->position.y;
        float disZ = cWaypoint->position.z - cTransformableCar->position.z;

        float distance = sqrt((disX*disX) + (disY*disY) + (disZ*disZ));

        if(distance<minDistance){
            closestWayPoint = waypointID;
            minDistance = distance;
        }
        
    }
*/
    // Cogemos el wayPoint de tipo 2, que es el de referencia
    int wayPointReference = -1;  //ID del waypoint mas cercano
    for(auto waypointID : cNavMesh->waypoints){
        auto waypoint = graph->GetEntities()[waypointID]; //Cogemos el waypoint del manager de waypoints
        auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());
        if(cWaypoint->type == 2){
            wayPointReference = waypointID;
        }
    }

    //TODO: Pillamos el waypoint de referencia que es una de type=2
    auto cNavMeshTarget = static_cast<CNavMesh*>(manNavMesh->GetEntities()[cTargetNavMesh->targetNavMesh]->GetComponent(CompType::NavMeshComp).get()); //Cogemos el componente NavMesh del TargetNavMesh
    // recorremos todos los waypoints del NavMesh al que vamos
    int wayPointDestination = -1;
    //std::cout << "vamos a entrar al for para coger el waypoint destino" << std::endl;
    for(auto waypointID : cNavMeshTarget->waypoints){
        auto waypoint = graph->GetEntities()[waypointID]; //Cogemos el waypoint del manager de waypoints
        auto cWaypoint = static_cast<CWayPoint*>(waypoint->GetComponent(CompType::WayPointComp).get());
        if(cWaypoint->type == 2){
            wayPointDestination = waypointID;
        }  
    }
    //int waypointTargetNavMesh = cNavMeshTarget->waypoints.at(wayPointDestination);
    //COMPROBAMOS DIJKSTRA
    auto path = Dijkstra(graph,wayPointReference,wayPointDestination);
    carAI->SetPath(path);

    auto path2 = path;
    while(!path2.empty()){
        std::cout << "nodo del path x= " << path2.top() << std::endl;
        path2.pop();
    }


    auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[path.top()]->GetComponent(CompType::WayPointComp).get());
    carAI->SetWayPoint(cWayPoint);

    auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
    cPosDestination->position = cWayPoint->position;
    carAI->SetDestination(cPosDestination);
}



void SystemPathPlanning::Update(CarAI* carAI, ManWayPoint* graph, ManNavMesh* manNavMesh){
    UpdateDijkstra(carAI, graph, manNavMesh);    
}

void SystemPathPlanning::UpdateDijkstra(CarAI* carAI, ManWayPoint* graph, ManNavMesh* manNavMesh){
    //Guardamos en varAIbles los componentes
	auto cTransformable = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());
    //auto cWayPoint     = static_cast<CWayPoint*>(carAI->GetComponent(CompType::WayPointComp).get());
    //float radious = cWayPoint->radious;
    auto cPosDestination     = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
    auto cCurrentNavMesh     = static_cast<CCurrentNavMesh*>(carAI->GetComponent(CompType::CurrentNavMeshComp).get());
    float radious = cPosDestination->radious;

    //Vamos a comprobar si esta en el rango del waypoint
    if((cPosDestination->position.z - radious) < cTransformable->position.z && (cPosDestination->position.z + radious) >= cTransformable->position.z 
        && (cPosDestination->position.x - radious) < cTransformable->position.x && (cPosDestination->position.x + radious) >= cTransformable->position.x){
        //Tenemos que comprobar si le quedan mas nodos que visitar en el path
        auto cPath = static_cast<CPath*>(carAI->GetComponent(CompType::PathComp).get());
        auto cTargetNavMesh = static_cast<CTargetNavMesh*>(carAI->GetComponent(CompType::TargetNavMeshComp).get());
        //cout << "Llegamos aun WayPoint!!!!!!!" << cCurrentNavMesh->currentNavMesh << endl;
        // TODO: Cada vez que un totem se caiga o se deje en el suelo, debemos indicarle su nuevo NavMesh

        // El path no puede estar vacio, cTargetNavMesh debe existir y debe ser diferente al currentNavMesh
        if(!cPath->stackPath.empty() && cTargetNavMesh->targetNavMesh >= 0){

            auto actualNode = cPath->stackPath.top(); //Id del waypoint en el que estamos
            std::cout << "El nodo actual es el: ' " << actualNode << " ' " << std::endl;
            //Recorremos todos los navmeshes
            bool changeNavMesh = false;
            for(auto navMesh : manNavMesh->GetEntities()){
                if(!changeNavMesh){         // como los comprobamos todos si cambiamos en la iteracion 2, en la 3 en adelante estara cambiado el current y podra cambiar otra vez
                    auto cNavMesh = static_cast<CNavMesh*>(navMesh->GetComponent(CompType::NavMeshComp).get());
                    //Recorremos todos los waypoints de cada navmesh
                    for(auto waypointId : cNavMesh->waypoints){
                        // Si el nodo actual es igual al waypoint del navmesh y mi current navmesh es diferente del navmesh con el que compruebo
                        //Entonces es que hemos cambiado de navmesh
                        if(actualNode == waypointId && cCurrentNavMesh->currentNavMesh != cNavMesh->id && !changeNavMesh){
                            cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                            //cTargetNavMesh->targetNavMesh = cNavMesh->id;
                            cout << "PathPlanning: CAMBIAMOS DE NAVMESH Ahora estamos en el navmesh numero: " << cCurrentNavMesh->currentNavMesh << endl;
                            changeNavMesh = true;
                        }
                    }
                }
            }

            cPath->stackPath.pop();
            //std::cout << "El siguiente nodo sera el: " << cPath->stackPath.top() << std::endl;
            //cout << "Llegamos al WayPoint: " << actualNode << endl;
            if(!cPath->stackPath.empty()){
                //Le asignamos el WayPoint siguiente del path (graph->GetEntities()[cPath->stackPath.top()])
                auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[cPath->stackPath.top()]->GetComponent(CompType::WayPointComp).get());
                cPosDestination->position = cWayPoint->position;

                carAI->SetDestination(cPosDestination);
            }

            auto navMeshDestination = manNavMesh->GetEntities()[cTargetNavMesh->targetNavMesh]; //NavMesh al que estamos yendo
            // Asi con todo, siempre que se suelte algo comprobar que efectivamente esta dentro de un NavMesh
            // Comprobamos si ha llegado a algun waypoint del TargetNavMesh
            auto cNavMesh = static_cast<CNavMesh*>(navMeshDestination->GetComponent(CompType::NavMeshComp).get());
            for(auto waypointId : cNavMesh->waypoints){
                if(waypointId==actualNode){
                    //cout << "PahtPalnning: La IA ha llegado al navmesh destino\n";
                    //Vaciamos el Path
                    while(!cPath->stackPath.empty()){
                        //std::cout << "limpiamos el path que hemos llegado al NavMesh destino" << std::endl;
                        cPath->stackPath.pop();
                    }
                }
            }
        }
    }
}



std::stack<int> SystemPathPlanning::Dijkstra(ManWayPoint* _graph, int start, int end) {
    //cout << "----------------------------------\n";
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
    //TODO: Cambiar esto para tenerlo guardado en una entidad o algo y no hacerlo cada calculo de Dijkstra
    for(auto node : _graph->GetEntities()){
        auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdges = static_cast<CWayPointEdges*>(node->GetComponent(CompType::WayPointEdgesComp).get());

        for(auto edge : cWayPointEdges->edges){
            graph[cWayPoint->id][edge.to] = edge.cost;
        }
    }

    //Comenzamos Dijkstra
    float distanceFromStart[size],pred[size];
    int visited[size],count,minDistanceFromStart,nextClosestNode = -1,i;

    for(i=0;i<size;i++) {
        distanceFromStart[i] = graph[start][i];  //Metemos las ponderaciones a los nodos desde el que iniciamos(Si no tiene es = INT_MAX)
        pred[i] = start;                
        visited[i] = 0;
    }

    //La distancia a si mismo es siempre 0
    distanceFromStart[start]=0; 
    visited[start]=1;
    count=1;

    while(count<size-1) {
        minDistanceFromStart=INT_MAX;
        for(i=0;i<size;i++){
            if(distanceFromStart[i] < minDistanceFromStart && !visited[i]) {
                //Si la distancia al nodo i es menor que la minDistanceFromStart y no esta visitado
                //Recordatorio: Si nuestro nodo start no esta conectado con i entonces distanceFromStart[1] = INT_MAX y no entrará aquí
                minDistanceFromStart=distanceFromStart[i]; // Distancia al nodo adyacente mas cercano
                nextClosestNode=i; //Siguiente nodo adyacente mas cercano
            }
        }

        visited[nextClosestNode]=1;

        for(i=0;i<size;i++){
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