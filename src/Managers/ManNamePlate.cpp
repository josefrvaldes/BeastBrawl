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
 * @author Clover Games Studio
 * 
 */
 
 
 #include "ManNamePlate.h"

#include <Components/CNamePlate.h>
#include <Components/CId.h>
#include <Components/CCar.h>
#include <Entities/NamePlate.h>
#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>

ManNamePlate::ManNamePlate(ManCar* cars){
    // for(const auto& car : cars->GetEntities()){
    //     //cout <<"Entraaaaaa\n";
    //     if(car.get() != cars->GetCar().get()){
    //         std::cout << "sdfasdfsa" << std::endl;
    //         auto cIdCar = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
    //         auto namePlate = make_shared<NamePlate>(cIdCar->id);
    //         entities.push_back(namePlate);
    //     }
    // }
}

void ManNamePlate::CreateNamePlate(Entity* car){
    auto cIdCar = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
    auto cCharacter = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());

    string billboardPath;
    if(cCharacter->character == mainCharacter::DRAGON){
        billboardPath = "media/BDragon.png";
    }else if(cCharacter->character == mainCharacter::GORILLA){
        billboardPath = "media/BGorila.png";
    }else if(cCharacter->character == mainCharacter::OCTOPUS){
        billboardPath = "media/BOctopus.png";        
    }else if(cCharacter->character == mainCharacter::PENGUIN){
        billboardPath = "media/BPinguino.png";        
    }else if(cCharacter->character == mainCharacter::SHARK){
        billboardPath = "media/BTiburon.png";        
    }else if(cCharacter->character == mainCharacter::TIGER){
        billboardPath = "media/BTigre.png";        
    }else{
        billboardPath = "media/BPinguino.png";
    }
    auto namePlate = make_shared<NamePlate>(cIdCar->id,billboardPath);
    entities.push_back(namePlate);
}

ManNamePlate::~ManNamePlate(){
    entities.clear();
    entities.shrink_to_fit();
}
