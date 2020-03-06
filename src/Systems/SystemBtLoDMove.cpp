#include "SystemBtLoDMove.h"


#include <behaviourTree/behaviourTree.h>
#include <behaviourTree/selector.h>
#include <behaviourTree/sequence.h>
#include <behaviourTree/decorator.h>

#include <behaviourTree/Blackboard.h>

#include <Components/CMovementType.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
///// DECORATORS //////

// Decorator Minimun
struct Minimum_LoDMove : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
    uint32_t totalTries = 3;
    uint32_t numTries = 0;
    virtual bool run(Blackboard* blackboard) override {
        if (numTries >= totalTries)
            return getChild()->run(blackboard);
        numTries++;
        //cout << "Fallamos al coger la llave, intento: " << numTries << endl;
        return false;
    }
};

// Decorator Limit
struct Limit_LoDMove : public Decorator {  // Decorator Limit
    uint32_t totalLimit = 3;
    uint32_t numLimit = 0;
    virtual bool run(Blackboard* blackboard) override {
        if (numLimit >= totalLimit)
            return false;
        numLimit++;
        return getChild()->run(blackboard);
    }
};

// Decorator UntilFail
struct UntilFail_LoDMove : public Decorator {  // Decorator UntilFail
    virtual bool run(Blackboard* blackboard) override {
        while (true) {
            bool result = getChild()->run(blackboard);
            if (!result) {
                break;
            }
        }
        return true;
    }
};

// Decorator Inverter
struct Inverter_LoDMove : public Decorator {  // Decorator Inverter
    virtual bool run(Blackboard* blackboard) override {
        return !(getChild()->run(blackboard));
    }
};


//Afirmacion No tenemos power up!
struct DontHavePoweUp_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        std::cout << "No power up baby" << std::endl;
        return true;
    }
};
// ----------------------------------------   CONDICIONES -------------------------------------------
// CONDICION --> tengo melon molon
struct HaveMelonMolon_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == typeCPowerUp::MelonMolon){
            return true;
        }
        return false;
    } 
};

//CONDICION --> comprobamos si el cocheIA actual esta fuera del rango de vision del jugador para aplicar SB
struct OutofVisionRange_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        if(blackboard->manCars->carInVisionRange(blackboard->manCars->GetCar().get(), blackboard->actualCar, 75) == false){
            return true;
        }
        return false;
    } 
};

//CONDICION --> comprobamos si el cocheIA actual se encuentra cerca de el del jugador para aplicar FL
struct InDistanceRange_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTransformableCar = static_cast<CTransformable*>(blackboard->manCars->GetCar()->GetComponent(CompType::TransformableComp).get());
        auto cTransformableCarAI = static_cast<CTransformable*>(blackboard->actualCar->GetComponent(CompType::TransformableComp).get());
        float vetorDistanceX = (cTransformableCarAI->position.x - cTransformableCar->position.x);
        float vetorDistanceZ = (cTransformableCarAI->position.x - cTransformableCar->position.z);
        float distanceToCarAI = sqrt(vetorDistanceX*vetorDistanceX + vetorDistanceZ*vetorDistanceZ);

        if(distanceToCarAI < 2000.0){ // To-Do: ajustar distancia
            return true;
        }
        return false;
    } 
};



// ----------------------------------------     ACCIONES   -------------------------------------------
//ACCION --> aplicamos SB pursue y si esta en el angulo lanzamos el melon molon
struct SBPursue_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //return blackboard->steeringBehaviours->UpdateObstacleAvoidance(blackboard->actualCar, blackboard->manPowerUps);
        //std::cout << gola << std::endl;
        //return true;
        Entity* desirableTarget = blackboard->manCars->GetDesirableTarget(blackboard->actualCar);
        if(blackboard->steeringBehaviours->IsTargeteable(blackboard->actualCar, desirableTarget, blackboard->manBoundingWall) == false)
            return false;
        float angle = blackboard->steeringBehaviours->UpdatePursuePowerUp(blackboard->actualCar, desirableTarget);  // To-Do: calcular coche a por el que se quiere ir
        if(angle>=-3 && angle <=3){
            shared_ptr<DataMap> data = make_shared<DataMap>();

            (*data)[ACTUAL_CAR] = blackboard->actualCar;
            EventManager::GetInstance().AddEventMulti(Event{EventType::THROW_POWERUP_AI, data});
        }
        //std::cout << "Aplico SB pursuePU" << std::endl;

        auto cMovementType = static_cast<CMovementType*>(blackboard->actualCar->GetComponent(CompType::MovementComp).get());
        cMovementType->movementType = "Steering Behaviour prediccion";
        return true;
    } 
};


//ACCION --> aplicamos SB Arrive (Seek)
struct SBArrive_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        blackboard->steeringBehaviours->UpdateArrive(blackboard->actualCar);
        //std::cout << "Aplico SB Arrive" << std::endl;
        auto cMovementType = static_cast<CMovementType*>(blackboard->actualCar->GetComponent(CompType::MovementComp).get());
        cMovementType->movementType = "Steering Behaviour moverse";
        return true;
    }
};


//ACCION --> aplicamos logica difusa
struct ApplyFuzzyLogic_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        blackboard->systemFuzzyLogicAI->Update(blackboard->actualCar, Constants::DELTA_TIME);
        auto cMovementType = static_cast<CMovementType*>(blackboard->actualCar->GetComponent(CompType::MovementComp).get());
        cMovementType->movementType = "Logica difusa";
        //std::cout << "Aplico FL" << std::endl;    
        return true;
    } 
};


struct CollisionAvoidance_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        bool collisionWall = blackboard->steeringBehaviours->UpdateWallAvoidance(blackboard->actualCar, blackboard->manBoundingWall);

        if(collisionWall == false){
            bool result = blackboard->steeringBehaviours->UpdateObstacleAvoidance(blackboard->actualCar, blackboard->manCars);

            if(result){
                auto cMovementType = static_cast<CMovementType*>(blackboard->actualCar->GetComponent(CompType::MovementComp).get());
                cMovementType->movementType = "Evasion de jugador";
            }
            return result;
        }else{
            auto cMovementType = static_cast<CMovementType*>(blackboard->actualCar->GetComponent(CompType::MovementComp).get());
            cMovementType->movementType = "Evasion de muro";
            return true;
        }
    } 
};


// NOTA: recordar que tenemos el otro Behaviour tree de tirar powerUp y por tanto lo seguiremos
// hasta que lo veamos, una vez visto le lanzaemos el powerUp e iremos a por el Totem u otro PowerUp

//    None,               // 0
//    RoboJorobo,         // 1
//    SuperMegaNitro,     // 2
//    PudinDeFrambuesa,   // 3
//    EscudoMerluzo,      // 4
//    TeleBanana,         // 5
//    MelonMolon          // 6




SystemBtLoDMove::SystemBtLoDMove(){
    // Sistemas de movimiento
    fuzzyLogic = make_shared<SystemFuzzyLogicAI>();
    steeringBehaviours = make_shared<SteeringBehaviours>();
    entradoFL=false;
    
    //Construir el Arblol
    //   Lanzar melon molon = true   -> Aplicar Steering de movimiento y lanzar en caso de que angulo sea 0
    
    //                 SELECTOR
    //    ////////////////////////////////////////////////////////////////////////////
    // CollAvoidance        sequence1                                               Selector(Vision LoD)
    //   (action)              //                                                       //
    //                    //////////////                                //////////////////////////////
    //                    //          //                                //                          //
    //                  Have MM?     ApplySB_Pursue                   Sequence2                     Selector (Distance - dentro del rango de vision)
    //                                                                //      //                       //
    //                                                  OutOfVisionRange   ApplySB_Seek          ///////////////////////////
    //                                                                                           //                      //
    //                                                                                        Sequence3                 ApplySB_Seek (lejos)
    //                                                                                       //      //                 
    //                                                                                    Cerca?     LogicaDifusa  


    selectorBehaviourTree = make_shared<selector>();

    shared_ptr<CollisionAvoidance_LoDMove> a_CollisionAvoidance = make_shared<CollisionAvoidance_LoDMove>();
    shared_ptr<sequence> sequence1 = make_shared<sequence>();
    shared_ptr<selector> selectorVision = make_shared<selector>();

    shared_ptr<HaveMelonMolon_LoDMove> c_HaveMelonMolon =   make_shared<HaveMelonMolon_LoDMove>();
    shared_ptr<SBPursue_LoDMove> a_SBPursue =   make_shared<SBPursue_LoDMove>();

    shared_ptr<sequence> sequence2 = make_shared<sequence>();
    shared_ptr<selector> selectorDistance = make_shared<selector>();

    shared_ptr<OutofVisionRange_LoDMove>  c_OutofVisionRange =   make_shared<OutofVisionRange_LoDMove>();
    shared_ptr<SBArrive_LoDMove>  a_SBArrive =   make_shared<SBArrive_LoDMove>();

    shared_ptr<sequence> sequence3 = make_shared<sequence>();

    shared_ptr<InDistanceRange_LoDMove>  c_InDistanceRange =   make_shared<InDistanceRange_LoDMove>();
    shared_ptr<ApplyFuzzyLogic_LoDMove>  a_FuzzyLogic =   make_shared<ApplyFuzzyLogic_LoDMove>();

    selectorBehaviourTree->addChild(a_CollisionAvoidance);
    selectorBehaviourTree->addChild(sequence1);
    selectorBehaviourTree->addChild(selectorVision);

    sequence1->addChild(c_HaveMelonMolon);
    sequence1->addChild(a_SBPursue);

    selectorVision->addChild(sequence2);
    selectorVision->addChild(selectorDistance);

    sequence2->addChild(c_OutofVisionRange);
    sequence2->addChild(a_SBArrive);

    selectorDistance->addChild(sequence3);
    selectorDistance->addChild(a_SBArrive);

    sequence3->addChild(c_InDistanceRange);
    sequence3->addChild(a_FuzzyLogic);

}




void SystemBtLoDMove::update(CarAI* actualCar, ManCar* manCars,ManPowerUp* manPowerUps, ManBoxPowerUp* manBoxPowerUps, ManTotem* manTotems, ManWayPoint* manWayPoint, ManNavMesh* manNavMesh, ManBoundingWall* m_manBoundingWall){
    if(entradoFL==false){
        fuzzyLogic->InitSystemFuzzyLogicAI(actualCar);  // To-Do: arreglar esta llamada para solo hacerla una vez
        entradoFL=true;
    }

    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, manCars, manPowerUps, manBoxPowerUps, manTotems, manWayPoint, fuzzyLogic.get(), steeringBehaviours.get(),manNavMesh, m_manBoundingWall);

    selectorBehaviourTree->run(blackboard.get());
}