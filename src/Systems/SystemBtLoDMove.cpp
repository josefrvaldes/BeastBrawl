#include "SystemBtLoDMove.h"


#include "../behaviourTree/behaviourTree.h"
#include "../behaviourTree/composite.h"
#include "../behaviourTree/selector.h"
#include "../behaviourTree/sequence.h"
#include "../behaviourTree/decorator.h"

#include "../behaviourTree/Blackboard.h"
//#include "../Components/CPowerUp.h"
#include "../Components/CTotem.h"

#include "../Entities/Totem.h"
#include "../Managers/ManTotem.h"
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
//CONDICION Tenemos powerUp?
struct HavePowerUp_LoDMove : public behaviourTree {
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
struct TotemAlone_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        if( blackboard->manTotems->GetEntities().size() > 0){
            //std::cout << "Si esta el totem en el suelo locoooooooooooooooooooooooooooooooooooooooooooooooo" << std::endl;
            return true;
        }
        return false;
    }
};
//CONDICION Tenemos un powerUp de ataque? Melon o Banana
struct HavePowerUpAttack_LoDMove : public behaviourTree {
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
//ACCION comprobamos si tenemos melon molon
struct HaveMelonMolon_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == typeCPowerUp::MelonMolon){
            // Steering Behavior de lanzarlarlo
            return true;
        }
        return false;
    } 
};


//ACCION comprobamos si esta dentro del rango de vision
struct OutofVisionRange_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        if(blackboard->manCars->carInVisionRange(blackboard->manCars->GetCar().get(), blackboard->actualCar, 66) == false){
            // Aplicar Steering Behavior para el movimiento
            return true;
        }
        return false;
    } 
};


//ACCION comprobamos si el coche actual se encuentra cerca de el del jugador
struct InDistanceRange_LoDMove : public behaviourTree {

    virtual bool run(Blackboard* blackboard) override {
        auto cTransformableCar = static_cast<CTransformable*>(blackboard->manCars->GetCar()->GetComponent(CompType::TransformableComp).get());
        auto cTransformableCarAI = static_cast<CTransformable*>(blackboard->actualCar->GetComponent(CompType::TransformableComp).get());
        float vetorDistanceX = (cTransformableCarAI->position.x - cTransformableCar->position.x);
        float vetorDistanceZ = (cTransformableCarAI->position.x - cTransformableCar->position.z);
        float distanceToCarAI = sqrt(vetorDistanceX*vetorDistanceX + vetorDistanceZ*vetorDistanceZ);

        if(distanceToCarAI < 800.0){
            // Aplicar Logica Difusa para el movimiento
            return true;
        }
        return false;
    } 
};


//ACCION comprobamos si el coche actual se encuentra cerca de el del jugador
struct InDistanceFar_LoDMove : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        // Aplicar Steering Behavior para el movimiento
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

    
    //   Lanzar melon molon = true   -> Aplicar Steering de movimiento y lanzar en caso de que angulo sea 0
    
    //                 SELECTOR
    //   MelonMolon             Selector(LoD)
    //                            //
    //                        //////////////
    //                        //          //
    //                      Fuera        Dentro
    //                        //            //
    //                        SB          Selector
    //                                       //
    //                                  //////////////
    //                                  //          //
    //                                Cerca       Lejos
    //                                  //          //
    //                                  LD          SB


    selectorBehaviourTree = make_shared<selector>();

    shared_ptr<HaveMelonMolon_LoDMove> a_HaveMelonMolon =   make_shared<HaveMelonMolon_LoDMove>();
    shared_ptr<selector> selectorVision = make_shared<selector>();

    shared_ptr<OutofVisionRange_LoDMove>  c_OutofVisionRange =   make_shared<OutofVisionRange_LoDMove>();
    shared_ptr<selector> selectorDistance = make_shared<selector>();

    shared_ptr<InDistanceRange_LoDMove>  a_InDistanceRange =   make_shared<InDistanceRange_LoDMove>();
    shared_ptr<InDistanceFar_LoDMove>  a_InDistanceFar =   make_shared<InDistanceFar_LoDMove>();

    selectorBehaviourTree->addChild(a_HaveMelonMolon);
    selectorBehaviourTree->addChild(selectorVision);

    selectorVision->addChild(c_OutofVisionRange);
    selectorVision->addChild(selectorDistance);

    selectorDistance->addChild(a_InDistanceRange);
    selectorDistance->addChild(a_InDistanceFar);

}




void SystemBtLoDMove::update(CarAI* actualCar, ManCar* manCars,ManPowerUp* manPowerUps, ManBoxPowerUp* manBoxPowerUps, ManTotem* manTotems, ManWayPoint* manWayPoint){
    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, manCars, manPowerUps, manBoxPowerUps, manTotems, manWayPoint);
    selectorBehaviourTree->run(blackboard.get());
}