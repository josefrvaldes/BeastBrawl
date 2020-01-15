#include "./ManNamePlate.h"
#include "../Entities/NamePlate.h"
#include "../Components/CNamePlate.h"
#include "../Components/CId.h"
#include "../Entities/CarAI.h"

ManNamePlate::ManNamePlate(ManCar* cars){
    for(auto car : cars->GetEntitiesAI()){
        //cout <<"Entraaaaaa\n";
        auto cIdCar = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
        auto namePlate = make_shared<NamePlate>(cIdCar->id);
        entities.push_back(namePlate);
    }
}


ManNamePlate::~ManNamePlate(){
    entities.clear();
    entities.shrink_to_fit();
}
