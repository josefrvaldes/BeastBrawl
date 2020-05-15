#include <string>
#include <iostream>


enum MAX_VELOCITY {
    V_LOW     = 180,
    V_MEDIUM  = 210,
    V_HIGH    = 240
};
enum ACCELERATION { // acceleration/100
    A_LOW     = 125,
    A_MEDIUM  = 150,
    A_HIGH    = 200
};
enum WEIGHT {
    W_LOW     = -50,
    W_MEDIUM  = 0,
    W_HIGH    = 50
};

enum TypeAnimal {PENGUIN, TIGER, SHARK, GORILLA, DRAGON, OCTOPUS};
std::string meshCar(TypeAnimal type) {
   switch(type) {
      case PENGUIN:
         return "kart_penguin.obj";
      case TIGER:
         return "kart_baxter.obj";
      case SHARK:
         return "kart_sharky.obj";
      case GORILLA:
         return "kart_kong.obj";
      case DRAGON:
         return "kart_deacon.obj";
      case OCTOPUS:
         return "cyberoctopus.obj";
      default:
         return "kart_physics.obj";
   }
};

