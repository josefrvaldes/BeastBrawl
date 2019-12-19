#include "PhysicsAI.h"


void CalculatePositionAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);
void CalculatePositionReverseAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);


PhysicsAI::PhysicsAI(){

}


std::stack<int> PhysicsAI::Dijkstra(ManWayPoint* _graph, int start, int end) {


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

    cout << "Nuevo Path: ";
    stack<int> pathAux(path);
    while(!pathAux.empty()){
        auto node = pathAux.top();
        pathAux.pop();

        cout << node << " - ";
    }

    cout << "\n---------------\n";

    return path;

    cout << "\n\n\n";
}


void PhysicsAI::fuzzyRules(CarAI* car){
    auto cCar        = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());
    float maxSpeed = cCar->maxSpeed;
    float minSpeed = cCar->reverseMaxSpeed;
    float accelerationCar = cCar->acceleration;
    //float algo = cCar->
    fuzzyRulesVelocity(maxSpeed, minSpeed, accelerationCar);
    fuzzyRulesAngle();

}
void PhysicsAI::fuzzyRulesVelocity(float maxSpeed, float minSpeed, float accelerationCar){

    shared_ptr<FuzzyVariable> ActualVelocity = flVelocity->CreateFLV("ActualVelocity");
    shared_ptr<FzSet> Velocity_Slow = ActualVelocity->AddLeftShoulderSet("Velocity_Slow", minSpeed, 0, 5);
    shared_ptr<FzSet> Velocity_Normal = ActualVelocity->AddTriangularSet("Velocity_Normal", 0, 5, maxSpeed/2);
    shared_ptr<FzSet> Velocity_High = ActualVelocity->AddRightShoulderSet("Velocity_High", 5, maxSpeed/2, maxSpeed);

    shared_ptr<FuzzyVariable> Angle = flVelocity->CreateFLV("Angle");
    shared_ptr<FzSet> Angle_Slow = Angle->AddLeftShoulderSet("Angle_Slow", 0, 30, 60);
    shared_ptr<FzSet> Angle_Normal = Angle->AddTriangularSet("Angle_Normal", 30, 60, 120);
    shared_ptr<FzSet> Angle_High = Angle->AddRightShoulderSet("Angle_High", 60, 120, 180);

    //FuzzyVariable& Distance = flVelocity.CreateFLV("Distance");
    //FzSet Distance_Near = Distance.AddLeftShoulderSet("Distance_Near", 0, 0, 10);
    //FzSet Distance_nNormal = Distance.AddTriangularSet("Distance_nNormal", 0, 10, 30);
    //FzSet Distance_Far = Distance.AddRightShoulderSet("Distance_Far", 10, 30, 40);

    shared_ptr<FuzzyVariable> Acceleration = flVelocity->CreateFLV("Acceleration");
    shared_ptr<FzSet> Accelerate_Brake = Acceleration->AddLeftShoulderSet("Accelerate_Brake", -accelerationCar, -accelerationCar/2, 0);
    shared_ptr<FzSet> Accelerate_None = Acceleration->AddTriangularSet("Accelerate_None", -accelerationCar/2, 0,accelerationCar/2);
    shared_ptr<FzSet> Accelerate_Max = Acceleration->AddRightShoulderSet("Accelerate_Max", 0, accelerationCar/2, accelerationCar);
    // To-Do: revisar el new por que no se tiene que hacer
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Slow, Angle_Slow)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Slow, Angle_Normal)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Slow, Angle_High)), Accelerate_None);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Normal, Angle_Slow)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Normal, Angle_Normal)), Accelerate_None);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Normal, Angle_High)), Accelerate_None);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_High, Angle_Slow)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_High, Angle_Normal)), Accelerate_Brake);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_High, Angle_High)), Accelerate_Brake);
}

void PhysicsAI::fuzzyRulesAngle(){

    shared_ptr<FuzzyVariable> Distance = flAngle->CreateFLV("Distance");
    shared_ptr<FzSet> Distance_Near = Distance->AddLeftShoulderSet("Distance_Near", 0, 0, 40);
    shared_ptr<FzSet> Distance_Normal = Distance->AddTriangularSet("Distance_Normal", 20, 40, 60);
    shared_ptr<FzSet> Distance_Far = Distance->AddRightShoulderSet("Distance_Far", 40, 100, 100);

    shared_ptr<FuzzyVariable> Direction = flAngle->CreateFLV("Direction");
    shared_ptr<FzSet> Direction_Left = Direction->AddLeftShoulderSet("Direction_Left", -180, -180, 0);
    shared_ptr<FzSet> Direction_Center = Direction->AddTriangularSet("Direction_Center", -20, 0, 20);
    shared_ptr<FzSet> Direction_Right = Direction->AddRightShoulderSet("Direction_Right", 0, 180, 180);

    shared_ptr<FuzzyVariable> Rotation = flAngle->CreateFLV("Rotation");
    shared_ptr<FzSet> Rotation_Left = Rotation->AddLeftShoulderSet("Rotation_Left", -10, -5, 0);
    shared_ptr<FzSet> Rotation_None = Rotation->AddTriangularSet("Rotation_None", -5, 0, 5);
    shared_ptr<FzSet> Rotation_Right = Rotation->AddRightShoulderSet("Rotation_Right", 0, 5, 10);
    // To-Do: revisar el new por que no se tiene que hacer
    flAngle->AddRule( (make_shared<FzAND>(Distance_Near, Direction_Left)), Rotation_Left);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Near, Direction_Center)), Rotation_None);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Near, Direction_Right)), Rotation_Right);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Normal, Direction_Left)), Rotation_Left);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Normal, Direction_Center)), Rotation_None);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Normal, Direction_Right)), Rotation_Right);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Far, Direction_Left)), Rotation_Left);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Far, Direction_Center)), Rotation_None);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Far, Direction_Right)), Rotation_Right);
}




//Nos devuelve el angulo en radianos entre el coche y el waypoint
float calculateAngle(CWayPoint* wayPointNext, CarAI* car,CCar* cCar){
    auto cTransformable = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());

    // calcular vector al wayPoint
    float vetorWaypointX = (wayPointNext->position.x - cTransformable->position.x );
    float vetorWaypointZ = (wayPointNext->position.z - cTransformable->position.z);
    
    // se calcula el siguiente punto al que avanzara el coche
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;
    // se calcula el vector entre el siguiente punto y y el punto actual del coche
    float xCoche = (posXSiguiente - cTransformable->position.x );
    float zCoche = (posZSiguiente - cTransformable->position.z);
    //float atanCozee = atan2(zCoche,xCoche)*180/PI;
    //std::cout << atanCozee << " --- PosX-Y( " << cTransformable->position.x << " , " << cTransformable->position.z << ") -- ( " << posXSiguiente << " , " << posZSiguiente << " )" << std::endl; 

    // se calcula el angulo entre los dos vectores
    float numerador = xCoche*vetorWaypointX + zCoche*vetorWaypointZ;
    float denominador = sqrt(xCoche*xCoche + zCoche*zCoche) * sqrt(vetorWaypointX*vetorWaypointX + vetorWaypointZ*vetorWaypointZ);
    float angle2 = 0.0;
    if(denominador!=0)
        angle2 = acos(numerador/denominador);
    angle2 = (angle2 * 180) / PI;

    // calcular si tiene que girar a la izquierda o derecha
    float valueAtan2 = atan2(vetorWaypointZ,vetorWaypointX)*180/PI;
    valueAtan2 = 180.0 - valueAtan2; // se le restan ya que el eje empieza en el lado contrario 
    if(valueAtan2<0)
        valueAtan2 += 360;

    ////calcular opuesto
    float oppositeAngleAtan2 = 0.0;
    if(valueAtan2<180)
        oppositeAngleAtan2 = valueAtan2 + 180;
    else
        oppositeAngleAtan2 = valueAtan2 - 180;
    
    // sentido de giro
    float prueba = 0.0;
    if(valueAtan2>=180){ // este if se hace para comprobar que se pueda hacer el && y no caiga en la parte que se reinician los grados
        if(cTransformable->rotation.y>oppositeAngleAtan2 && cTransformable->rotation.y<valueAtan2)
            prueba = angle2;
        else
            prueba = angle2 * (-1);
    }else{
        if(cTransformable->rotation.y>valueAtan2 && cTransformable->rotation.y<oppositeAngleAtan2)
            prueba = angle2 * (-1);
        else
            prueba = angle2;
    }


    //std::cout << "Atan2: " << prueba << " ----- Atan: " << valueAtan2  << " Opo: " << oppositeAngleAtan2 << "   Rot: " << cTransformable->rotation.y << endl; // primero la Z, luego la X
    return prueba;
}


float PhysicsAI::calculateFuzzyVelocity(float speedCar, float angle){
    flVelocity->Fuzzify("ActualVelocity", speedCar); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
    flVelocity->Fuzzify("Angle", angle);

    float defuzzificacion = flVelocity->DeFuzzify("Acceleration"); 
    //std::cout << "LA DEFUZZIFICACION DA:  " << defuzzificacion << std::endl;
    return defuzzificacion;
}

float PhysicsAI::calculateFuzzyDirection(float distance, float direction){
    flAngle->Fuzzify("Distance", distance); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
    flAngle->Fuzzify("Direction", direction);

    float defuzzificacion = flAngle->DeFuzzify("Rotation"); 
    //std::cout << "LA DEFUZZIFICACION DA:  " << defuzzificacion << std::endl;
    return defuzzificacion;
}

void PhysicsAI::InitPhysicsIA(CarAI* car){
    fuzzyRules(car);
    //fuzzyRulesVelocity(car);
}

void PhysicsAI::Update(ManWayPoint* graph, CarAI* car, float deltaTime){
    //Guardamos en varAIbles los componentes
	auto cTransformable = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
    auto cWayPoint     = static_cast<CWayPoint*>(car->GetComponent(CompType::WayPointComp).get());
    auto cCar        = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());
    float angleRange = 0;
    float angle = 0;
    float radious = cWayPoint->radious;
    float distance2P = sqrt( pow((cWayPoint->position.x - cTransformable->position.x),2) + pow((cWayPoint->position.z - cTransformable->position.z),2) );


    //Vamos a comprobar si esta en el rango del waypoint
    if((cWayPoint->position.z - radious) < cTransformable->position.z && (cWayPoint->position.z + radious) >= cTransformable->position.z 
        && (cWayPoint->position.x - radious) < cTransformable->position.x && (cWayPoint->position.x + radious) >= cTransformable->position.x){
            
        //Tenemos que comprobar si le quedan mas nodos que visitar en el path
        auto cPath = static_cast<CPath*>(car->GetComponent(CompType::PathComp).get());

        auto actualNode = cPath->stackPath.top();
        cPath->stackPath.pop();

        cout << "Llegamos al WayPoint: " << actualNode << endl;
        if(!cPath->stackPath.empty()){
            //Le asignamos el WayPoint siguiente del path (graph->GetEntities()[cPath->stackPath.top()])
            auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[cPath->stackPath.top()]->GetComponent(CompType::WayPointComp).get());
            car->SetWayPoint(cWayPoint);
        }else{
            //Si esta vacio es que ha acabado el path y recalculamos otro
            //TODO: de momento le recalculamos otro aleatorio
            int indx;
            do{
                indx = rand() % graph->GetEntities().size();

            }while(indx==actualNode);

            //COMPROBAMOS DIJKSTRA
            auto path = Dijkstra(graph,actualNode,indx);
            car->SetPath(path);

            auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[path.top()]->GetComponent(CompType::WayPointComp).get());
            car->SetWayPoint(cWayPoint);
        }           
    }else{

        angle = calculateAngle(cWayPoint, car, cCar);
        if (angle < 0)
            angleRange = angle*(-1);
        else
            angleRange = angle;

        //cout << "Angulo Positivo: " << angleRange*180/PI << endl;
        if(cCar->speed == 0)
            cCar->speed = 0.1;
            //std::cout << "VOY A ENTRAR A VELOCITY DIFUSA" <<std::endl;
        float fuzzyAceleration = calculateFuzzyVelocity(cCar->speed, angleRange);
        float fuzzyRotation = 0.0;
        if(cCar->speed>3 || cCar->speed < -3){
            fuzzyRotation = calculateFuzzyDirection(distance2P, angle);
        }

        //Aumentamos la velocidad
        //cCar->wheelRotation = angle
        
        cCar->wheelRotation = fuzzyRotation;
        //std::cout << "DeFuzzyRot: " << fuzzyRotation << std::endl;
        cCar->speed += fuzzyAceleration;
        if(cCar->speed > cCar->maxSpeed){
            cCar->speed = cCar->maxSpeed;
        }


        // calculamos las posiciones
        float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
        cTransformable->position.x -= cos(angleRotation) * cCar->speed * deltaTime;
        cTransformable->position.z += sin(angleRotation) * cCar->speed * deltaTime;
        if(cCar->wheelRotation != 0){
            cTransformable->rotation.y += cCar->wheelRotation * 0.20;
            if(cTransformable->rotation.y>=360.0)
                cTransformable->rotation.y -= 360.0;
            else if(cTransformable->rotation.y < 0.0)
                cTransformable->rotation.y += 360.0;
        }
    }



    
    //std::cout << "Car speed" << cCar->speed << std::endl;
    //Aumentamos la velocidad
    /**
    cCar->speed += cCar->acceleration;
    if(cCar->speed > cCar->maxSpeed){
        cCar->speed = cCar->maxSpeed;
    }

        if(cCar->speed>=0){
            CalculatePositionAI(cCar,cTransformable,deltaTime,angle);            
        }
        
    }*/
        //Si tiene rotacion, rotamos el coche
    
}


//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
//void CalculatePositionAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle){
//    
//    //Modificamos la posicion en X y Z en funcion del angulo
//    cTransformable->position.x += cos(angle) * cCar->speed * deltaTime;
//    cTransformable->position.z += sin(angle) * cCar->speed * deltaTime;
//
//    //Si tiene rotacion, rotamos el coche
//    if(cCar->wheelRotation != 0){
//        cTransformable->rotation.y += cCar->wheelRotation * 0.20;
//    }
//}
//
//
////Calcula la posicion del coche (duda con las formulas preguntar a Jose)
//void CalculatePositionReverseAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle){
//    
//    //Modificamos la posicion en X y Z en funcion del angulo
//    cTransformable->position.x += cos(angle) * cCar->speed * deltaTime;
//    cTransformable->position.z += sin(angle) * cCar->speed * deltaTime;
//
//    //Si tiene rotacion, rotamos el coche
//    if(cCar->wheelRotation != 0){
//        cTransformable->rotation.y -= cCar->wheelRotation * 0.20;
//    }
//}
