#include "SystemBtDecisionMove.h"


#include <behaviourTree/behaviourTree.h>
#include <behaviourTree/selector.h>
#include <behaviourTree/sequence.h>
#include <behaviourTree/decorator.h>

#include <behaviourTree/Blackboard.h>
#include <Components/CTotem.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CBrainAI.h>

void SystemBtDecisionMove::AddManager(Manager &m) {
    managers.push_back(&m);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
///// DECORATORS //////
struct Facil2times_dm : public Decorator {
    uint32_t totalTried = 2;
    uint32_t numTried = 0;
    virtual bool run(Blackboard* blackboard) override {
        if (numTried >= totalTried)
            return false;
        numTried++;
        return getChild()->run(blackboard);
    }
};

// Decorator Inverter
struct Inverter_dm : public Decorator {  // Decorator Inverter
    virtual bool run(Blackboard* blackboard) override {
        return !(getChild()->run(blackboard));
    }
};
// ----------------------------------------   CONDICIONES -------------------------------------------
//CONDICION Tenemos powerUp?
struct HavePowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if(cPowerUp->typePowerUp != typeCPowerUp::None){
            return true;
        }
        return false;
    }
};
//CONDICION esa el Totem suelto por el mapa?
struct TotemAlone_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTotem = static_cast<CTotem*>(blackboard->manTotems->GetEntities()[0]->GetComponent(CompType::TotemComp).get());
        return cTotem->active;
    }
};

struct TotemOtherCar_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        for(const auto& actualAI : blackboard->manCars->GetEntities()){
            if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
                auto cTotemAI = static_cast<CTotem*>(actualAI->GetComponent(CompType::TotemComp).get());
                if(cTotemAI->active == true){
                    return true;
                }
            }
        }
        return false;  
    }
};
//CONDICION Tenemos un powerUp de ataque? Melon o Banana
struct HavePowerUpAttack_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == typeCPowerUp::MelonMolon || 
            cPowerUp->typePowerUp == typeCPowerUp::TeleBanana ||
            cPowerUp->typePowerUp == typeCPowerUp::SuperMegaNitro  ){
            //std::cout << "El powerUp era o Melon o Banana" << std::endl;
            return true;
        }
        return false;
    }
};

struct HaveTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cTotem = static_cast<CTotem*>(actualCar->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            // nosotros tenemos el totem
            return true;
        }
        return false;
    }
};

// TODO: IMPLEMENTADO - No probado 
struct SeeTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->totemInVision)
            return true;
        return false;
    }
};

// TODO: IMPLEMENTADO - No probado
struct SeePowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->boxInVision.size() > 0)
            return true;
        return false;
    }
};

// TODO: IMPLEMENTADO -> No probado
struct Think_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        cBrain->thinking = true;
        return true;
    }
};

// TODO: IMPLEMENTADO -> No probado
struct NoThink_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        cBrain->thinking = false;
        return true;
    }
};

// TODO: IMPLEMENTADO - No probado
struct NoCriticalTime_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        // cogemos el tiempo que llevamos el totem y vemos si supera el tiempo critico
        for(const auto& actualAI : blackboard->manCars->GetEntities()){
            if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
                auto cTotemAI = static_cast<CTotem*>(actualAI->GetComponent(CompType::TotemComp).get());
                if(cTotemAI->active && (cTotemAI->accumulatedTime/1000.0 + 10.0 < cTotemAI->DURATION_TIME/1000.0) ){
                    // si el tiempo actual + el critco es menor al total = no estamos en tiempo critico
                    return true;
                }
            }
        }
        return false;
    }
};

// Condicion para saber si estamos en el mismo navmesh que el totem
struct SameNavMeshCT_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cCurrendNavMeshCar = static_cast<CCurrentNavMesh*>(blackboard->actualCar->GetComponent(CompType::CurrentNavMeshComp).get());
        auto cCurrentNavMeshTotem = static_cast<CCurrentNavMesh*>(blackboard->manTotems->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());
        if(cCurrendNavMeshCar->currentNavMesh == cCurrentNavMeshTotem->currentNavMesh){
            // estamos en el mismo navmesh que el totem
            return true;
        }else{
            // no estamos en el mismo navmesh
            return false;
        }
    }
};

// TODO: IMPLEMENTADO  -> No probado -> es si tenemos un coche en ranfo de vision, es decir, coger el ocmponenre brain y comprobarlo
struct SeeCar_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->carInVision.size() > 0)
            return true;
        return false;
    }
};

// TODO: IMPLEMENTADO  -> No probado -> es si tenemos un coche en ranfo de vision y tiene el totem 
struct SeeCarTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->carInVision.size() > 0){
            for(const auto& currentCar : cBrain->carInVision){
                auto cTotemCar = static_cast<CTotem*>(currentCar->GetComponent(CompType::TotemComp).get());
                if(cTotemCar->active == true){
                    return true;
                }
            }
        }
        return false;
    }
};

// ----------------------------------------     ACCIONES   -------------------------------------------
//ACCION movernos al totem
struct MoveToTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTransTotem = static_cast<CTransformable*>(blackboard->manTotems->GetEntities()[0].get()->GetComponent(CompType::TransformableComp).get());
        //DataMap* dataTotem = new DataMap();
        shared_ptr<DataMap> dataTotem = make_shared<DataMap>();                                                                
        (*dataTotem)[ACTUAL_CAR] = blackboard->actualCar;              
        (*dataTotem)[POS_DESTINATION] = cTransTotem->position;                                        
        EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataTotem}); 
        return true;
    }
};

// TODO: Con la nueva estructura el manager de pahtplanning yan tiene los managers no hay que pasarselos.
struct MovNavMeshTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //auto cCurrendNavMeshCar = static_cast<CCurrentNavMesh*>(blackboard->actualCar->GetComponent(CompType::CurrentNavMeshComp).get());
        auto cCurrentNavMeshTotem = static_cast<CCurrentNavMesh*>(blackboard->manTotems->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrainAI->targetNavMesh != cCurrentNavMeshTotem->currentNavMesh){
            cBrainAI->targetNavMesh = cCurrentNavMeshTotem->currentNavMesh;
            shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();      
            (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
            (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;
            (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;  
            (*dataPowerUp)[ID_DESTINATION] = cCurrentNavMeshTotem->currentNavMesh;                                                                                                   
            EventManager::GetInstance().AddEventMulti(Event{EventType::CALCULATE_PATH_TO_NAVMESH, dataPowerUp}); 
        }  
        return true;
    }
};
// TODO: IMPLEMENTADO PARCIAL -> QUEDA LA OTRA PARTE
struct MoveRandom_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        std::cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;    
        shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();
        (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;  
        (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;                                                                                                      
        EventManager::GetInstance().AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
        return true;
    }
};
// TODO: NO IMPLEMENTADO COMPLETO -> indicar, con otro evento, que debe ser un powerUp del NavMesh central locoooo
struct MoveRandomCentral_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {  
        shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();
        (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;  
        (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;                                                                                                      
        EventManager::GetInstance().AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
        return true;
    }
};

// TODO: NO IMPLEMENTADO  -> movernos al coche que estamos viendo delante nuestra
// accedemos aqui cuando el tiempo no es critico, y tenemos coches en el rango de vision
// importante recordar que vienen ordenados por el FOV y por tanto debo coger el primero
struct MovCarView_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override { 
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
            auto carFollow = static_cast<Car*>(cBrain->carInVision[0]);
            auto cTransCarFollow = static_cast<CTransformable*>(carFollow->GetComponent(CompType::TransformableComp).get());
        // lo primero actualizamos el targetCar
        cBrain->targetCar = carFollow;
        // y despues nos dirigimos a el -> A MATARLO, por ser CALVO
        shared_ptr<DataMap> dataFollow = make_shared<DataMap>();
        (*dataFollow)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataFollow)[POS_DESTINATION] = cTransCarFollow->position;                                        
        EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataFollow}); 

        return true;
    }
};

// TODO: IMPLEMENTADO  - No probado -> movernos al coche que estamos viendo delante y tiene el totem
struct MovCarViewTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override { 
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->carInVision.size() > 0){
            for(const auto& currentCar : cBrain->carInVision){
                auto cTotemCar = static_cast<CTotem*>(currentCar->GetComponent(CompType::TotemComp).get());
                if(cTotemCar->active == true){
                    auto cTransCarFollow = static_cast<CTransformable*>(currentCar->GetComponent(CompType::TransformableComp).get());
                    // lo primero actualizamos el targetCar
                    cBrain->targetCar = static_cast<Car*>(currentCar);
                    // y despues nos dirigimos a el -> A MATARLO, por ser CALVO
                    shared_ptr<DataMap> dataFollow = make_shared<DataMap>();
                    (*dataFollow)[ACTUAL_CAR] = blackboard->actualCar;     
                    (*dataFollow)[POS_DESTINATION] = cTransCarFollow->position;                                        
                    EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataFollow}); 
                    return true;
                }
            }
        }
        return false;
    }
};

// TODO: IMPLEMENTADO  -> No probado
struct MovViewPowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override { 
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        auto boxSize = cBrainAI->boxInVision.size();
        if(boxSize > 0){
            auto randBox = rand() % boxSize;
            auto cTransBoxPU = static_cast<CTransformable*>(cBrainAI->boxInVision[randBox]->GetComponent(CompType::TransformableComp).get());

            shared_ptr<DataMap> dataBoxPU = make_shared<DataMap>();
            (*dataBoxPU)[ACTUAL_CAR] = blackboard->actualCar;     
            (*dataBoxPU)[POS_DESTINATION] = cTransBoxPU->position;                                        
            EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataBoxPU}); 

            return true;
        }
        return false;
    }
};

//ACCION huir ya que tenemos el totem (seguir en el random que teniamos antes de ir de power up en power up, de momento)
// TODO: como no esta implementado actualmente nos movemos entre los powerUps para no quedarnos parados
struct EscapeWithTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std::cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;    
        shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();
        (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;  
        (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;                                                                                                      
        EventManager::GetInstance().AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
        return true;
    }
};
//ACCION movernos a un powerUp (seguir en el random que teniamos antes de ir de power up en power up)
// TODO: ... para hacerlo mas autonomo --> nos moveremos aun powerUp aleatorio del mapa.. mas adelante sera al mas cercano/ al que este mirando
struct MoveToPowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std::cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;
        shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();      
        (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;  
        (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;                                                                                                      
        EventManager::GetInstance().AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
        return true;
    }
};
//ACCION movernos al jugador con Totem 
struct MoveToCarTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        for(const auto& actualAI : blackboard->manCars->GetEntities()){
            if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
                auto cTotem = static_cast<CTotem*>(actualAI->GetComponent(CompType::TotemComp).get());
                if(cTotem->active == true){
                    auto cCurrendNavMeshCarAI = static_cast<CCurrentNavMesh*>(actualAI->GetComponent(CompType::CurrentNavMeshComp).get());
                    auto cBrainAI = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
                    if(cBrainAI->targetNavMesh != cCurrendNavMeshCarAI->currentNavMesh){
                        cBrainAI->targetNavMesh = cCurrendNavMeshCarAI->currentNavMesh;

                        shared_ptr<DataMap> dataNavCar = make_shared<DataMap>();      
                        (*dataNavCar)[ACTUAL_CAR] = actualCar;     
                        (*dataNavCar)[MAN_WAYPOINTS] = blackboard->manWayPoint;
                        (*dataNavCar)[MAN_NAVMESH] = blackboard->manNavMesh;  
                        (*dataNavCar)[ID_DESTINATION] = cCurrendNavMeshCarAI->currentNavMesh;                                                                                                   
                        EventManager::GetInstance().AddEventMulti(Event{EventType::CALCULATE_PATH_TO_NAVMESH, dataNavCar}); 
                    }  
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
SystemBtDecisionMove::SystemBtDecisionMove(){

    //   CREACION DEL ARBOL DE DECISIONES

    selectorBehaviourTree = make_unique<selector>();

    shared_ptr<selector> selector1                                                  = make_unique<selector>();
    // selector TOTEM EN EL SUELO
        shared_ptr<sequence> sequence1_1                                            = make_unique<sequence>();
            shared_ptr<TotemAlone_dm> c_TotemAlone                                  = make_unique<TotemAlone_dm>();
            shared_ptr<selector> selector11                                         = make_unique<selector>();
                shared_ptr<sequence> sequence11_1                                   = make_unique<sequence>();
                    shared_ptr<SeeTotem_dm> c_SeeTotem                              = make_unique<SeeTotem_dm>();
                    shared_ptr<MoveToTotem_dm> a_MoveToTotem                        = make_unique<MoveToTotem_dm>();
                shared_ptr<sequence> sequence11_2                                   = make_unique<sequence>();
                    shared_ptr<SameNavMeshCT_dm> c_SameNavMeshCT                    = make_unique<SameNavMeshCT_dm>();
                    //shared_ptr<MoveToTotem_dm> a_MoveToTotem                      = make_unique<MoveToTotem_dm>(); --> no vamos a crear dos acciones iguales
                shared_ptr<selector> selector111                                    = make_unique<selector>();
                    shared_ptr<sequence> sequence111_1                              = make_unique<sequence>(); 
                        shared_ptr<SeePowerUp_dm> c_SeePowerUp                      = make_unique<SeePowerUp_dm>();
                        shared_ptr<MovViewPowerUp_dm> a_MovViewPowerUp              = make_unique<MovViewPowerUp_dm>();
                    shared_ptr<selector> selector1111                               = make_unique<selector>(); 
                        shared_ptr<Facil2times_dm> d_Facil2TimesMT                  = make_unique<Facil2times_dm>();
                            shared_ptr<sequence> sequence1111_1                     = make_unique<sequence>();
                                shared_ptr<NoThink_dm> a_NoThinck                   = make_unique<NoThink_dm>(); 
                                shared_ptr<MovNavMeshTotem_dm> a_MoveToNavMeshTotem = make_unique<MovNavMeshTotem_dm>();
                        shared_ptr<Think_dm> a_Thinck                               = make_unique<Think_dm>(); 
    // selector TOTEM ALGUIEN
        shared_ptr<sequence> sequence1_2                                            = make_unique<sequence>();
            shared_ptr<TotemOtherCar_dm> c_TotemOtherCar                            = make_unique<TotemOtherCar_dm>();
            shared_ptr<selector> selector12                                         = make_unique<selector>();
                shared_ptr<sequence> sequence12_1                                   = make_unique<sequence>();
                    shared_ptr<HavePowerUpAttack_dm> c_PowerUpAttack                = make_unique<HavePowerUpAttack_dm>();
                    shared_ptr<selector> selector121                                = make_unique<selector>();
                        shared_ptr<sequence> sequence121_1                          = make_unique<sequence>();
                            shared_ptr<SeeCar_dm> c_SeeCar                          = make_unique<SeeCar_dm>();
                            shared_ptr<selector> selector122                        = make_unique<selector>();
                                shared_ptr<sequence> sequence122_1                  = make_unique<sequence>();
                                    shared_ptr<NoCriticalTime_dm> c_NoCriticalTime  = make_unique<NoCriticalTime_dm>();
                                    shared_ptr<MovCarView_dm> a_MovCarView          = make_unique<MovCarView_dm>();
                                shared_ptr<sequence> sequence122_2                  = make_unique<sequence>();
                                    shared_ptr<SeeCarTotem_dm> c_SeeCarTotem        = make_unique<SeeCarTotem_dm>();
                                    shared_ptr<MovCarViewTotem_dm> a_MovCarViewTotem= make_unique<MovCarViewTotem_dm>();
                        shared_ptr<selector> selector1211                           = make_unique<selector>();
                                shared_ptr<Facil2times_dm> d_Facil2TimesMC          = make_unique<Facil2times_dm>();
                                    shared_ptr<sequence> sequence1211_1             = make_unique<sequence>();  
                                        //shared_ptr<NoThink_dm> a_NoThinck         = make_unique<NoThink_dm>();                   
                                        shared_ptr<MoveRandom_dm> a_MoveRandom      = make_unique<MoveRandom_dm>();   
                                //shared_ptr<Think_dm> a_Thinck                     = make_unique<Think_dm>();
                shared_ptr<selector> selector123                                    = make_unique<selector>();
                    shared_ptr<sequence> sequence123_2                              = make_unique<sequence>();
                        shared_ptr<Inverter_dm> m_Inverter1                         = make_unique<Inverter_dm>();
                            shared_ptr<HavePowerUp_dm> c_HavePowerUp                = make_unique<HavePowerUp_dm>(); 
                        shared_ptr<selector> selector1234                           = make_unique<selector>();
                            shared_ptr<sequence> sequence1234_1                     = make_unique<sequence>();
                                //shared_ptr<SeePowerUp_dm> c_SeePowerUp            = make_unique<SeePowerUp_dm>();
                                //shared_ptr<MovViewPowerUp_dm> a_MovViewPowerUp    = make_unique<MovViewPowerUp_dm>();
                          //shared_ptr<MoveRandom_dm> a_MoveRandom                  = make_unique<MoveRandom_dm>();
                  //shared_ptr<MoveRandom_dm> a_MoveRandom                          = make_unique<MoveRandom_dm>();
    // selector YO TOTEM
    shared_ptr<selector> selector2                                                  = make_unique<selector>();
        shared_ptr<sequence> sequence2_1                                            = make_unique<sequence>();
            //shared_ptr<CriticalTime_dm> c_CriticalTime                            = make_unique<CriticalTime_dm>();
            //shared_ptr<MoveRandom_dm> a_MoveRandom                                = make_unique<MoveRandom_dm>();
        shared_ptr<MoveRandomCentral_dm> a_MoveRandomCentral                        = make_unique<MoveRandomCentral_dm>();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // CONSTRUCCION DEL ARBOL

    selectorBehaviourTree->addChild(selector1);//hecho
        selector1->addChild(sequence1_1);//hecho
            sequence1_1->addChild(c_TotemAlone);//hecho
            sequence1_1->addChild(selector11);//hecho
                selector11->addChild(sequence11_1);//hecho
                    sequence11_1->addChild(c_SeeTotem);//hecho
                    sequence11_1->addChild(a_MoveToTotem);//hecho
                selector11->addChild(sequence11_2);//hecho
                    sequence11_2->addChild(c_SameNavMeshCT);//hecho
                    sequence11_2->addChild(a_MoveToTotem);//hecho
                selector11->addChild(selector111);//hecho
                    selector111->addChild(sequence111_1);//hecho
                        sequence111_1->addChild(c_SeePowerUp);//hecho
                        sequence111_1->addChild(a_MovViewPowerUp);//hecho
                    selector111->addChild(selector1111);//hecho
                        selector1111->addChild(d_Facil2TimesMT);//hecho
                            d_Facil2TimesMT->addChild(sequence1111_1);//hecho
                                sequence1111_1->addChild(a_NoThinck);//hecho
                                sequence1111_1->addChild(a_MoveToNavMeshTotem);//hecho
                        selector1111->addChild(a_Thinck);//hecho
        selector1->addChild(sequence1_2);//hecho
            sequence1_2->addChild(c_TotemOtherCar);//hecho
            sequence1_2->addChild(selector12);//hecho
                selector12->addChild(sequence12_1);//hecho
                    sequence12_1->addChild(c_PowerUpAttack);//hecho
                    sequence12_1->addChild(selector121);//hecho
                        selector121->addChild(sequence121_1);//hecho
                            sequence121_1->addChild(c_SeeCar);//hecho 
                            sequence121_1->addChild(selector122);//hecho
                                selector122->addChild(sequence122_1);//hecho
                                    sequence122_1->addChild(c_NoCriticalTime);//hecho
                                    sequence122_1->addChild(a_MovCarView);//hecho
                                selector122->addChild(sequence122_2);//hecho
                                    sequence122_2->addChild(c_SeeCarTotem);//hecho
                                    sequence122_2->addChild(a_MovCarViewTotem);//hecho
                        selector121->addChild(selector1211);//hecho
                            selector1211->addChild(d_Facil2TimesMC);//hecho
                                d_Facil2TimesMC->addChild(sequence1211_1);//hecho
                                    sequence1211_1->addChild(a_NoThinck);//hecho
                                    sequence1211_1->addChild(a_MoveRandom);//hecho
                            selector1211->addChild(a_Thinck);//hecho
                selector12->addChild(selector123);//hecho
                    selector123->addChild(sequence123_2);//hecho
                        sequence123_2->addChild(m_Inverter1);//hecho
                            m_Inverter1->addChild(c_HavePowerUp);//hecho
                        sequence123_2->addChild(selector1234);//hecho
                            selector1234->addChild(sequence1234_1);//hecho
                                sequence1234_1->addChild(c_SeePowerUp);//hecho
                                sequence1234_1->addChild(a_MovViewPowerUp);//hecho
                            selector1234->addChild(a_MoveRandom);//hecho
                    selector123->addChild(a_MoveRandom);//hecho
    selectorBehaviourTree->addChild(selector2);//hecho
        selector2->addChild(sequence2_1);//hecho
        selector2->addChild(a_MoveRandomCentral);//por decidir--> de momento igual que un random normal











    //shared_ptr<sequence> sequence1 = make_shared<sequence>();
    //    shared_ptr<TotemAlone_dm> c_totemAlone =   make_shared<TotemAlone_dm>();
    //    shared_ptr<MoveToTotem_dm> a_moveToTotem = make_shared<MoveToTotem_dm>();
    //shared_ptr<sequence> sequence2 = make_shared<sequence>();
    //    shared_ptr<HaveTotem_dm> c_haveTotem =   make_shared<HaveTotem_dm>();
    //    shared_ptr<EscapeWithTotem_dm> a_escapeWithTotem = make_shared<EscapeWithTotem_dm>();s
    //shared_ptr<sequence> sequence3 =    make_shared<sequence>();
    //    shared_ptr<Inverter_dm> m_inverter1 =  make_shared<Inverter_dm>();
    //        shared_ptr<HavePowerUp_dm> c_havePowerUp = make_shared<HavePowerUp_dm>(); 
    //    shared_ptr<MoveToPowerUp_dm> a_moveToPowerUp = make_shared<MoveToPowerUp_dm>(); 
    //shared_ptr<sequence> sequence4  = make_shared<sequence>();
    //    shared_ptr<HavePowerUpAttack_dm> c_havePowerUpAttack = make_shared<HavePowerUpAttack_dm>();
    //    shared_ptr<MoveToCarTotem_dm> a_moveToCarTotem =       make_shared<MoveToCarTotem_dm>();
//
//
    //std::cout << "------------------------ Behaviour Move To --------------------------------" << std::endl;
    //// ASIGNACION DEL ARBOL DE DECISIONES
//
    //// el totem esta en el suelo?
    //selectorBehaviourTree->addChild(sequence1);
    //// tengo yo el totem?
    //selectorBehaviourTree->addChild(sequence2);
    //// vamos a coger PowerUps si no tenemos
    //selectorBehaviourTree->addChild(sequence3);
    //// si el powerUp es de ataque voy a por el que tiene el totem
    //selectorBehaviourTree->addChild(sequence4);
    //// TODO somos una IA que tiene el powerUp y obvimanete no nos vamos a perseguir a nosotros mismos
    ////selectorBehaviourTree->addChild(a_moveToPowerUp);
//
    //sequence1->addChild(c_totemAlone);
    //sequence1->addChild(a_moveToTotem);
//
    //sequence2->addChild(c_haveTotem);
    //sequence2->addChild(a_escapeWithTotem);
//
    //sequence3->addChild(m_inverter1);
    //    m_inverter1->addChild(c_havePowerUp);
    //sequence3->addChild(a_moveToPowerUp);
//
    //sequence4->addChild(c_havePowerUpAttack);
    //sequence4->addChild(a_moveToCarTotem);
    
}




void SystemBtDecisionMove::update(CarAI* actualCar){
    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, static_cast<ManCar*>(managers[0]), static_cast<ManPowerUp*>(managers[1]), 
        static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManTotem*>(managers[3]), static_cast<ManWayPoint*>(managers[4]), static_cast<ManNavMesh*>(managers[5]) );
    
    

    selectorBehaviourTree->run(blackboard.get());
}