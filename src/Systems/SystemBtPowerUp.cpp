#include "SystemBtPowerUp.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
bool door = false;  // imaginemos que door=false es = puerta cerrada "door is open?"
bool key = false;   // tenemos una llave
// ACCION DE ABRIR LA PUERTA
struct openDoor : public behaviourTree {
    virtual bool run() override {
        door = true;
        cout << "Abrimos la puerta" << endl;
        return true;
    }
};
//ACCION COGER LA LLAVE
struct getKey : public behaviourTree {
    virtual bool run() override {
        key = true;
        cout << "Cogemos la llave" << endl;
        return true;
    }
};
//CONDICION PUERTA ABIERTA?
struct haveDoorOpen : public behaviourTree {
    virtual bool run() override {
        cout << "Comprobamos si la puerta esta abierta: " << boolalpha << door << endl;
        return door;
    }
};
//CONDICION TENEMOS LLAVE?
struct haveKey : public behaviourTree {
    virtual bool run() override {
        cout << "Comprobamos si tenemos la llave: " << boolalpha << key << endl;
        return key;
    }
};
///// DECORATORS //////
struct Minimum : public Decorator {  // Tiene que intentar coger la llave 3 veces para que la pueda coger
    uint32_t totalTries = 3;
    uint32_t numTries = 0;
    virtual bool run() override {
        if (numTries >= totalTries)
            return getChild()->run();
        numTries++;
        cout << "Fallamos al coger la llave, intento: " << numTries << endl;
        return false;
    }
};
struct Limit : public Decorator {  // Decorator Limit
    uint32_t totalLimit = 3;
    uint32_t numLimit = 0;
    virtual bool run() override {
        if (numLimit >= totalLimit)
            return false;
        numLimit++;
        return getChild()->run();
    }
};
struct UntilFail : public Decorator {  // Decorator UntilFail
    virtual bool run() override {
        while (true) {
            bool result = getChild()->run();
            if (!result) {
                break;
            }
        }
        return true;
    }
};
struct Inverter : public Decorator {  // Decorator Inverter
    virtual bool run() override {
        return !(getChild()->run());
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

    selectorBehaviourTree = make_shared<selector>();
    shared_ptr<sequence> sequence1 = make_shared<sequence>();

    shared_ptr<haveDoorOpen> puertaAbiertaSiNo = make_shared<haveDoorOpen>();
    shared_ptr<haveKey> tengoLlaveSiNo = make_shared<haveKey>();
    shared_ptr<openDoor> abrirPuerta = make_shared<openDoor>();
    shared_ptr<getKey> cogerLlave = make_shared<getKey>();

    shared_ptr<Minimum> tryCatchKey3 = make_shared<Minimum>();

    selectorBehaviourTree->addChild(puertaAbiertaSiNo);
    selectorBehaviourTree->addChild(sequence1);
    selectorBehaviourTree->addChild(tryCatchKey3);

    sequence1->addChild(tengoLlaveSiNo);
    sequence1->addChild(abrirPuerta);

    tryCatchKey3->addChild(cogerLlave);
}

void SystemBtPowerUp::update(){
    selectorBehaviourTree->run();
}