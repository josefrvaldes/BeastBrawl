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
         return "mrsbaxter.obj";
      case SHARK:
         return "kart_sharky.obj";
      case GORILLA:
         return "kaiserkong.obj";
      case DRAGON:
         return "kart_deacon.obj";
      case OCTOPUS:
         return "cyberoctopus.obj";
      default:
         return "kart_physics.obj";
   }
};


shared_ptr<CAnimation> animCar(TypeAnimal type) {
   shared_ptr<CAnimation> anim = make_shared<CAnimation>();
   switch(type) {
      case PENGUIN: {
         anim->AddAnimationRight(Animation("animations/baxter/selection/selectionbaxter_000001.obj", 1, false));
         anim->AddAnimationLeft(Animation("animations/dragon/selection/selectiondragon_000001.obj", 1, false));
         anim->AddAnimationIdle(Animation("animations/penguin/idle/kart_penguin_000001.obj", 1, true));
         anim->AddAnimationWin(Animation("animations/penguin/selection/selectionpenguin_000001.obj", 18, true));
         // anim->AddAnimationWin(Animation("animations/penguin/selection/selectionpenguin_000001.obj", 30, true));
      } break;
      case OCTOPUS: {
         anim->AddAnimationRight(Animation("animations/cyberoctopus/right/turnrightoctopus_000001.obj", 10, false));
         anim->AddAnimationLeft(Animation("animations/cyberoctopus/left/turnleftoctopus_000001.obj", 10, false));
         anim->AddAnimationIdle(Animation("animations/cyberoctopus/idle/cyberoctopus_000001.obj", 1, true));
         // anim->AddAnimationWin(Animation("animations/cyberoctopus/win/victoryoctopus_000001.obj", 26, true));
         anim->AddAnimationWin(Animation("animations/cyberoctopus/win/victoryoctopus_000001.obj", 18, true));
      } break;
      case DRAGON:{
         anim->AddAnimationLeft(Animation("animations/dragon/left/turnleftdragon_000001.obj", 10, false));
         anim->AddAnimationRight(Animation("animations/dragon/right/turnrightdragon_000001.obj", 10, false));
         anim->AddAnimationIdle(Animation("animations/dragon/idle/kart_deacon_000001.obj", 1, true));
         // anim->AddAnimationWin(Animation("animations/dragon/win/victorydragon_000001.obj", 31, true));
         anim->AddAnimationWin(Animation("animations/dragon/win/victorydragon_000001.obj", 18, true));
      } break;
      case GORILLA:{
         anim->AddAnimationLeft(Animation("animations/kong/left/turnleftkong_000001.obj", 10, false));
         anim->AddAnimationRight(Animation("animations/kong/right/turnrightkong_000001.obj", 10, false));
         anim->AddAnimationIdle(Animation("animations/kong/idle/kaiserkong_000001.obj", 1, true));
         // anim->AddAnimationWin(Animation("animations/kong/win/victorykong_000001.obj", 31, true));
         anim->AddAnimationWin(Animation("animations/kong/win/victorykong_000001.obj", 18, true));
      } break;
      case SHARK:{
         anim->AddAnimationLeft(Animation("animations/sharky/left/turnleftsharky_000001.obj", 10, false));
         anim->AddAnimationRight(Animation("animations/sharky/right/turnrightsharky_000001.obj", 10, false));
         anim->AddAnimationIdle(Animation("animations/sharky/idle/kart_sharky_000001.obj", 1, true));
         anim->AddAnimationWin(Animation("animations/sharky/win/victorysharky_000001.obj", 18, true));
         // anim->AddAnimationWin(Animation("animations/sharky/win/victorysharky_000001.obj", 31, true));
      } break;
      case TIGER:
      default:
       break;
   }
   anim->ActivateAnimationIdle();
   return anim;
};

