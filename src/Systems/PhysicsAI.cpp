#include "PhysicsAI.h"


void CalculatePositionAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);
void CalculatePositionReverseAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);


PhysicsAI::PhysicsAI(){

}


void PhysicsAI::fuzzyRulesVelocity(CarAI* car){

    auto components = car->GetComponents();
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);
    float maxSpeed = cCar->maxSpeed;
    float minSpeed = cCar->reverseMaxSpeed;
    float accelerationCar = cCar->acceleration;
    float breakCar = cCar->slowDown;
    //float algo = cCar->


    FuzzyVariable& ActualVelocity = flVelocity.CreateFLV("ActualVelocity");
    FzSet Velocity_Slow = ActualVelocity.AddLeftShoulderSet("Velocity_Slow", minSpeed, minSpeed/2, 0);
    FzSet Velocity_Normal = ActualVelocity.AddTriangularSet("Velocity_Normal", minSpeed/2, 0,maxSpeed/2);
    FzSet Velocity_High = ActualVelocity.AddRightShoulderSet("Velocity_High", 0, maxSpeed/2, maxSpeed);

    FuzzyVariable& Angle = flVelocity.CreateFLV("Angle");
    FzSet Angle_Slow = Angle.AddLeftShoulderSet("Angle_Slow", 0, 30, 60);
    FzSet Angle_Normal = Angle.AddTriangularSet("Angle_Normal", 30, 60, 120);
    FzSet Angle_High = Angle.AddRightShoulderSet("Angle_High", 60, 120, 180);

    //FuzzyVariable& Distance = flVelocity.CreateFLV("Distance");
    //FzSet Distance_Near = Distance.AddLeftShoulderSet("Distance_Near", 0, 0, 10);
    //FzSet Distance_nNormal = Distance.AddTriangularSet("Distance_nNormal", 0, 10, 30);
    //FzSet Distance_Far = Distance.AddRightShoulderSet("Distance_Far", 10, 30, 40);

    FuzzyVariable& Acceleration = flVelocity.CreateFLV("Acceleration");
    FzSet Accelerate_Brake = Acceleration.AddLeftShoulderSet("Accelerate_Brake", breakCar, breakCar/2, 0);
    FzSet Accelerate_None = Acceleration.AddTriangularSet("Accelerate_None", breakCar/2, 0,accelerationCar/2);
    FzSet Accelerate_Max = Acceleration.AddRightShoulderSet("Accelerate_Max", 0, accelerationCar/2, accelerationCar);
    // To-Do: revisar el new por que no se tiene que hacer
    flVelocity.AddRule( *(new FzAND(Velocity_Slow, Angle_Slow)), Accelerate_Max);
    flVelocity.AddRule( *(new FzAND(Velocity_Slow, Angle_Normal)), Accelerate_Max);
    flVelocity.AddRule( *(new FzAND(Velocity_Slow, Angle_High)), Accelerate_None);
    flVelocity.AddRule( *(new FzAND(Velocity_Normal, Angle_Slow)), Accelerate_Max);
    flVelocity.AddRule( *(new FzAND(Velocity_Normal, Angle_Normal)), Accelerate_None);
    flVelocity.AddRule( *(new FzAND(Velocity_Normal, Angle_High)), Accelerate_None);
    flVelocity.AddRule( *(new FzAND(Velocity_High, Angle_Slow)), Accelerate_Max);
    flVelocity.AddRule( *(new FzAND(Velocity_High, Angle_Normal)), Accelerate_Brake);
    flVelocity.AddRule( *(new FzAND(Velocity_High, Angle_High)), Accelerate_Brake);
    // seguimos con las pruebas
      //flVelocity.Fuzzify("ActualVelocity", 15); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
      //flVelocity.Fuzzify("Angle", 30);
      //double resultadoDefuzzification = flVelocity.DeFuzzify("Acceleration");
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

    //cout << "Angulo: " << angle*180/PI << endl;
    return angle;
}


float PhysicsAI::calculateFuzzyVelocity(float speedCar, float angle){
    flVelocity.Fuzzify("ActualVelocity", speedCar); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
    flVelocity.Fuzzify("Angle", angle);

    return flVelocity.DeFuzzify("Acceleration");  
}

void PhysicsAI::InitPhysicsIA(CarAI* car){
    fuzzyRulesVelocity(car);
}

void PhysicsAI::Update(vector<WayPoint *> wayPoints, CarAI* car, float deltaTime){
    //Guardamos en varAIbles los componentes
    auto components = car->GetComponents();
	auto cTransformable = static_cast<CTransformable*>(components[CompType::TransformableComp]);
    auto cWayPoint     = static_cast<CWayPoint*>(components[CompType::WayPointComp]);
    auto cCar        = static_cast<CCar*>(components[CompType::CarComp]);
    float angleRange = 0;
    float angle = 0;
    float radious = cWayPoint->radious;

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

        angle = calculateAngle(cWayPoint, car);
        if (angle < 0)
            angleRange = angle*(-1);
        else
            angleRange = angle;

        cout << "Angulo Positivo: " << angleRange*180/PI << endl;
        float fuzzyAceleration = calculateFuzzyVelocity(cCar->speed, angleRange);
        //Aumentamos la velocidad
        cCar->wheelRotation = angle;
        cCar->speed += fuzzyAceleration;
        if(cCar->speed > cCar->maxSpeed){
            cCar->speed = cCar->maxSpeed;
        }
    }


    cTransformable->position.x += cos(angle) * cCar->speed * deltaTime;
    cTransformable->position.z += sin(angle) * cCar->speed * deltaTime;

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
