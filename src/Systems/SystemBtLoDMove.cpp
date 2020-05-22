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
 * @author Carlos de la Fuente Torres <delafuentetorresc@gmail.com>
 * @author Antonio Jose Martinez Garcia
 * 
 */
 
 
 #include "SystemBtLoDMove.h"


#include <behaviourTree/behaviourTree.h>
#include <behaviourTree/selector.h>
#include <behaviourTree/sequence.h>
#include <behaviourTree/decorator.h>

#include <behaviourTree/Blackboard.h>

#include <Components/CBrainAI.h>
#include <Components/CNitro.h>
#include <Components/CTransformable.h>
#include <Components/CPosDestination.h>



void SystemBtLoDMove::AddManager(Manager &m) {
    managers.push_back(&m);
}

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
        //std::cout << "No power up baby" << std::endl;
        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------   CONDICIONES -----------------------------------------//
// CONDICION --> la IA se encuentra pensando?
struct IsThinking_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrain = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->thinking){
            return true;
        }
        return false;
    } 
};


// CONDICION --> usando nitro?
struct UsingNitro_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cNitro = static_cast<CNitro*>(blackboard->actualCar->GetComponent(CompType::NitroComp).get());
        if(cNitro->activePowerUp){
            return true;
        }
        return false;
    } 
};


// CONDICION --> objetivo a distancia cercana?
struct NearDestination_LoDMove : public behaviourTree {
    float distanceToObjective = 200.0;

    virtual bool run(Blackboard* blackboard) override {
        auto cTransformable = static_cast<CTransformable*>(blackboard->actualCar->GetComponent(CompType::TransformableComp).get());
        auto cPosDestination = static_cast<CPosDestination*>(blackboard->actualCar->GetComponent(CompType::PosDestination).get());
        if(glm::distance(cTransformable->position, cPosDestination->position) < distanceToObjective){
            return true;
        }

        auto cBrain = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->targetCar){
            auto cTransTarget = static_cast<CTransformable*>(cBrain->targetCar->GetComponent(CompType::TransformableComp).get());
            if(glm::distance(cTransformable->position, cTransTarget->position) < distanceToObjective){
                return true;
            }
        }
        return false;
    } 
};


// CONDICION --> tengo melon molon?
struct HaveMelonMolon_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == typeCPowerUp::MelonMolon){
            return true;
        }
        return false;
    } 
};

// CONDICION --> tengo target?
struct HaveTarget_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrain = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->targetCar){
            return true;
        }
        return false;
    } 
};


//CONDICION -> Tengo el target en vision?
struct TargetCarInVision_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        for(const auto& car : cBrainAI->carInVision){
            if(cBrainAI->targetCar == car)
                return true;
        }
        return false;
    } 
};

//CONDICION --> comprobamos si el cocheIA actual esta fuera del rango de vision del jugador para aplicar SB
struct OutofVisionRange_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTransCar = static_cast<CTransformable*>(blackboard->actualCar->GetComponent(CompType::TransformableComp).get());
        if(blackboard->manCars->systemDataVision->EntityInVisionHumanSD(cTransCar->position) == false){
            return true;
        }
        return false;
    } 
};

//CONDICION --> comprobamos si el cocheIA actual se encuentra cerca de el del jugador para aplicar FL
struct InDistanceRange_LoDMove : public behaviourTree {
    const float maxDistanceToSee = 1500.0;

    virtual bool run(Blackboard* blackboard) override {
        auto cTransformableCar = static_cast<CTransformable*>(blackboard->manCars->GetCar()->GetComponent(CompType::TransformableComp).get());
        auto cTransformableCarAI = static_cast<CTransformable*>(blackboard->actualCar->GetComponent(CompType::TransformableComp).get());
        float distanceToCarAI = glm::distance(cTransformableCar->position, cTransformableCarAI->position);

        if(distanceToCarAI < maxDistanceToSee){ // To-Do: ajustar distancia
            return true;
        }
        return false;
    } 
};



// ----------------------------------------     ACCIONES   -------------------------------------------
//ACCION --> aplicamos la accion de pensar
struct SBThink_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        cBrainAI->movementType = "Pensandoooooo";
        blackboard->steeringBehaviours->UpdateThink(blackboard->actualCar, blackboard->manNavMesh);
        return true;
    } 
};


//ACCION --> evasion de obstaculos SB
struct ObstacleAvoidance_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        bool result = blackboard->steeringBehaviours->UpdateObstacleAvoidance(blackboard->actualCar, blackboard->manBoundingOBB);
        if(result){
            //cout << "Evasion de obstaculo\n";
            auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
            cBrainAI->movementType = "Evasion de obstaculo";
            return true;
        }
        return false;
    } 
};


//ACCION --> evasion de coches SB
struct CarAvoidance_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        bool result = blackboard->steeringBehaviours->UpdateCarAvoidance(blackboard->actualCar);
        if(result){
            auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
            cBrainAI->movementType = "Evasion de coche";
            return true;
        }
        return false;
    } 
};


/*struct CollisionAvoidance_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        bool collisionWall = blackboard->steeringBehaviours->UpdateWallAvoidance(blackboard->actualCar, blackboard->manBoundingWall, blackboard->manBoundingOBB);

        if(collisionWall == false){
            bool result = blackboard->steeringBehaviours->UpdateCarAvoidance(blackboard->actualCar, blackboard->manCars);

            if(result){
                auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
                cBrainAI->movementType = "Evasion de coche";
            }
            return false;
        }else{
            auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
            cBrainAI->movementType = "Evasion de muro";
            return true;
        }
    } 
};*/


//ACCION --> aplicamos SB pursue y si esta en el angulo lanzamos el melon molon
struct SBPursue_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        float angle = blackboard->steeringBehaviours->UpdatePursuePowerUp(blackboard->actualCar, cBrainAI->targetCar);
        if(angle>=-3 && angle <=3){
            shared_ptr<DataMap> data = make_shared<DataMap>();
            (*data)[ACTUAL_CAR] = blackboard->actualCar;
            EventManager::GetInstance().AddEventMulti(Event{EventType::THROW_POWERUP_AI, data});
        }

        cBrainAI->movementType = "Steering Behaviour prediccion";
        return true;
    } 
};


//ACCION --> aplicamos SB Arrive (Seek)
struct SBArrive_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        blackboard->steeringBehaviours->UpdateArrive(blackboard->actualCar);
        //std::cout << "Aplico SB Arrive" << std::endl;
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        cBrainAI->movementType = "Steering Behaviour moverse";
        return true;
    }
};


//ACCION --> aplicamos logica difusa
struct ApplyFuzzyLogic_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        cBrainAI->fuzzyLogic->Update(blackboard->actualCar, Constants::DELTA_TIME);
        //blackboard->systemFuzzyLogicAI->Update(blackboard->actualCar, Constants::DELTA_TIME);
        cBrainAI->movementType = "Logica difusa";
        //std::cout << "Aplico FL" << std::endl;    
        return true;
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
    //fuzzyLogic = make_shared<SystemFuzzyLogicAI>();
    //entradoFL=false;
    steeringBehaviours = make_shared<SteeringBehaviours>();

    //Construir el Arblol
    selectorBehaviourTree = make_shared<selector>();

    // Condiciones
    shared_ptr<IsThinking_LoDMove> c_IsThinking = make_shared<IsThinking_LoDMove>();
    shared_ptr<UsingNitro_LoDMove> c_UsingNitro = make_shared<UsingNitro_LoDMove>();
    shared_ptr<NearDestination_LoDMove> c_NearDestination = make_shared<NearDestination_LoDMove>();
    shared_ptr<HaveMelonMolon_LoDMove> c_HaveMelonMolon =   make_shared<HaveMelonMolon_LoDMove>();
    shared_ptr<HaveTarget_LoDMove> c_HaveTarget =   make_shared<HaveTarget_LoDMove>();
    shared_ptr<TargetCarInVision_LoDMove> c_TargetCarInVision =   make_shared<TargetCarInVision_LoDMove>();
    shared_ptr<OutofVisionRange_LoDMove>  c_OutofVisionRange =   make_shared<OutofVisionRange_LoDMove>();
    shared_ptr<InDistanceRange_LoDMove>  c_InDistanceRange =   make_shared<InDistanceRange_LoDMove>();
    

    // Acciones
    shared_ptr<SBThink_LoDMove> a_SBThink = make_shared<SBThink_LoDMove>();
    shared_ptr<ObstacleAvoidance_LoDMove> a_ObstacleAvoidance = make_shared<ObstacleAvoidance_LoDMove>();
    shared_ptr<CarAvoidance_LoDMove> a_CarAvoidance = make_shared<CarAvoidance_LoDMove>();
    shared_ptr<SBPursue_LoDMove> a_SBPursue =   make_shared<SBPursue_LoDMove>();
    shared_ptr<SBArrive_LoDMove>  a_SBArrive =   make_shared<SBArrive_LoDMove>();
    shared_ptr<ApplyFuzzyLogic_LoDMove>  a_FuzzyLogic =   make_shared<ApplyFuzzyLogic_LoDMove>();

    // elements
    shared_ptr<sequence> sequence1 = make_shared<sequence>();
    shared_ptr<sequence> sequence2 = make_shared<sequence>();
    shared_ptr<Inverter_LoDMove> inverter2 = make_shared<Inverter_LoDMove>();
    shared_ptr<selector> selector2 = make_shared<selector>();
    shared_ptr<sequence> sequence3 = make_shared<sequence>();
    shared_ptr<selector> selector41 = make_shared<selector>();
    shared_ptr<sequence> sequence41 = make_shared<sequence>();
    shared_ptr<selector> selector42 = make_shared<selector>();
    shared_ptr<sequence> sequence42 = make_shared<sequence>();


    selectorBehaviourTree->addChild(sequence1);
        sequence1->addChild(c_IsThinking);
        sequence1->addChild(a_SBThink);
    
    selectorBehaviourTree->addChild(a_ObstacleAvoidance);

    selectorBehaviourTree->addChild(sequence2);
        sequence2->addChild(inverter2);
            inverter2->addChild(selector2);
                selector2->addChild(c_UsingNitro);
                selector2->addChild(c_NearDestination);
        sequence2->addChild(a_CarAvoidance);
    
    selectorBehaviourTree->addChild(sequence3);
        sequence3->addChild(c_HaveMelonMolon);
        sequence3->addChild(c_HaveTarget);
        sequence3->addChild(c_TargetCarInVision);
        sequence3->addChild(a_SBPursue);

    selectorBehaviourTree->addChild(selector41);
        selector41->addChild(sequence41);
            sequence41->addChild(c_OutofVisionRange);
            sequence41->addChild(a_SBArrive);
        selector41->addChild(selector42);
            selector42->addChild(sequence42);
                sequence42->addChild(c_InDistanceRange);
                sequence42->addChild(a_FuzzyLogic);
            selector41->addChild(a_SBArrive);

}

void SystemBtLoDMove::InitFuzzyLogic(ManCar &manCars){
    for(const auto& currentCar : manCars.GetEntities()){
        if(static_cast<Car*>(currentCar.get())->GetTypeCar() == TypeCar::CarAI){
            auto cBrain = static_cast<CBrainAI*>(currentCar.get()->GetComponent(CompType::BrainAIComp).get());
            cBrain->fuzzyLogic->InitSystemFuzzyLogicAI(static_cast<CarAI*>(currentCar.get()));
        }
    }
}


void SystemBtLoDMove::AddCLPhysicsSB(CLPhysics* clPhysics){
    steeringBehaviours->SetCLPhysics(clPhysics);
}

void SystemBtLoDMove::update(CarAI* actualCar){
    //if(entradoFL==false){
    //    fuzzyLogic->InitSystemFuzzyLogicAI(actualCar);  // To-Do: arreglar esta llamada para solo hacerla una vez
    //    entradoFL=true;
    //}

    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, static_cast<ManCar*>(managers[0]), static_cast<ManPowerUp*>(managers[1]), 
            static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManTotem*>(managers[3]), static_cast<ManWayPoint*>(managers[4]), steeringBehaviours.get(), 
            static_cast<ManNavMesh*>(managers[5]), static_cast<ManBoundingWall*>(managers[6]), static_cast<ManBoundingOBB*>(managers[7]));

    selectorBehaviourTree->run(blackboard.get());
}