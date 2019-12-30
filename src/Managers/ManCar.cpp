#include "ManCar.h"
#include <functional>
#include <iostream>
#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Physics.h"
#include "../Components/CPowerUp.h"
#include "../Components/CShield.h"
#include "../Components/CTotem.h"
#include "../Components/CRoboJorobo.h"
#include "../Components/CNitro.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Game.h"
#include "Manager.h"


class Position;
using namespace std;

ManCar::ManCar() {
    SubscribeToEvents();
    CreateMainCar();
    cout << "Hemos creado un powerup, ahora tenemos " << entities.size() << " powerups" << endl;
}


// TO-DO: este paso de physics es kk, hay que revisarlo de enviarlo como referencia o algo pero me da error
ManCar::ManCar(Physics *_physics, Camera *_cam) : ManCar() {
    this->physics = _physics;
    this->cam = _cam;
}





std::stack<int> ManCar::Dijkstra(ManWayPoint* _graph, int start, int end) {
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
    int visited[size],count,minDistanceFromStart,nextClosestNode,i;

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

    //cout << "Nuevo Path: ";
    stack<int> pathAux(path);
    while(!pathAux.empty()){
        auto node = pathAux.top();
        pathAux.pop();

        //cout << node << " - ";
    }

    //cout << "\n---------------\n";

    return path;

    //cout << "\n\n\n";
}


// comprueba si has superado el tiempo necesario para ganar
void ManCar::UpdateCar(){
    auto cTotem = static_cast<CTotem*>(GetCar()->GetComponent(CompType::TotemComp).get());
    if(cTotem->active){
        cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
    }

    if(cTotem->accumulatedTime/1000.0 > cTotem->durationTime/1000.0){
        cout << "Has ganado\n";
        Game::GetInstance()->SetState(State::ENDRACE);
    }

    // Actualiza el componente nitro
    auto cNitro = static_cast<CNitro *>(GetCar()->GetComponent(CompType::NitroComp).get());
    if(cNitro->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime){  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }

    // Actualiza el componente escudo
    auto cShield = static_cast<CShield *>(GetCar()->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime){  // comprueba el tiempo desde que se lanzo
        cShield->deactivePowerUp();
    }


}


void ManCar::UpdateCarAI(CarAI* carAI,ManWayPoint* graph){
    auto cTotem = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());
    if(cTotem->active){
        cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
    }

    if(cTotem->accumulatedTime/1000.0 > cTotem->durationTime/1000.0){
        cout << "Has ganado\n";
        Game::GetInstance()->SetState(State::ENDRACE);
    }

    // Actualiza el componente nitro
    auto cNitro = static_cast<CNitro *>(carAI->GetComponent(CompType::NitroComp).get());
    if(cNitro->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime){  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }

    // Actualiza el componente escudo
    auto cShield = static_cast<CShield *>(carAI->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime){  // comprueba el tiempo desde que se lanzo
        cShield->deactivePowerUp();
    }

    //Guardamos en varAIbles los componentes
	auto cTransformable = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());
    //auto cWayPoint     = static_cast<CWayPoint*>(carAI->GetComponent(CompType::WayPointComp).get());
    //float radious = cWayPoint->radious;
    auto cPosDestination     = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
    float radious = cPosDestination->radious;


    //Vamos a comprobar si esta en el rango del waypoint
    if((cPosDestination->position.z - radious) < cTransformable->position.z && (cPosDestination->position.z + radious) >= cTransformable->position.z 
        && (cPosDestination->position.x - radious) < cTransformable->position.x && (cPosDestination->position.x + radious) >= cTransformable->position.x){
            
        //Tenemos que comprobar si le quedan mas nodos que visitar en el path
        auto cPath = static_cast<CPath*>(carAI->GetComponent(CompType::PathComp).get());
        if(!cPath->stackPath.empty()){
            auto actualNode = cPath->stackPath.top();
            cPath->stackPath.pop();
            //cout << "Llegamos al WayPoint: " << actualNode << endl;
            if(!cPath->stackPath.empty()){
                //Le asignamos el WayPoint siguiente del path (graph->GetEntities()[cPath->stackPath.top()])
                auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[cPath->stackPath.top()]->GetComponent(CompType::WayPointComp).get());
                cPosDestination->position = cWayPoint->position;

                carAI->SetDestination(cPosDestination);
            }else{
                //Si esta vacio es que ha acabado el path y recalculamos otro
                //TO-DO: de momento le recalculamos otro aleatorio
                // habra que llamar al arbol de decisiones broooo
                int indx;
                do{
                    indx = rand() % graph->GetEntities().size();

                }while(indx==actualNode);

                //COMPROBAMOS DIJKSTRA
                auto path = Dijkstra(graph,actualNode,indx);
                carAI->SetPath(path);

                auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[path.top()]->GetComponent(CompType::WayPointComp).get());
                carAI->SetWayPoint(cWayPoint);
            }          
        }
    }
}


ManCar::~ManCar() {
    cout << "Llamando al destructor de ManCar" << endl;
    CarAIs.clear();
    CarAIs.shrink_to_fit();
}


void ManCar::CreateMainCar() {
    car = make_shared<Car>();
}


void ManCar::CreateCar() {
    shared_ptr<Car> p = make_shared<Car>();
    entities.push_back(p);
}


void ManCar::CreateCarAI(glm::vec3 _position,  CWayPoint* _waypoint){
	shared_ptr<CarAI> p = make_shared<CarAI>(_position);
    CarAIs.push_back(p);
    p->SetWayPoint(_waypoint); // tiene que tener un waypoint inicial To-Do: cambiar esto
}


void ManCar::CreateCarAI(){
	shared_ptr<CarAI> p = make_shared<CarAI>();
    CarAIs.push_back(p);
}


void ManCar::SubscribeToEvents() {
    // auto accelerateCar = [&](Data d) {
    //     cout << "Estamos ejecutando lambda " << endl;
    //     /*Car *c = this->GetCar().get();
    //     auto components = c->GetComponents();
    //     CTransformable *trans = static_cast<CTransformable*>(components[CompType::TransformableComp].get()); 
    //     cout << "El coche está en  " << trans->position.x << "," << trans->position.y << "," << trans->position.z << endl;*/
    //     //physics->Accelerate(car.get(), cam);
    // };

    /*int (*accelerate) (Data) = [&](Data d) {
        physics->Accelerate(car.get(), cam);
        return 0;
    };*/
    //int (*func_pointer) (int) = [](int a) { return a; };

    //accelerateCar(0);
    // auto lambdaAccelerate = [&](DataMap d){this->AccelerateCar(d);};
    //
    //auto lambdaGuardaAccel = [&lambdaAccelerate](DataMap d) {lambdaAccelerate(d);};


    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_I,
        bind(&ManCar::AccelerateCar, this, placeholders::_1),
        "AccelerateCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_O,
        bind(&ManCar::Decelerate, this, placeholders::_1),
        "Decelerate"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_A,
        bind(&ManCar::TurnLeftCar, this, placeholders::_1),
        "TurnLeftCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_D,
        bind(&ManCar::TurnRightCar, this, placeholders::_1),
        "TurnRightCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::NO_I_O_PRESS,
        bind(&ManCar::NotAcceleratingOrDecelerating, this, placeholders::_1),
        "NotAcceleratingOrDecelerating"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::NO_A_D_PRESS,
        bind(&ManCar::NotTurning, this, placeholders::_1),
        "NotTurning"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CATCH_BOX_POWERUP,
        bind(&ManCar::CatchPowerUp, this, placeholders::_1),
        "CatchPowerUp"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CATCH_AI_BOX_POWERUP,
        bind(&ManCar::CatchPowerUpAI, this, placeholders::_1),
        "CatchPowerUpAI"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_SPACE,
        bind(&ManCar::ThrowPowerUp, this, placeholders::_1),
        "ThrowPowerUp"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::THROW_POWERUP_AI,
        bind(&ManCar::ThrowPowerUpAI, this, placeholders::_1),
        "ThrowPowerUpAI"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_ENTITY_POWERUP,
        bind(&ManCar::CollisionPowerUp, this, placeholders::_1),
        "CollisionPowerUp"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_ENTITY_AI_POWERUP,
        bind(&ManCar::CollisionPowerUpAI, this, placeholders::_1),
        "CollisionPowerUpAI"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_PLAYER_TOTEM,
        bind(&ManCar::CatchTotemPlayer, this, placeholders::_1),
        "CatchTotemPlayer"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_AI_TOTEM,
        bind(&ManCar::CatchTotemAI, this, placeholders::_1),
        "CatchTotemAI"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CHANGE_DESTINATION,
        bind(&ManCar::ChangePosDestination, this, placeholders::_1),
        "ChangePosDestination"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::MOVE_TO_POWERUP,
        bind(&ManCar::MoveToPowerUp, this, placeholders::_1),
        "MoveToPowerUp"));

}


void ManCar::ChangePosDestination(DataMap data){
    auto cPosDestination = static_cast<CPosDestination*>(any_cast<CarAI*>(data["actualCar"])->GetComponent(CompType::PosDestination).get());
    cPosDestination->position = any_cast<glm::vec3>(data["posDestination"]);
}

void ManCar::MoveToPowerUp(DataMap data){
    auto graph = any_cast<ManWayPoint*>(data["manWayPoints"]);
    auto cPath = static_cast<CPath*>(any_cast<CarAI*>(data["actualCar"])->GetComponent(CompType::PathComp).get());
    auto cPosDestination = static_cast<CPosDestination*>(any_cast<CarAI*>(data["actualCar"])->GetComponent(CompType::PosDestination).get());

    if(!cPath->stackPath.empty()){
        //Le asignamos el WayPoint siguiente del path (graph->GetEntities()[cPath->stackPath.top()])
        auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[cPath->stackPath.top()]->GetComponent(CompType::WayPointComp).get());
        cPosDestination->position = cWayPoint->position;

        any_cast<CarAI*>(data["actualCar"])->SetDestination(cPosDestination);
    }
}


void ManCar::CatchTotemAI(DataMap d){
    auto cTotem = static_cast<CTotem*>(any_cast<Entity*>(d["actualCar"])->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();
}

void ManCar::CatchTotemPlayer(DataMap d){
    auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();
}

void ManCar::UseTotem(Entity* carWinTotem){
    auto cTotem = static_cast<CTotem*>(carWinTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();
    //std::cout << "Has utilizado el robo jorobo de p*** madre coleeega ahora es tuyo" << std::endl; 
}

void ManCar::ThrowTotem(Entity* carLoseTotem){
    auto cTotem = static_cast<CTotem*>(carLoseTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = false;
    cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
    //std::cout << "El tiempo acumulado del totem hasta ahora del jugador es de:  " << cTotem->accumulatedTime/1000.0 << std::endl; 
}


bool ManCar::useRoboJorobo(Entity* newCarWithTotem){
    // recorremos las IA
    for(shared_ptr<Entity> carAI : CarAIs){
        auto cTotem = static_cast<CTotem*>(carAI.get()->GetComponent(CompType::TotemComp).get()); 
        // Si algun coche tenia el totem .... lo pierde, comprobamos que no sea el mmismo coche con las ID
        if(cTotem->active == true && newCarWithTotem!=carAI.get()){
            ThrowTotem(carAI.get());
            //al perderlo se lo asignamos al que ha usado el robo jorobo
            UseTotem(newCarWithTotem);

            return true;                                                               // para salirnos y no hacer mas calculos
        }
    }
    // comprobamos el player
    auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get()); 
    if(cTotem->active == true && car.get()!=newCarWithTotem){
        ThrowTotem(car.get());
        //al perderlo se lo asignamos al que ha usado el robo jorobo
        UseTotem(newCarWithTotem);
        return true;                                                               // para salirnos y no hacer mas calculos
    }
    return false;
}



void ManCar::CollisionPowerUp(DataMap d){
    // debemos desactivar el powerUp y para el contador de tiempo del totem
    auto cShield = static_cast<CShield*>(car.get()->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp == false){            // comprobamos si tiene el escudo
        auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            ThrowTotem(car.get());
        }
        // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
        auto cCar = static_cast<CCar*>(car.get()->GetComponent(CompType::CarComp).get());
        cCar->speed = 0.0f;
    }else{
        std::cout << "El escudo me salvo el culito :D" << std::endl;
        cShield->deactivePowerUp(); // desactivamos el escudo
    }
}


void ManCar::CollisionPowerUpAI(DataMap d){
    // debemos desactivar el powerUp y para el contador de tiempo del totem
    auto cShield = static_cast<CShield*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp == false){            // comprobamos si tiene el escudo
        auto cTotem = static_cast<CTotem*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            ThrowTotem(any_cast<Entity*>(d["carAI"]));
        }
        // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
        auto cCar = static_cast<CCar*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::CarComp).get());
        cCar->speed = 0.0f;  // To-Do: no funciona en la IA por que la logica difusa no la hace acelerar
    }else{
        std::cout << "El escudo me salvo el culito :D" << std::endl;
        cShield->deactivePowerUp(); // desactivamos el escudo
    }
}


// se calcula el coche mas cerano
CTransformable* ManCar::calculateCloserCar(Entity* actualCar){
    // Primero metemos al jugador por lo que no hace falta calcularlo luego
    CTransformable* closestCar = nullptr;
    bool carPrincipal = false;
    if(actualCar != car.get()){
        closestCar = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());
    }else{
        closestCar = static_cast<CTransformable*>(CarAIs[0].get()->GetComponent(CompType::TransformableComp).get());
        carPrincipal = true;
    }
    auto cTransActualCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float vectorX = closestCar->position.x - cTransActualCar->position.x;
    float vectorZ = closestCar->position.z - cTransActualCar->position.z;
    float distanceMimum = sqrt((vectorX*vectorX) + (vectorZ*vectorZ));

    // reducimos cierta distancia en caso de que se encuentre en el radio de vision
    if(carPrincipal == true){
        if(carInVisionRange(actualCar, CarAIs[0].get(), 15) == true)
            distanceMimum = distanceMimum/100.0;
    }else{
        if(carInVisionRange(actualCar, car.get(), 15) == true)
            distanceMimum = distanceMimum / 100.0;
    }

    float distanceNext = 0.0;
    float vectorXNext = 0.0;
    float vectorZNext = 0.0;
    // Para CarAI
    for(shared_ptr<Entity> carAI : CarAIs){
        if(actualCar != carAI.get()){
            auto cTransNextCar = static_cast<CTransformable*>(carAI.get()->GetComponent(CompType::TransformableComp).get()); 
            vectorXNext = cTransNextCar->position.x - cTransActualCar->position.x;     
            vectorZNext = cTransNextCar->position.z - cTransActualCar->position.z;
            distanceNext = sqrt((vectorXNext*vectorXNext) + (vectorZNext*vectorZNext));
            
            if(carInVisionRange(actualCar, carAI.get(), 15) == true)
                distanceNext = distanceNext / 100.0;
            if(distanceMimum > distanceNext){
                distanceMimum = distanceNext;
                closestCar = cTransNextCar;
            }
        }
    }
    return closestCar;
}



void ManCar::ThrowPowerUp(DataMap d) {
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    //auto cRoboJorobo = static_cast<CRoboJorobo*>(car->GetComponent(CompType::RoboJoroboComp).get());
    auto cShield = static_cast<CShield*>(car.get()->GetComponent(CompType::ShieldComp).get());
    auto cNitro = static_cast<CNitro*>(car.get()->GetComponent(CompType::NitroComp).get());
    bool robado = false;  
    if(cPowerUpCar->typePowerUp != typeCPowerUp::None){

        switch (cPowerUpCar->typePowerUp){
            case typeCPowerUp::RoboJorobo:
                robado = useRoboJorobo(car.get());
                if (!robado)
                    std::cout << "La has cagado, el Totem no lo tenia nadie..." << std::endl; 
                break;
            case typeCPowerUp::EscudoMerluzo:
                cShield->activatePowerUp();
                break;
            case typeCPowerUp::SuperMegaNitro:
                cNitro->activatePowerUp();
                break;
            default:  // en caso del melon molon o la telebanana
                shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                DataMap data;
                data["typePowerUp"] = cPowerUpCar->typePowerUp;
                data["posCocheSalida"] = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());;
                // To-Do: actualmente solo se pasa el coche mas cercano, calcular aparte si se encuentra en pantalla
                data["posCochePerseguir"] = calculateCloserCar(car.get());
                eventManager->AddEventMulti(Event{EventType::PowerUp_Create, data});

                break;
        }

        // Sonido de lanzar power-up
        d["typePowerUp"] = cPowerUpCar->typePowerUp;
        EventManager::GetInstance()->AddEventMulti(Event{EventType::THROW_POWERUP, d});

        // Ya no tenemos power-up
        cPowerUpCar->typePowerUp = typeCPowerUp::None;
        DataMap d;
        d["typePowerUp"] = cPowerUpCar->typePowerUp;
        EventManager::GetInstance()->AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, d});
        
    }
}



void ManCar::ThrowPowerUpAI(DataMap d) {
    auto cPowerUpCar = static_cast<CPowerUp*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::PowerUpComp).get());
    //auto cRoboJorobo = static_cast<CRoboJorobo*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::RoboJoroboComp).get());
    auto cShield = static_cast<CShield*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::ShieldComp).get());
    auto cNitro = static_cast<CNitro*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::NitroComp).get());
    bool robado = false; 
    
    if(cPowerUpCar->typePowerUp != typeCPowerUp::None){
        switch (cPowerUpCar->typePowerUp){
            case typeCPowerUp::RoboJorobo:
                robado = useRoboJorobo(any_cast<CarAI*>(d["actualCar"]));
                if (!robado)
                    std::cout << "La has cagado, el Totem no lo tenia nadie..." << std::endl; 
                break;
            case typeCPowerUp::EscudoMerluzo:
                cShield->activatePowerUp();
                break;
            case typeCPowerUp::SuperMegaNitro:
                cNitro->activatePowerUp();
                break;
            default:     // en caso del melon molon o la telebanana
                shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                DataMap data;
                data["typePowerUp"] = cPowerUpCar->typePowerUp;
                data["posCocheSalida"] = static_cast<CTransformable*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::TransformableComp).get());;
                // To-Do: actualmente solo se pasa el coche mas cercano, calcular aparte si se encuentra en pantalla
                data["posCochePerseguir"] = calculateCloserCar(any_cast<CarAI*>(d["actualCar"]));
                eventManager->AddEventMulti(Event{EventType::PowerUp_Create, data});

                break;
        }
        cPowerUpCar->typePowerUp = typeCPowerUp::None;
    }
}

/*
int calculateProbabilityPowerUp(int totalPowerUps, std::vector<int> probabilityPU){
    int sum_of_weight = 0;

    int rnd = rand() % 100+1; // 100%
    for(int i=0; i<totalPowerUps; i++) {
      if(rnd < probabilityPU[i])
        return i;
      rnd -= probabilityPU[i];
    }
}
*/

void ManCar::CatchPowerUp(DataMap d) {
    // To-Do: porcentajes temporales
    srand(time(NULL));
    int indx = rand() % 100+1;
    if(indx <= 5)                       // 5%
        indx = 1;
    else if(indx > 5 && indx <= 20)     // 15%
        indx = 2;
    else if(indx > 20 && indx <= 40)    // 20%
        indx = 3;
    else if(indx > 40 && indx <= 55)    // 15%
        indx = 4;
    else if(indx > 55 && indx <= 70)    // 15%
        indx = 5;
    else if(indx > 70)                  //  30%
        indx = 6;

    //None,               // 0
    //RoboJorobo,         // 1
    //SuperMegaNitro,     // 2
    //PudinDeFrambuesa,   // 3
    //EscudoMerluzo,      // 4
    //TeleBanana,         // 5
    //MelonMolon          // 6
    //indx = 5;
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        std::cout << "Mi PowerUp es:   " << (int)cPowerUpCar->typePowerUp << std::endl;
        d["typePowerUp"] = cPowerUpCar->typePowerUp;

        //RenderFacadeManager::GetInstance()->GetRenderFacade()->FacadeUpdatePowerUpHUD(d);
        EventManager::GetInstance()->AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, d});
    }
    //cPowerUp->typePowerUp = dynamic_cast<typeCPowerUp*>(indx);
}


void ManCar::CatchPowerUpAI(DataMap d) {
    // To-Do: porcentajes temporales
    srand(time(NULL));
    int indx = rand() % 100+1;
    if(indx <= 5)                       // 5%
        indx = 1;
    else if(indx > 5 && indx <= 20)     // 15%
        indx = 2;
    else if(indx > 20 && indx <= 40)    // 20%
        indx = 3;
    else if(indx > 40 && indx <= 55)    // 15%
        indx = 4;
    else if(indx > 55 && indx <= 70)    // 15%
        indx = 5;
    else if(indx > 70)                  //  30%
        indx = 6;

    auto cPowerUpCar = static_cast<CPowerUp*>(any_cast<Entity*>(d["actualCar"])->GetComponent(CompType::PowerUpComp).get());
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        std::cout << "Power Up de la IA:   " << (int)cPowerUpCar->typePowerUp << std::endl;
    }
    //cPowerUp->typePowerUp = dynamic_cast<typeCPowerUp*>(indx);
}



// calcula si el otro coche se encuentra en el rango de vision del coche actual
bool ManCar::carInVisionRange(Entity* actualCar, Entity* otherCar, uint32_t rangeVision){
    float seeCar = false;
    // calcular un desplazamiento para ser en tercera persona
    auto cTransformableActual = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float posXActualCar = cTransformableActual->position.x + 40 * cos((cTransformableActual->rotation.y * PI) / 180.0);
    float posZActualCar = cTransformableActual->position.z - 40 * sin((cTransformableActual->rotation.y * PI) / 180.0);

    // vector between actualCar and otherCar
    auto cTransformableOther = static_cast<CTransformable*>(otherCar->GetComponent(CompType::TransformableComp).get());
    float vetorWaypointX = (cTransformableOther->position.x - posXActualCar);
    float vetorWaypointZ = (cTransformableOther->position.z - posZActualCar);

    // calculate position rotated of otherCar atan2
    float valueAtan2 = atan2(vetorWaypointZ,vetorWaypointX)*180/PI;
    valueAtan2 = 180.0 - valueAtan2; // se le restan ya que el eje empieza en el lado contrario 
    if(valueAtan2<0)
        valueAtan2 += 360;

    //compare with actualCar actualRotation
    if(cTransformableActual->rotation.y-rangeVision >= 0 && cTransformableActual->rotation.y+rangeVision<360){
        if(cTransformableActual->rotation.y-rangeVision<valueAtan2 && cTransformableActual->rotation.y+rangeVision>valueAtan2){
            seeCar=true;
        }
    }else{  // coge el angulo 0 de por medio
        float rotMin = cTransformableActual->rotation.y-rangeVision;
        float rotMax = cTransformableActual->rotation.y+rangeVision;
        if(cTransformableActual->rotation.y-rangeVision < 0)
            rotMin += 360;
        if(cTransformableActual->rotation.y+rangeVision >= 360)
            rotMax -= 360;
        if(rotMin<valueAtan2 || rotMax>valueAtan2){
            seeCar=true;
        }
    }
    return seeCar;
}


// comprobamos si tenemos algun coche en el rango de vision
bool ManCar::anyCarInVisionRange(Entity* actualCar, uint32_t rangeVision){
    bool seeCar = false;
    for(shared_ptr<Entity> carAI : CarAIs){
        if(actualCar!=carAI.get()){
            if(carInVisionRange(actualCar,carAI.get(), rangeVision) == true){
                seeCar = true;
            }
        }
    }
    // comprobamos el player
    if(car.get()!=actualCar){
        if(carInVisionRange(actualCar,car.get(), rangeVision) == true){
            seeCar = true;
        }
    }
    return seeCar;
}





void ManCar::TurnLeftCar(DataMap d) {
    // cout << "Han llamado izquierda" << endl;
    physics->TurnLeft(car.get(), cam);
}

void ManCar::NotTurning(DataMap d) {
    // cout << "Han llamado not turning" << endl;
    physics->NotTurning(car.get(), cam);
}

void ManCar::Decelerate(DataMap d) {
    // cout << "Han llamado decel" << endl;
    physics->Decelerate(car.get(), cam);
}

void ManCar::NotAcceleratingOrDecelerating(DataMap d) {
    // cout << "Han llamado notaccel" << endl;
    physics->NotAcceleratingOrDecelerating(car.get(), cam);
}

void ManCar::TurnRightCar(DataMap d) {
    // cout << "Han llamado derecha" << endl;
    physics->TurnRight(car.get(), cam);
}

void ManCar::AccelerateCar(DataMap d) {
    // cout << "Estamos en AccelerateCar" << endl;
    // int mint = any_cast<int>(d["int"]);
    // cout << "Hemos obtenido el int" << mint << endl;
    // float mfloat = any_cast<float>(d["float"]);
    // cout << "Hemos obtenido el float" << endl;
    // vector<int> mvect = any_cast<vector<int>>(d["vector"]);
    // cout << "Hemos recibido por evento un int=" << mint << " un float=" << mfloat << " y un vector de int con size=" << mvect.size() << endl;
    
    physics->Accelerate(car.get(), cam);
}

void ManCar::Integrate(float delta) {
    //physics->update(GetCar().get(), cam.get());
}