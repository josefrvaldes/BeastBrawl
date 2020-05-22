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
 * 
 */
 
 
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
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrainAI->numTried >= cBrainAI->totalTried && cBrainAI->stackPath.empty()){
            //cout << "SE HACE UN RESET DEL PENSAR" << endl;
            cBrainAI->numTried = 0;
        }
        if (cBrainAI->numTried < cBrainAI->totalTried){

            cBrainAI->numTried += 1;
            if(cBrainAI->numTried == cBrainAI->totalTried){
                //cout << "INTENTO NUMERO: " << cBrainAI->numTried << " YA NO ESTAMOS PENSANDO 1" << endl;
                return getChild()->run(blackboard);
            }else{
                //cout << "LA IA ESTA PENSANDO... " << endl;
                return false;
            }

        }
        //numTried = totalTried;

        //cout << "INTENTO NUMERO: " << cBrainAI->numTried << " YA NO ESTAMOS PENSANDO 2" << endl;
        //numTried 
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
            //cout << "TENEMOS POWER UP " << endl;
            return true;
        }
        //cout << "NO TENEMOS POWER UP " << endl;
        return false;
    }
};
//CONDICION esa el Totem suelto por el mapa?
struct TotemAlone_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTotem = static_cast<CTotem*>(blackboard->manTotems->GetEntities()[0]->GetComponent(CompType::TotemComp).get());
        if(cTotem->active){
            //cout << "EL TOTEM ESTA EN EL SUELO" << endl;
        }else{
            //cout << "EL TOTEM NO ESTA EN EL SUELO" << endl;
        }
        return cTotem->active;
    }
};

struct TotemOtherCar_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        for(const auto& actualAI : blackboard->manCars->GetEntities()){
            if(actualAI.get() != blackboard->actualCar){
                auto cTotemAI = static_cast<CTotem*>(actualAI->GetComponent(CompType::TotemComp).get());
                if(cTotemAI->active == true){
                    //cout << "EL TOTEM LO TIENE OTRO COCHE " << endl;
                    return true;
                }
            }
        }
        //cout << " EL TOTEM NOOOOOOOO LO TIENE OTRO COCHE " << endl;
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
            //std:://cout << "TENEMOS POWERUP DE ATAQUE ";
            //if(cPowerUp->typePowerUp == typeCPowerUp::MelonMolon)  ////cout << "MELON " <<endl;
            //if(cPowerUp->typePowerUp == typeCPowerUp::TeleBanana)  ////cout << "BANANA " <<endl;
            //if(cPowerUp->typePowerUp == typeCPowerUp::SuperMegaNitro)  ////cout << "SUPERMEGANITRO " <<endl;
            return true;
        }
        //cout << "NOOOOOO TENEMOS POWERUP DE ATAQUE " <<endl;
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

// IMPLEMENTADO 
struct SeeTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->totemInVision){
            //cout << "VEO EL TOTEM" << endl;
            return true;
        }
        //cout << "NO VEO EL TOTEM" << endl;
        return false;
    }
};

// IMPLEMENTADO o
struct SeePowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->boxInVision.size() > 0){
            //cout << "VEMOS AL MENOS 1 POWERUP" << endl;
            return true;
        }
        //cout << "NO VEMOS NINGUN POWERUP" << endl;
        return false;
    }
};

// IMPLEMENTADO 
struct Think_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        cBrain->thinking = true;
        //cout << "PONEMOS EL CBRAIN A TRUE, YA QUE ESTAMOS PENSANDO" <<endl;
        return true;
    }
};

// IMPLEMENTADO 
struct NoThink_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        cBrain->thinking = false;
        //cout << "PONEMOS EN CBRAIN->THINKING A FALSE, YA NO DEBEMOS DE PENSAR MAAAAS" <<endl;
        return true;
    }
};

// IMPLEMENTADO 
struct NoCriticalTime_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        // cogemos el tiempo que llevamos el totem y vemos si supera el tiempo critico
        for(const auto& actualAI : blackboard->manCars->GetEntities()){
            if(actualAI.get() != blackboard->actualCar){
                auto cTotemAI = static_cast<CTotem*>(actualAI->GetComponent(CompType::TotemComp).get());
                auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
                if(cTotemAI->active && (cTotemAI->accumulatedTime/1000.0 + cBrainAI->timeFollowDirect < cTotemAI->DURATION_TIME/1000.0) ){
                    // si el tiempo actual + el critco es menor al total = no estamos en tiempo critico
                    //cout << "EL TIEMPO QUE QUEDA NO ES CRITICOOOO" <<endl;
                    return true;
                }
            }
        }
        //cout << "EL TIEMPO QUE QUEDA ES CRITICO " <<endl;
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
            //cout << "ESTAMOS EN EL MISMO NAVMESH QUE EL TOTEM" << endl;
            return true;
        }else{
            // no estamos en el mismo navmesh
            //cout << "NO ESTAMOS EN EL MISMO NAVMEHS QUE EL TOTEM" << endl;
            return false; 
        }
    }
};


// Condicion para saber si estamos en el mismo navmesh que el coche que tiene el totem
struct SameNavMeshCCT_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cCurrendNavMeshCar = static_cast<CCurrentNavMesh*>(blackboard->actualCar->GetComponent(CompType::CurrentNavMeshComp).get());

        for(const auto& actualAI : blackboard->manCars->GetEntities()){
            if(actualAI.get() != blackboard->actualCar){
                auto cTotemAI = static_cast<CTotem*>(actualAI->GetComponent(CompType::TotemComp).get());
                if(cTotemAI->active){
                    auto cCurrendNavMeshCarAI = static_cast<CCurrentNavMesh*>(actualAI->GetComponent(CompType::CurrentNavMeshComp).get());
                    if(cCurrendNavMeshCar->currentNavMesh == cCurrendNavMeshCarAI->currentNavMesh){
                        // estamos en el mismo navmesh que el totem
                        //cout << "ESTAMOS EN EL MISMO NAVMESH QUE EL COCHE QUUE TIENE ELTOTEM" << endl;
                        return true;
                    }
                }
            }
        }
        //cout << "NO ESTAMOS EN EL MISMO NAVMESH QUE EL COCHE QUE TIENE EL TOTEM " <<endl;
        return false;
    }
};



//IMPLEMENTADO  -> es si tenemos un coche en ranfo de vision, es decir, coger el ocmponenre brain y comprobarlo
struct SeeCar_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->carInVision.size() > 0){
            //cout << "VEO POR LO MENOS A UN COCHE"<<endl;
            return true;
        }
        //cout << "NO VEO NINGUN COCHE"<<endl;
        return false;
    }
};

// IMPLEMENTADO  -> es si tenemos un coche en ranfo de vision y tiene el totem 
struct SeeCarTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrain->carInVision.size() > 0){
            for(const auto& currentCar : cBrain->carInVision){
                if(currentCar != blackboard->actualCar){
                    auto cTotemCar = static_cast<CTotem*>(currentCar->GetComponent(CompType::TotemComp).get());
                    if(cTotemCar->active == true){
                        //cout << "DE LOS COCHES QUE ESTOY VIENDO, UNO TIENE EL TOTEM " << endl;
                        return true;
                    }
                }
            }
        }
        //cout << "NINGUN COCHE QUE ESTOY VIENDO TIENE EL TOTEM " << endl;
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
        //cout << "ACCION: VAMOS A POR EL TOTEM" << endl;
        return true;
    }
};

//Con la nueva estructura el manager de pahtplanning yan tiene los managers no hay que pasarselos.
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
        }//else{
            //cBrainAI->targetNavMesh =-1;
        //}  
        //cout << "NOS MOVEREMOS AL NAVMESH QUE DONDE ESTA EL TOTEEEEEEM " << endl;
        return true;
    }
};
// IMPLEMENTADO  -> QUEDA LA OTRA PARTE
struct MoveRandom_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std:://cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;    
        shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();
        (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;  
        (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;                                                                                                      
        EventManager::GetInstance().AddEventMulti(Event{EventType::MOVE_RANDOM_POWERUP, dataPowerUp}); 
        return true;
    }
};
//  IMPLEMENTADO COMPLETO -> indicar, con otro evento, que debe ser un powerUp del NavMesh central locoooo
struct MoveRandomCentral_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {  
        shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();
        (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;  
        (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;                                                                                                      
        EventManager::GetInstance().AddEventMulti(Event{EventType::MOVE_RANDOM_POWERUP, dataPowerUp}); 
        return true;
    }
};

//  IMPLEMENTADO  -> movernos al coche que estamos viendo delante nuestra
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
        //cout << "NOS MOVEMOS AL COCHE QUE ESTAMOS VIENDO (AL PRIMERO, QUE ES EL MAS CENTRADO) " << endl;
        return true;
    }
};

//IMPLEMENTADO  -> movernos al coche que estamos viendo delante y tiene el totem
struct MovCarViewTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override { 
        auto actualCar = blackboard->actualCar;
        auto cBrain = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
        for(const auto& currentCar : cBrain->carInVision){
            if(currentCar != actualCar){
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
                    //cout << "NOS MOVEMOS AL COCHE QUE TIENE EL TOTEM Y ESTAMOS VIENDO " << endl;
                    return true;
                }
            }
        }
        return false;
    }
};


struct MovViewPowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override { 
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrainAI->targetBoxPowerUp == nullptr){
            auto randBox = rand() % cBrainAI->boxInVision.size();
            cBrainAI->targetBoxPowerUp = static_cast<BoxPowerUp*>(cBrainAI->boxInVision[randBox]);
            auto cTransBoxPU = static_cast<CTransformable*>(cBrainAI->boxInVision[randBox]->GetComponent(CompType::TransformableComp).get());

            shared_ptr<DataMap> dataBoxPU = make_shared<DataMap>();
            (*dataBoxPU)[ACTUAL_CAR] = blackboard->actualCar;     
            (*dataBoxPU)[POS_DESTINATION] = cTransBoxPU->position;                                        
            EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataBoxPU}); 
            //cout << "ACCION: NOS MOVEMOS AL POWERUP: " << randBox << " DENTRO DE LA LISTA DE LO QUE VEO" << endl;
            return true;
        }
        //cout << "YA TENEMOS LOCALIZADO EL POWERUP, POR LO QUE NO VOLVEMOS A CALCULAR CPOSDESTINATION" <<endl;
        return true;
    }
};


struct MoveToCarTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override { 
        auto actualCar = blackboard->actualCar;
        auto cBrainAI = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());

        for(const auto& currentCar : blackboard->manCars->GetEntities()){
            if(currentCar.get() != actualCar){
                auto cTotem = static_cast<CTotem*>(currentCar->GetComponent(CompType::TotemComp).get());
                if(cTotem->active == true){
                    cBrainAI->targetCar = static_cast<Car*>(currentCar.get());

                    auto cTransCarTotem = static_cast<CTransformable*>(currentCar->GetComponent(CompType::TransformableComp).get());
                    shared_ptr<DataMap> dataCarTotem = make_shared<DataMap>();
                    (*dataCarTotem)[ACTUAL_CAR] = actualCar;     
                    (*dataCarTotem)[POS_DESTINATION] = cTransCarTotem->position;                                        
                    EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataCarTotem});

                    //cout << "MISMO NAVMESH QUE EL COCHE CON TOTEM -> A POR EL!! "<< endl;
                    return true;
                }
            }
        }
        return false;     
    }
};


//ACCION movernos al jugador con Totem 
struct MoveToNavMeshCarTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        for(const auto& curretCar : blackboard->manCars->GetEntities()){
            if(curretCar.get() != actualCar){
                auto cTotem = static_cast<CTotem*>(curretCar->GetComponent(CompType::TotemComp).get());
                if(cTotem->active == true){
                    auto cCurrendNavMeshCarAI = static_cast<CCurrentNavMesh*>(curretCar->GetComponent(CompType::CurrentNavMeshComp).get());
                    auto cBrainAI = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
                    if(cBrainAI->targetNavMesh != cCurrendNavMeshCarAI->currentNavMesh){
                        cBrainAI->targetNavMesh = cCurrendNavMeshCarAI->currentNavMesh;
                        shared_ptr<DataMap> dataNavCar = make_shared<DataMap>();      
                        (*dataNavCar)[ACTUAL_CAR] = actualCar;     
                        (*dataNavCar)[MAN_WAYPOINTS] = blackboard->manWayPoint;
                        (*dataNavCar)[MAN_NAVMESH] = blackboard->manNavMesh;  
                        (*dataNavCar)[ID_DESTINATION] = cCurrendNavMeshCarAI->currentNavMesh;                                                                                                   
                        EventManager::GetInstance().AddEventMulti(Event{EventType::CALCULATE_PATH_TO_NAVMESH, dataNavCar}); 
                    }//else{
                        //cBrainAI->targetNavMesh = -1;
                    //}
                    //cout << "NOS MOVEMOS AL NAVMHES DONDE ESTA EL COCHE CON EL TOTEM LOCOOOOO "<< endl;
                }
            }
        }
        return true;     
    }
};
// NOTA: recordar que tenemos el otro Behaviour tree de tirar powerUp y por tanto lo seguiremos
// hasta que lo veamos, una vez visto le lanzaemos el powerUp e iremos a por el Totem u otro PowerUp




SystemBtDecisionMove::SystemBtDecisionMove(){

    //   CREACION DEL ARBOL DE DECISIONES
    selectorBehaviourTree = make_unique<selector>();

    // SELECTORES
    shared_ptr<selector> selector1              = make_unique<selector>();
    shared_ptr<selector> selector11             = make_unique<selector>();
    shared_ptr<selector> selector111            = make_unique<selector>();
    shared_ptr<selector> selector1111           = make_unique<selector>(); 
    shared_ptr<selector> selector12             = make_unique<selector>();
    shared_ptr<selector> selector121            = make_unique<selector>();
    shared_ptr<selector> selector122            = make_unique<selector>();
    shared_ptr<selector> selector1211           = make_unique<selector>();
    shared_ptr<selector> selector123            = make_unique<selector>();
    shared_ptr<selector> selector1234           = make_unique<selector>();
    shared_ptr<selector> selector2              = make_unique<selector>();

    // SECUENCIAS
    shared_ptr<sequence> sequence1_1            = make_unique<sequence>();
    shared_ptr<sequence> sequence11_1           = make_unique<sequence>();
    shared_ptr<sequence> sequence11_2           = make_unique<sequence>();
    shared_ptr<sequence> sequence111_1          = make_unique<sequence>(); 
    shared_ptr<sequence> sequence1111_1         = make_unique<sequence>();
    shared_ptr<sequence> sequence1_2            = make_unique<sequence>();
    shared_ptr<sequence> sequence12_1           = make_unique<sequence>();
    shared_ptr<sequence> sequence121_1          = make_unique<sequence>();
    shared_ptr<sequence> sequence122_1          = make_unique<sequence>();
    shared_ptr<sequence> sequence122_2          = make_unique<sequence>();
    shared_ptr<sequence> sequence1211_1         = make_unique<sequence>();
    shared_ptr<sequence> sequence1211_2         = make_unique<sequence>();  
    shared_ptr<sequence> sequence123_2          = make_unique<sequence>();
    shared_ptr<sequence> sequence1234_1         = make_unique<sequence>();
    shared_ptr<sequence> sequence2_1            = make_unique<sequence>();

    // CONDICIONES
    shared_ptr<TotemAlone_dm> c_TotemAlone              = make_unique<TotemAlone_dm>();
    shared_ptr<SeeTotem_dm> c_SeeTotem                  = make_unique<SeeTotem_dm>();
    shared_ptr<SameNavMeshCT_dm> c_SameNavMeshCT        = make_unique<SameNavMeshCT_dm>();
    shared_ptr<SeePowerUp_dm> c_SeePowerUp              = make_unique<SeePowerUp_dm>();
    shared_ptr<TotemOtherCar_dm> c_TotemOtherCar        = make_unique<TotemOtherCar_dm>();
    shared_ptr<HavePowerUpAttack_dm> c_PowerUpAttack    = make_unique<HavePowerUpAttack_dm>();
    shared_ptr<SeeCar_dm> c_SeeCar                      = make_unique<SeeCar_dm>();
    shared_ptr<NoCriticalTime_dm> c_NoCriticalTime      = make_unique<NoCriticalTime_dm>();
    shared_ptr<SeeCarTotem_dm> c_SeeCarTotem            = make_unique<SeeCarTotem_dm>();
    shared_ptr<SameNavMeshCCT_dm> c_SameNavMeshCCT      = make_unique<SameNavMeshCCT_dm>();
    shared_ptr<HavePowerUp_dm> c_HavePowerUp            = make_unique<HavePowerUp_dm>(); 

    // ACCIONES
    shared_ptr<MoveToTotem_dm> a_MoveToTotem                        = make_unique<MoveToTotem_dm>();
    shared_ptr<MovViewPowerUp_dm> a_MovViewPowerUp                  = make_unique<MovViewPowerUp_dm>();
    shared_ptr<NoThink_dm> a_NoThinck                               = make_unique<NoThink_dm>(); 
    shared_ptr<MovNavMeshTotem_dm> a_MoveToNavMeshTotem             = make_unique<MovNavMeshTotem_dm>();
    shared_ptr<Think_dm> a_Thinck                                   = make_unique<Think_dm>(); 
    shared_ptr<MovCarView_dm> a_MovCarView                          = make_unique<MovCarView_dm>();
    shared_ptr<MovCarViewTotem_dm> a_MovCarViewTotem                = make_unique<MovCarViewTotem_dm>();
    shared_ptr<MoveToCarTotem_dm> a_MoveToCarTotem                  = make_unique<MoveToCarTotem_dm>();
    shared_ptr<MoveToNavMeshCarTotem_dm> a_MoveToNavMeshCarTotem    = make_unique<MoveToNavMeshCarTotem_dm>();   
    shared_ptr<MoveRandom_dm> a_MoveRandom                          = make_unique<MoveRandom_dm>();
    shared_ptr<MoveRandomCentral_dm> a_MoveRandomCentral            = make_unique<MoveRandomCentral_dm>();

    // DECORADORES
    shared_ptr<Facil2times_dm> d_Facil2TimesMT      = make_unique<Facil2times_dm>();
    shared_ptr<Facil2times_dm> d_Facil2TimesMC      = make_unique<Facil2times_dm>();
    shared_ptr<Inverter_dm> m_Inverter0             = make_unique<Inverter_dm>();
    shared_ptr<Inverter_dm> m_Inverter1             = make_unique<Inverter_dm>();
    shared_ptr<Inverter_dm> m_Inverter2             = make_unique<Inverter_dm>();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // CONSTRUCCION DEL ARBOL
    selectorBehaviourTree->addChild(m_Inverter0);
        m_Inverter0->addChild(a_NoThinck);
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
                        sequence111_1->addChild(m_Inverter1);//hecho
                            m_Inverter1->addChild(c_HavePowerUp);//hecho
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
                            selector1211->addChild(sequence1211_1);
                                sequence1211_1->addChild(c_SameNavMeshCCT);
                                sequence1211_1->addChild(a_MoveToCarTotem);
                            selector1211->addChild(d_Facil2TimesMC);//hecho
                                d_Facil2TimesMC->addChild(sequence1211_2);//hecho
                                    sequence1211_2->addChild(a_NoThinck);//hecho
                                    sequence1211_2->addChild(a_MoveToNavMeshCarTotem);//hecho
                            selector1211->addChild(a_Thinck);//hecho
                selector12->addChild(selector123);//hecho
                    selector123->addChild(sequence123_2);//hecho
                        sequence123_2->addChild(m_Inverter2);//hecho
                            m_Inverter2->addChild(c_HavePowerUp);//hecho
                        sequence123_2->addChild(selector1234);//hecho
                            selector1234->addChild(sequence1234_1);//hecho
                                sequence1234_1->addChild(c_SeePowerUp);//hecho
                                sequence1234_1->addChild(a_MovViewPowerUp);//hecho
                            selector1234->addChild(a_MoveRandom);//hecho
                    selector123->addChild(a_MoveRandom);//hecho
    selectorBehaviourTree->addChild(selector2);//hecho
        selector2->addChild(sequence2_1);//hecho
            sequence2_1->addChild(c_NoCriticalTime);//hecho
            sequence2_1->addChild(a_MoveRandom);//hecho
        selector2->addChild(a_MoveRandomCentral);//por decidir--> de momento igual que un random normal

}




void SystemBtDecisionMove::update(CarAI* actualCar){
    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, static_cast<ManCar*>(managers[0]), static_cast<ManPowerUp*>(managers[1]), 
        static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManTotem*>(managers[3]), static_cast<ManWayPoint*>(managers[4]), static_cast<ManNavMesh*>(managers[5]) );
    
    

    selectorBehaviourTree->run(blackboard.get());
    //cout << "PASAMOS 1 CICLO ---------------------------------------------------------------------------------------------------------------------------- " << endl;
}