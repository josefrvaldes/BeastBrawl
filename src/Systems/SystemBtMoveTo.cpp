#include "SystemBtMoveTo.h"


#include "../behaviourTree/behaviourTree.h"
#include "../behaviourTree/composite.h"
#include "../behaviourTree/selector.h"
#include "../behaviourTree/sequence.h"
#include "../behaviourTree/decorator.h"

#include "../behaviourTree/Blackboard.h"
//#include "../Components/CPowerUp.h"
#include "../Components/CTotem.h"
#include "../Components/CCurrentNavMesh.h"
#include "../Components/CTargetNavMesh.h"


#include "../Entities/Totem.h"
#include "../Managers/ManTotem.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
///// DECORATORS //////

// Decorator Minimun
struct Minimum_mt : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
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
struct Limit_mt : public Decorator {  // Decorator Limit
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
struct UntilFail_mt : public Decorator {  // Decorator UntilFail
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
struct Inverter_mt : public Decorator {  // Decorator Inverter
    virtual bool run(Blackboard* blackboard) override {
        return !(getChild()->run(blackboard));
    }
};


//Afirmacion No tenemos power up!
struct DontHavePoweUp_mt : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        std::cout << "No power up baby" << std::endl;
        return true;
    }
};
// ----------------------------------------   CONDICIONES -------------------------------------------
//CONDICION Tenemos powerUp?
struct HavePowerUp_mt : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //return true;
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if(cPowerUp->typePowerUp == typeCPowerUp::None){
            //std::cout << "No tenemos powerUp" << std::endl;
            return false;
        }
        return true;
    }
};
//CONDICION esa el Totem suelto por el mapa?
struct TotemAlone_mt : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        if( blackboard->manTotems->GetEntities().size() > 0){
            //std::cout << "Si esta el totem en el suelo locoooooooooooooooooooooooooooooooooooooooooooooooo" << std::endl;
            return true;
        }
        return false;
    }
};
//CONDICION Tenemos un powerUp de ataque? Melon o Banana
struct HavePowerUpAttack_mt : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        return true;
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == typeCPowerUp::MelonMolon || 
            cPowerUp->typePowerUp == typeCPowerUp::TeleBanana   ){
            //std::cout << "El powerUp era o Melon o Banana" << std::endl;
            return true;
        }
        return false;
    }
};

// ----------------------------------------     ACCIONES   -------------------------------------------
//ACCION movernos al totem
struct MoveToTotem_mt : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        if( blackboard->manTotems->GetEntities().size() > 0){
            //std::cout << "La IA va a por el Totem que esta en el suelo" << std::endl;
            auto cCurrentMeshCar = static_cast<CCurrentNavMesh*>(blackboard->actualCar->GetComponent(CompType::CurrentNavMeshComp).get());
            auto cCurrentMeshTotem = static_cast<CCurrentNavMesh*>(blackboard->manTotems->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());
            if(cCurrentMeshCar->currentNavMesh == cCurrentMeshTotem->currentNavMesh){
                auto cTransformable = static_cast<CTransformable*>(blackboard->manTotems->GetEntities()[0].get()->GetComponent(CompType::TransformableComp).get());
                shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                DataMap dataTotem;                                                                           
                dataTotem["actualCar"] = blackboard->actualCar;              // nos guardamos el puntero para eliminar el powerUp
                dataTotem["posDestination"] = cTransformable->position;              // nos guardamos el puntero al coche                              
                eventManager->AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataTotem}); 
                return true;
            }else{
                auto cTargetMeshCar = static_cast<CTargetNavMesh*>(blackboard->actualCar->GetComponent(CompType::TargetNavMeshComp).get());
                cTargetMeshCar->targetNavMesh = cCurrentMeshTotem->currentNavMesh;
                // Actualmente solo nos movemos entre waypoints hasta coincidir en el mismo navMesh
                //std::cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;
                shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                DataMap dataPowerUp;       
                dataPowerUp["actualCar"] = blackboard->actualCar;     
                dataPowerUp["manWayPoints"] = blackboard->manWayPoint;                                                                                                      
                eventManager->AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
                return true;
            }
        }
        return false;
    }
};

//ACCION movernos a un powerUp (seguir en el random que teniamos antes de ir de power up en power up)
// TODO ... para hacerlo mas autonomo --> nos moveremos aun powerUp aleatorio del mapa.. mas adelante sera al mas cercano/ al que este mirando
struct MoveToPowerUp_mt : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std::cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;
        shared_ptr<EventManager> eventManager = EventManager::GetInstance();
        DataMap dataPowerUp;       
        dataPowerUp["actualCar"] = blackboard->actualCar;     
        dataPowerUp["manWayPoints"] = blackboard->manWayPoint;                                                                                                      
        eventManager->AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
        return true;
    }
};
//ACCION movernos al jugador con Totem 
struct MoveToCarTotem_mt : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        for(auto actualAI : blackboard->manCars->GetEntitiesAI()){
            auto cTotem = static_cast<CTotem*>(actualAI.get()->GetComponent(CompType::TotemComp).get());
            // TODO actualmente debemsos hacer que si una IA tiene el Totem no se siga a si misma... quedaria parada
            if(cTotem->active == true){
                auto cCurrentMeshCar = static_cast<CCurrentNavMesh*>(blackboard->actualCar->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cCurrentMeshCarAI = static_cast<CCurrentNavMesh*>(actualAI.get()->GetComponent(CompType::CurrentNavMeshComp).get());
                if(cCurrentMeshCar->currentNavMesh == cCurrentMeshCarAI->currentNavMesh){
                    //if(blackboard->actualCar != actualAI.get()){
                        auto cTransformable = static_cast<CTransformable*>(actualAI.get()->GetComponent(CompType::TransformableComp).get());
                        shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                        DataMap dataCarTotem;                                                                           
                        dataCarTotem["actualCar"] = blackboard->actualCar;             
                        dataCarTotem["posDestination"] = cTransformable->position;                                        
                        eventManager->AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataCarTotem}); 
                        return true;
                    //}else{
                    //    std::cout << "somos una IA que tiene el powerUp y obvimanete no nos vamos a perseguir a nosotros mismos" << std::endl;
                    //    return false;
                    //}
                }else{
                    // Actualmente solo nos movemos entre waypoints hasta coincidir en el mismo navMesh
                    //std::cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;
                    shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                    DataMap dataPowerUp;       
                    dataPowerUp["actualCar"] = blackboard->actualCar;     
                    dataPowerUp["manWayPoints"] = blackboard->manWayPoint;                                                                                                      
                    eventManager->AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
                    return true;
                }
            }
        }
        return false;     
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
SystemBtMoveTo::SystemBtMoveTo(){

    //   CREACION DEL ARBOL DE DECISIONES

    selectorBehaviourTree = make_shared<selector>();


    shared_ptr<sequence> sequence1 = make_shared<sequence>();
        shared_ptr<TotemAlone_mt> c_totemAlone =   make_shared<TotemAlone_mt>();
        shared_ptr<MoveToTotem_mt> a_moveToTotem = make_shared<MoveToTotem_mt>();

    shared_ptr<sequence> sequence2 =    make_shared<sequence>();
        shared_ptr<Inverter_mt> m_inverter1 =  make_shared<Inverter_mt>();
            shared_ptr<HavePowerUp_mt> c_havePowerUp = make_shared<HavePowerUp_mt>(); 
        shared_ptr<MoveToPowerUp_mt> a_moveToPowerUp = make_shared<MoveToPowerUp_mt>(); 

    shared_ptr<sequence> sequence3  = make_shared<sequence>();
        shared_ptr<HavePowerUpAttack_mt> c_havePowerUpAttack = make_shared<HavePowerUpAttack_mt>();
        shared_ptr<MoveToCarTotem_mt> a_moveToCarTotem =       make_shared<MoveToCarTotem_mt>();


    std::cout << "------------------------ Behaviour Move To --------------------------------" << std::endl;
    // ASIGNACION DEL ARBOL DE DECISIONES

    selectorBehaviourTree->addChild(sequence1);
    selectorBehaviourTree->addChild(sequence2);
    selectorBehaviourTree->addChild(sequence3);
    // TODO somos una IA que tiene el powerUp y obvimanete no nos vamos a perseguir a nosotros mismos
    //selectorBehaviourTree->addChild(a_moveToPowerUp);

    sequence1->addChild(c_totemAlone);
    sequence1->addChild(a_moveToTotem);

    sequence2->addChild(m_inverter1);
        m_inverter1->addChild(c_havePowerUp);
    sequence2->addChild(a_moveToPowerUp);

    sequence3->addChild(c_havePowerUpAttack);
    sequence3->addChild(a_moveToCarTotem);
    
}




void SystemBtMoveTo::update(CarAI* actualCar, ManCar* manCars,ManPowerUp* manPowerUps, ManBoxPowerUp* manBoxPowerUps, ManTotem* manTotems, ManWayPoint* manWayPoint){
    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, manCars, manPowerUps, manBoxPowerUps, manTotems, manWayPoint);
    selectorBehaviourTree->run(blackboard.get());
}