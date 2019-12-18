#include "SystemBtPowerUp.h"


#include "../behaviourTree/behaviourTree.h"
#include "../behaviourTree/composite.h"
#include "../behaviourTree/selector.h"
#include "../behaviourTree/sequence.h"
#include "../behaviourTree/decorator.h"

#include "../behaviourTree/Blackboard.h"
#include "../Components/CPowerUp.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
///// DECORATORS //////

// Decorator Minimun
struct Minimum : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
    uint32_t totalTries = 3;
    uint32_t numTries = 0;
    virtual bool run(Blackboard* blackboard) override {
        if (numTries >= totalTries)
            return getChild()->run(blackboard);
        numTries++;
        cout << "Fallamos al coger la llave, intento: " << numTries << endl;
        return false;
    }
};

// Decorator Limit
struct Limit : public Decorator {  // Decorator Limit
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
struct UntilFail : public Decorator {  // Decorator UntilFail
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
struct Inverter : public Decorator {  // Decorator Inverter
    virtual bool run(Blackboard* blackboard) override {
        return !(getChild()->run(blackboard));
    }
};



///////////////////////////////////
///////// CONDICIONES /////////////

//bool door = false;  // imaginemos que door=false es = puerta cerrada "door is open?"
//bool key = false;   // tenemos una llave
//// ACCION DE ABRIR LA PUERTA
//struct openDoor : public behaviourTree {
//    virtual bool run(Blackboard* blackboard) override {
//        door = true;
//        cout << "Abrimos la puerta" << endl;
//        return true;
//    }
//};
////ACCION COGER LA LLAVE
//struct getKey : public behaviourTree {
//    virtual bool run(Blackboard* blackboard) override {
//        key = true;
//        cout << "Cogemos la llave" << endl;
//        return true;
//    }
//};
////CONDICION PUERTA ABIERTA?
//struct haveDoorOpen : public behaviourTree {
//    virtual bool run(Blackboard* blackboard) override {
//        cout << "Comprobamos si la puerta esta abierta: " << boolalpha << door << endl;
//        return door;
//    }
//};
////CONDICION TENEMOS LLAVE?
//struct haveKey : public behaviourTree {
//    virtual bool run(Blackboard* blackboard) override {
//        cout << "Comprobamos si tenemos la llave: " << boolalpha << key << endl;
//        return key;
//    }
//};



//CONDICION Tenemos powerUp?
struct HavePowerUp : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if(cPowerUp->typePowerUp == typeCPowerUp::None){
            std::cout << "No tenemos powerUp" << std::endl;
            return false;
        }
        return true;
    }
};


//Afirmacion No tenemos power up!
struct DontHavePoweUp : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        std::cout << "Tienes power up baby" << std::endl;
        return true;
    }
};


//    None,               // 0
//    RoboJorobo,         // 1
//    SuperMegaNitro,     // 2
//    PudinDeFrambuesa,   // 3
//    EscudoMerluzo,      // 4
//    TeleBanana,         // 5
//    MelonMolon          // 6

//Condicion -> El powerUp es Nitro, RoboJorobo, EscudoMerluzo
struct CompPowerUp : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp ==    typeCPowerUp::EscudoMerluzo || 
            cPowerUp->typePowerUp ==    typeCPowerUp::RoboJorobo    || 
            cPowerUp->typePowerUp ==    typeCPowerUp::SuperMegaNitro    ){
            return true;
        }else
            return false;
    }
};


//Condicion -> El powerUp es Nitro, RoboJorobo, EscudoMerluzo
struct ThrowPowerUp : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        std::cout << "Lanzaaas el powerUp beibeee (realmente no)" << std::endl;
        return true;
    }
};



SystemBtPowerUp::SystemBtPowerUp(){

    //BehaviorTree BASICO
    //                                  SELECTOR1
                                            //
            ///////////////////////////////////////////////////////////////////////////////////
            //                                      //                                       //
    // La pueta esta abierta?                     SEQUENCE                               DECORATOR (minimum) (3 intentos)
                                        ///////////////////////////////                      //
                                        //                          //                       //
    //                              // tengo llave?             //abrir puerta        // coger llave

//    selectorBehaviourTree = make_shared<selector>();
//    shared_ptr<sequence> sequence1 = make_shared<sequence>();
//
//    shared_ptr<haveDoorOpen> puertaAbiertaSiNo = make_shared<haveDoorOpen>();
//    shared_ptr<haveKey> tengoLlaveSiNo = make_shared<haveKey>();
//    shared_ptr<openDoor> abrirPuerta = make_shared<openDoor>();
//    shared_ptr<getKey> cogerLlave = make_shared<getKey>();
//
//    shared_ptr<Minimum> tryCatchKey3 = make_shared<Minimum>();
//
//    selectorBehaviourTree->addChild(puertaAbiertaSiNo);
//    selectorBehaviourTree->addChild(sequence1);
//    selectorBehaviourTree->addChild(tryCatchKey3);
//
//    sequence1->addChild(tengoLlaveSiNo);
//    sequence1->addChild(abrirPuerta);
//
//    tryCatchKey3->addChild(cogerLlave);


//              SELECTOR
                   //
    //////////////////////////////////////
    //                                  //
//  DECORATOR (inverter)                cout(TienesPowerUp)
    //
// Tenemos PU?    


    //   CREACION DEL ARBOL DE DECISIONES

    selectorBehaviourTree = make_shared<selector>();

    shared_ptr<HavePowerUp> c_havePowerUp = make_shared<HavePowerUp>();             // Condicion -->    ¿Tengo un PowerUp?
    shared_ptr<DontHavePoweUp> c_dontHavePoweUp = make_shared<DontHavePoweUp>();

    shared_ptr<Inverter> m_inverter1 = make_shared<Inverter>();                     // Decorator -->    inverter para Condicion ¿Tengo un PowerUp?

    shared_ptr<sequence> sequence1 = make_shared<sequence>();
    shared_ptr<CompPowerUp> c_compPowerUp = make_shared<CompPowerUp>();             // Condicion -->    ¿El powerUp que tengo es Escudo, Nitro o Pudding?
   // shared_ptr<ThrowPowerUp> a_throwPowerUp = make_shared<ThrowPowerUp>();           // Accion    -->    Lanzar el powerUp


    // ASIGNACION DEL ARBOL DE DECISIONES

    m_inverter1->addChild(c_havePowerUp);

    selectorBehaviourTree->addChild(m_inverter1);
    selectorBehaviourTree->addChild(sequence1);
    selectorBehaviourTree->addChild(c_dontHavePoweUp); // hasta que acabe el arbol para imprimir una salida


    sequence1->addChild(c_compPowerUp);
    //sequence1->addChild(a_throwPowerUp);

    
}




void SystemBtPowerUp::update(CarAI* actualCar, ManCar* manCars,ManPowerUp* manPowerUps, ManBoxPowerUp* manBoxPowerUps, ManTotem* manTotems, ManWayPoint* manWayPoint){
    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, manCars, manPowerUps, manBoxPowerUps, manTotems, manWayPoint);
    selectorBehaviourTree->run(blackboard.get());
}