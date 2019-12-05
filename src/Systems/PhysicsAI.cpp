#include "PhysicsAI.h"


void CalculatePositionAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);
void CalculatePositionReverseAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);


PhysicsAI::PhysicsAI(){

}


//Nos devuelve el angulo en radianos entre el coche y el waypoint
float calculateAngle(CWayPoint* wayPointNext, CarAI* car){
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable*>(components[CompType::TransformableComp]);
    
    
    //Calculo de angulo en radianes
    //Fuente: https://math.stackexchange.com/questions/1201337/finding-the-angle-between-two-points
    float x = (wayPointNext->position.x - cTransformable->position.x );
    float z = (wayPointNext->position.z - cTransformable->position.z);

    float angle = atan2(z,x);

    cout << "Angulo: " << angle*180/PI << endl;
    return angle;
}


void PhysicsAI::Update(vector<WayPoint *> wayPoints, CarAI* car, float deltaTime){
    //Guardamos en varAIbles los componentes
    auto components = car->GetComponents();
	auto cTransformable = static_cast<CTransformable*>(components[CompType::TransformableComp]);
    auto cWayPoint     = static_cast<CWayPoint*>(components[CompType::WayPointComp]);
    auto cCar        = static_cast<CCar*>(components[CompType::CarComp]);

    float radious = cWayPoint->radious;
    float angle = calculateAngle(cWayPoint, car);

    //TODO: Cambiar por valores del coche y todas esas vainas

    //Vamos a comprobar si esta en el rango del waypoint
    if((cWayPoint->position.z - radious) < cTransformable->position.z && (cWayPoint->position.z + radious) >= cTransformable->position.z 
        && (cWayPoint->position.x - radious) < cTransformable->position.x && (cWayPoint->position.x + radious) >= cTransformable->position.x){
            //Ha llegado al rango vamos a buscarle otro waypoint
            //TODO: Ya veremos como gestionamos el cambiar de waypoint de momento lo pongo al azar
            int indx = rand() % wayPoints.size();
            cout << "Cambiamos de WayPoint\n";
            car->SetWayPoint(wayPoints[indx]->position);
    }else{
        //Aumentamos la velocidad
        cCar->wheelRotation = angle;
        cCar->speed += cCar->acceleration;
        if(cCar->speed > cCar->maxSpeed){
            cCar->speed = cCar->maxSpeed;
        }

        if(cCar->speed>=0){
            CalculatePositionAI(cCar,cTransformable,deltaTime,angle);            
        }
        
    }
    
}


//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void CalculatePositionAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle){
    
    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->position.x += cos(angle) * cCar->speed * deltaTime;
    cTransformable->position.z += sin(angle) * cCar->speed * deltaTime;

    //Si tiene rotacion, rotamos el coche
    if(cCar->wheelRotation != 0){
        cTransformable->rotation.y += cCar->wheelRotation * 0.20;
    }
}


//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void CalculatePositionReverseAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle){
    
    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->position.x += cos(angle) * cCar->speed * deltaTime;
    cTransformable->position.z += sin(angle) * cCar->speed * deltaTime;

    //Si tiene rotacion, rotamos el coche
    if(cCar->wheelRotation != 0){
        cTransformable->rotation.y -= cCar->wheelRotation * 0.20;
    }
}
