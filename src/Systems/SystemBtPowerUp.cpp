#include "SystemBtPowerUp.h"


#include "../behaviourTree/behaviourTree.h"
#include "../behaviourTree/composite.h"
#include "../behaviourTree/selector.h"
#include "../behaviourTree/sequence.h"
#include "../behaviourTree/decorator.h"

#include "../behaviourTree/Blackboard.h"
#include "../Components/CPowerUp.h"
#include "../Components/CTotem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
///// DECORATORS //////

// DECORATOR Minimun
struct Minimum : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
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

// DECORATOR Limit
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

// DECORATOR UntilFail
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

// DECORATOR Inverter
struct Inverter : public Decorator {  // Decorator Inverter
    virtual bool run(Blackboard* blackboard) override {
        return !(getChild()->run(blackboard));
    }
};

////////////////////////
///// CONDICIONES///////

//Condicion Tenemos powerUp?
struct HavePowerUp : public behaviourTree {
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


//Afirmacion No tenemos power up!
struct DontHavePoweUp : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std::cout << "No power up baby" << std::endl;
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
        if( cPowerUp->typePowerUp ==    typeCPowerUp::EscudoMerluzo     || 
            cPowerUp->typePowerUp ==    typeCPowerUp::PudinDeFrambuesa  || 
            cPowerUp->typePowerUp ==    typeCPowerUp::SuperMegaNitro        ){
            //std::cout << "Es o escudo merluzo o Pudign o nitro" << std::endl;
            return true;
        }
        //std::cout << "No es ni escudo merluzo ni Pudign ni nitro" << std::endl;
        return false;
    }
};


//Condicion -> El powerUp es Nitro, RoboJorobo, EscudoMerluzo
// TODO -- lo hace jose
struct ThrowPowerUp : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std::cout << "Lanzaaas el powerUp beibeee" << std::endl;
        shared_ptr<EventManager> eventManager = EventManager::GetInstance();
        DataMap d;
        d["actualCar"] = blackboard->actualCar;
        eventManager->AddEventMulti(Event{EventType::THROW_POWERUP_AI, d});
        return true;
    }
};

struct HaveRoboJorobo : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == typeCPowerUp::RoboJorobo){
            //std::cout << "Es el robo jorobo" << std::endl;    
            return true;
        }
        //std::cout << "No es el robo jorobo" << std::endl;  
        return false;
    } 
};

// TODO --> actualmente si tienes tu el totem te lo quitas y te lo vuelve a asiganar
struct HaveTotemOtherCar : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        for(auto AIcar : blackboard->manCars->GetEntities()){
            auto cTotem = static_cast<CTotem*>(AIcar.get()->GetComponent(CompType::TotemComp).get()); 
            // Si algun coche tenia el totem .... lo pierde
            if(cTotem->active == true){
                //std::cout << "y una IA tiene el totem" << std::endl;  
                return true;           
            }                                                    // para salirnos y no hacer mas calculos
        }
        auto cTotem = static_cast<CTotem*>(blackboard->manCars->GetCar().get()->GetComponent(CompType::TotemComp).get()); 
        if(cTotem->active == true){
        //std::cout << "y el player tiene el totem" << std::endl;     
            return true;
        }
        //std::cout << "Nadie tiene el totem" << std::endl;     
        return false;
    } 
};

struct LookEnemy : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //return false;
        //std::cout << "miramos a ver si te veo,,,,,,," << std::endl;
        return blackboard->manCars->anyCarInVisionRange(blackboard->actualCar, 60);
        // estrategia: que tu vector director y el vector alenemigo tengan una difrencia de maximo 5 grados
        //return true;
    } 
};

struct HaveBanana : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == typeCPowerUp::TeleBanana){
            //std::cout << "Tengo una Banana " << std::endl;
            return true;
        }
        //std::cout << "No es una Banana.. es un melon y no veo a nadie chaoooo " << std::endl;
        return false;
    } 
};




SystemBtPowerUp::SystemBtPowerUp(){

    //   CREACION DEL ARBOL DE DECISIONES
    selectorBehaviourTree = make_shared<selector>();

    shared_ptr<HavePowerUp> c_havePowerUp = make_shared<HavePowerUp>();             // Condicion -->    ¿Tengo un PowerUp?
    shared_ptr<DontHavePoweUp> c_dontHavePoweUp = make_shared<DontHavePoweUp>();

    shared_ptr<Inverter> m_inverter1 = make_shared<Inverter>();                     // Decorator -->    inverter para Condicion ¿Tengo un PowerUp?

    shared_ptr<selector> selector1 = make_shared<selector>();
    shared_ptr<sequence> sequence1 = make_shared<sequence>();
    shared_ptr<CompPowerUp> c_compPowerUp = make_shared<CompPowerUp>();             // Condicion -->    ¿El powerUp que tengo es Escudo, Nitro o Pudding?
    shared_ptr<ThrowPowerUp> a_throwPowerUp = make_shared<ThrowPowerUp>();          // Accion    -->    Lanzar el powerUp

    shared_ptr<sequence> sequence2 = make_shared<sequence>();
    shared_ptr<HaveRoboJorobo> c_haceRoboJorobo = make_shared<HaveRoboJorobo>();
    shared_ptr<sequence> sequence21 = make_shared<sequence>();
    shared_ptr<HaveTotemOtherCar> c_haveTotemOtherCar = make_shared<HaveTotemOtherCar>();
    // como hijo tenemos el lanzar powerUp.... misma struct que el sequence1

    shared_ptr<selector> selector2 = make_shared<selector>();
    shared_ptr<sequence> sequence3 = make_shared<sequence>();
    shared_ptr<LookEnemy> c_lookEnemy = make_shared<LookEnemy>();
    // como hijo volvemos a tener lanzar el powerUp
    shared_ptr<sequence> sequence4 = make_shared<sequence>();
    shared_ptr<HaveBanana> c_haveBanana = make_shared<HaveBanana>();
    // como no... el hijo lanzar powerUp

    // podemos salir si .. tenemos el melonMolon pero no vemos a nadie

    std::cout << "------------------------ Behaviour Tree Power Up --------------------------------" << std::endl;
    // ASIGNACION DEL ARBOL DE DECISIONES

    m_inverter1->addChild(c_havePowerUp);

    selectorBehaviourTree->addChild(m_inverter1);
    selectorBehaviourTree->addChild(selector1);
    selectorBehaviourTree->addChild(selector2);
    //selectorBehaviourTree->addChild(sequence1);
    //selectorBehaviourTree->addChild(sequence2);
    //selectorBehaviourTree->addChild(selector2);
    selectorBehaviourTree->addChild(c_dontHavePoweUp); // hasta que acabe el arbol para imprimir una salida

    selector1->addChild(sequence1);
    selector1->addChild(sequence2);
    
    sequence1->addChild(c_compPowerUp);
    sequence1->addChild(a_throwPowerUp);

    sequence2->addChild(c_haceRoboJorobo);
    sequence2->addChild(sequence21);

    sequence21->addChild(c_haveTotemOtherCar);
    sequence21->addChild(a_throwPowerUp);        // no implementado

    selector2->addChild(sequence3);
    selector2->addChild(sequence4);

    sequence3->addChild(c_lookEnemy);            // no implementado
    sequence3->addChild(a_throwPowerUp);

    sequence4->addChild(c_haveBanana);
    sequence4->addChild(a_throwPowerUp);
    
}




void SystemBtPowerUp::update(CarAI* actualCar, ManCar* manCars,ManPowerUp* manPowerUps, ManBoxPowerUp* manBoxPowerUps, ManTotem* manTotems, ManWayPoint* manWayPoint){
    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, manCars, manPowerUps, manBoxPowerUps, manTotems, manWayPoint);
    selectorBehaviourTree->run(blackboard.get());
}