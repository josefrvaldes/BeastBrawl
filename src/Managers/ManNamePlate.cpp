#include "ManNamePlate.h"

#include <Components/CNamePlate.h>
#include <Components/CId.h>
#include <Entities/NamePlate.h>
#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>

ManNamePlate::ManNamePlate(ManCar* cars){
    for(const auto& car : cars->GetEntities()){
        //cout <<"Entraaaaaa\n";
        if(car.get() != cars->GetCar().get()){
            std::cout << "sdfasdfsa" << std::endl;
            auto cIdCar = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
            auto namePlate = make_shared<NamePlate>(cIdCar->id);
            entities.push_back(namePlate);
        }
    }
}


ManNamePlate::~ManNamePlate(){
    entities.clear();
    entities.shrink_to_fit();
}
