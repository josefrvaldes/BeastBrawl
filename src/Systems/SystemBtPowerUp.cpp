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
 * @author Antonio Jose Martinez Garcia
 * 
 */
 
 
 #include "SystemBtPowerUp.h"


#include <behaviourTree/behaviourTree.h>
#include <behaviourTree/selector.h>
#include <behaviourTree/sequence.h>
#include <behaviourTree/decorator.h>

#include <behaviourTree/Blackboard.h>
#include <Components/CTotem.h>
#include <Components/CBrainAI.h>
#include <Components/CHurt.h>



void SystemBtPowerUp::AddManager(Manager &m) {
    managers.push_back(&m);
}

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------     Condiciones   ------------------------------------------- //
//Condicion -> Estamos heridos?
struct IsHurt_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cHurt = static_cast<CHurt*>(blackboard->actualCar->GetComponent(CompType::HurtComp).get());
        if(cHurt->hurt)
            return true;
        return false;
    }
};


//Condicion -> Tenemos algun powerUp?
struct HaveSomePowerUp_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        //cout << "Tipo de PU: " << int(cPowerUp->typePowerUp) << "\n";
        if(cPowerUp->typePowerUp == typeCPowerUp::None)
            return false;
        return true;
    }
};

//Condicion -> Tenemos X powerUp?
template <typename Type_PU, Type_PU Type_X>
struct HaveThisPowerUp_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == Type_X)
            return true;
        return false;
    } 
};

//Condicion -> Veo el totem en el suelo?
struct TotemInVision_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrainAI->totemInVision)
            return true;
        return false;
    } 
};

//Condicion -> Tengo target Car asignado?
struct HaveTargetCar_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrainAI->targetCar)
            return true;
        return false;
    } 
};

//Condicion -> Tengo el target en vision?
struct TargetCarInVision_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        for(const auto& car : cBrainAI->carInVision){
            if(cBrainAI->targetCar == car)
                return true;
        }
        return false;
    } 
};

//Condicion -> Tengo algun coche en vision?
struct CarInVision_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        if(cBrainAI->carInVision.size() > 0)
            return true;
        return false;
    } 
};

//Condicion -> Tengo el coche con totem en vision?  // To-Do: ponerle un distancia incluso? preguntar a carlos
struct CarWithTotemInVision_pu : public behaviourTree {
    float distanceToCar = 450.0;
    
    virtual bool run(Blackboard* blackboard) override {
        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
        for(const auto& car : cBrainAI->carInVision){
            auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
            if(cTotem->active){
                auto cTransCar = static_cast<CTransformable*>(blackboard->actualCar->GetComponent(CompType::TransformableComp).get());
                auto cTransTarget = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
                if(glm::distance(cTransCar->position, cTransTarget->position) <= distanceToCar){
                    return true;
                }
                break;
            }
        }
        return false;
    } 
};


//Condicion -> Algun coche lleva el totem?
struct TotemInSomeCar_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        for(const auto& car : blackboard->manCars->GetEntities()){
            auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
            if(cTotem->active && car.get()!=blackboard->actualCar)
                return true;
        }
        return false;
    } 
};

//Condicion -> Coche con totem con BASTANTE tiempo restante?
struct HighTimeToEndCar_pu : public behaviourTree {
    //int timeMin = 10000; // 10 segundos

    virtual bool run(Blackboard* blackboard) override {
        for(const auto& car : blackboard->manCars->GetEntities()){
            auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
            auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
            if(cTotem->active && car.get()!=blackboard->actualCar && (cTotem->SEGUNDOS*1000 - cTotem->accumulatedTime) > cBrainAI->timeFollowDirect)
                return true;
        }
        return false;
    } 
};

//Condicion -> Destino dentro de angulo?
struct DestinationInAngle_pu : public behaviourTree {
    float fovSmall = 20.0;
    float minDistance = 70.0;

    virtual bool run(Blackboard* blackboard) override {
        auto cPosDestination = static_cast<CPosDestination*>(blackboard->actualCar->GetComponent(CompType::PosDestination).get());
        auto cTransCar = static_cast<CTransformable*>(blackboard->actualCar->GetComponent(CompType::TransformableComp).get());

        // calcular un desplazamiento para ser en tercera persona
        float posXActualCar = cTransCar->position.x + 38.9 * cos(glm::radians(cTransCar->rotation.y));
        float posZActualCar = cTransCar->position.z - 38.9 * sin(glm::radians(cTransCar->rotation.y));

        // vector between actualCar and otherCar
        float vetorToEntityX = (cPosDestination->position.x - posXActualCar);
        float vetorToEntityZ = (cPosDestination->position.z - posZActualCar);

        // calculate position rotated of otherCar atan2
        float valueAtan2 = glm::degrees( atan2(vetorToEntityZ, vetorToEntityX) );
        valueAtan2 = 180.0 - valueAtan2;  // se le restan ya que el eje empieza en el lado contrario
        if (valueAtan2 < 0)
            valueAtan2 += 360;

        //compare with actualCar actualRotation
        float offset = valueAtan2 - cTransCar->rotation.y;
        if(offset<fovSmall && offset>-fovSmall && glm::distance(cTransCar->position, cPosDestination->position)>=minDistance)
            return true;
        return false;
    } 
};

//    None,               // 0
//    RoboJorobo,         // 1
//    SuperMegaNitro,     // 2
//    PudinDeFrambuesa,   // 3
//    EscudoMerluzo,      // 4
//    TeleBanana,         // 5
//    MelonMolon          // 6


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------     ACCIONES   ------------------------------------------- //
//Accion -> Lanzamos el powerUp
struct ThrowPowerUp_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std::cout << "Lanzaaas el powerUp beibeee" << std::endl;
        shared_ptr<DataMap> data = make_shared<DataMap>();
        (*data)[ACTUAL_CAR] = blackboard->actualCar;
        EventManager::GetInstance().AddEventMulti(Event{EventType::THROW_POWERUP_AI, data});
        return true;
    }
};

//Accion -> Guardar el powerUp
struct SavePowerUp_pu : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        // lo guardas devolviendo true
        return true;
    }
};


SystemBtPowerUp::SystemBtPowerUp(){
    using t_PU = typeCPowerUp;
    //   CREACION DEL ARBOL DE DECISIONES
    selectorBehaviourTree = make_shared<selector>();

    // Acciones
    shared_ptr<ThrowPowerUp_pu> a_throwPowerUp = make_shared<ThrowPowerUp_pu>();
    shared_ptr<SavePowerUp_pu> a_savePowerUp = make_shared<SavePowerUp_pu>();
    // Condiciones
    shared_ptr<IsHurt_pu> c_isHurt = make_shared<IsHurt_pu>();
    shared_ptr<HaveSomePowerUp_pu> c_haveSomePowerUp = make_shared<HaveSomePowerUp_pu>();           // Condicion -->    ¿Tengo un PowerUp?
    shared_ptr<HaveThisPowerUp_pu<t_PU, t_PU::MelonMolon>> c_haveThisPowerUp_MM = make_shared<HaveThisPowerUp_pu<t_PU, t_PU::MelonMolon>>();
    shared_ptr<HaveThisPowerUp_pu<t_PU, t_PU::TeleBanana>> c_haveThisPowerUp_TB = make_shared<HaveThisPowerUp_pu<t_PU, t_PU::TeleBanana>>();
    shared_ptr<HaveThisPowerUp_pu<t_PU, t_PU::SuperMegaNitro>> c_haveThisPowerUp_SMN = make_shared<HaveThisPowerUp_pu<t_PU, t_PU::SuperMegaNitro>>();
    shared_ptr<HaveThisPowerUp_pu<t_PU, t_PU::PudinDeFrambuesa>> c_haveThisPowerUp_PF = make_shared<HaveThisPowerUp_pu<t_PU, t_PU::PudinDeFrambuesa>>();
    shared_ptr<HaveThisPowerUp_pu<t_PU, t_PU::EscudoMerluzo>> c_haveThisPowerUp_EM = make_shared<HaveThisPowerUp_pu<t_PU, t_PU::EscudoMerluzo>>();
    shared_ptr<HaveThisPowerUp_pu<t_PU, t_PU::RoboJorobo>> c_haveThisPowerUp_RJ = make_shared<HaveThisPowerUp_pu<t_PU, t_PU::RoboJorobo>>();
    shared_ptr<TotemInVision_pu> c_totemInVision = make_shared<TotemInVision_pu>();
    shared_ptr<HaveTargetCar_pu> c_haveTargetCar = make_shared<HaveTargetCar_pu>();
    shared_ptr<TargetCarInVision_pu> c_targetCarInVision = make_shared<TargetCarInVision_pu>();
    shared_ptr<CarInVision_pu> c_carInVision = make_shared<CarInVision_pu>();
    shared_ptr<CarWithTotemInVision_pu> c_carWithTotemInVision = make_shared<CarWithTotemInVision_pu>();
    shared_ptr<TotemInSomeCar_pu> c_totemInSomeCar = make_shared<TotemInSomeCar_pu>();
    shared_ptr<HighTimeToEndCar_pu> c_highTimeToEndCar = make_shared<HighTimeToEndCar_pu>();
    shared_ptr<DestinationInAngle_pu> c_destinationInAngle = make_shared<DestinationInAngle_pu>();


    // tengo PU
    shared_ptr<Inverter> m_inverter = make_shared<Inverter>();                                 // Decorator -->    inverter para Condicion ¿Tengo un PowerUp?

    // melon molon
        // no tiene selectores o secuencias

    // telebanana
    shared_ptr<sequence> sequence_11 = make_shared<sequence>();
    shared_ptr<selector> selector_11 = make_shared<selector>();
    shared_ptr<sequence> sequence_12 = make_shared<sequence>();
    shared_ptr<selector> selector_12 = make_shared<selector>();
    shared_ptr<sequence> sequence_13 = make_shared<sequence>();
    shared_ptr<sequence> sequence_14 = make_shared<sequence>();

    // pudding o escudo
    shared_ptr<sequence> sequence_21 = make_shared<sequence>();
    shared_ptr<selector> selector_21 = make_shared<selector>();

    // nitro
    shared_ptr<sequence> sequence_31 = make_shared<sequence>();
    shared_ptr<selector> selector_31 = make_shared<selector>();
    shared_ptr<sequence> sequence_32 = make_shared<sequence>();
    shared_ptr<selector> selector_32 = make_shared<selector>();
    shared_ptr<sequence> sequence_33 = make_shared<sequence>();

    // robojorobo
    shared_ptr<sequence> sequence_41 = make_shared<sequence>();
    shared_ptr<selector> selector_41 = make_shared<selector>();
    shared_ptr<sequence> sequence_42 = make_shared<sequence>();



    //std::cout << "------------------------ Behaviour Tree Power Up --------------------------------" << std::endl;
    // ASIGNACION DEL ARBOL DE DECISIONES

    // estoy herido
    selectorBehaviourTree->addChild(c_isHurt);

    // tengo power up
    selectorBehaviourTree->addChild(m_inverter);
        m_inverter->addChild(c_haveSomePowerUp);

    // melon molon
    selectorBehaviourTree->addChild(c_haveThisPowerUp_MM);

    // telebanana
    selectorBehaviourTree->addChild(sequence_11);
        sequence_11->addChild(c_haveThisPowerUp_TB);
        sequence_11->addChild(selector_11);
            selector_11->addChild(sequence_12);
                sequence_12->addChild(c_haveTargetCar);
                sequence_12->addChild(selector_12);
                    selector_12->addChild(sequence_13);
                        sequence_13->addChild(c_targetCarInVision);
                        sequence_13->addChild(a_throwPowerUp);
                    selector_12->addChild(a_savePowerUp);
            selector_11->addChild(sequence_14);
                sequence_14->addChild(c_carInVision);
                sequence_14->addChild(a_throwPowerUp);
            selector_11->addChild(a_savePowerUp);

    // pudding y escudo
    selectorBehaviourTree->addChild(sequence_21);
        sequence_21->addChild(selector_21);
            selector_21->addChild(c_haveThisPowerUp_PF);
            selector_21->addChild(c_haveThisPowerUp_EM);
        sequence_21->addChild(a_throwPowerUp);
    
    // nitro
    selectorBehaviourTree->addChild(sequence_31);
        sequence_31->addChild(c_haveThisPowerUp_SMN);
        sequence_31->addChild(selector_31);
            selector_31->addChild(sequence_32);
                sequence_32->addChild(c_totemInSomeCar);
                sequence_32->addChild(selector_32);
                    selector_32->addChild(c_carWithTotemInVision);
                    selector_32->addChild(c_highTimeToEndCar);
                sequence_32->addChild(a_throwPowerUp);
            selector_31->addChild(sequence_33);
                sequence_33->addChild(c_destinationInAngle);
                sequence_33->addChild(a_throwPowerUp);
            selector_31->addChild(a_savePowerUp);

    // robojorobo
    selectorBehaviourTree->addChild(sequence_41);
        sequence_41->addChild(c_haveThisPowerUp_RJ);
        sequence_41->addChild(selector_41);
            selector_41->addChild(sequence_42);
                sequence_42->addChild(c_totemInVision);
                sequence_42->addChild(a_savePowerUp);
            selector_41->addChild(a_throwPowerUp);
            
    
}




void SystemBtPowerUp::update(CarAI* actualCar){
    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, static_cast<ManCar*>(managers[0]), static_cast<ManPowerUp*>(managers[1]), 
            static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManTotem*>(managers[3]), 
            static_cast<ManWayPoint*>(managers[4]), static_cast<ManNavMesh*>(managers[5]));
    
    selectorBehaviourTree->run(blackboard.get());
}
