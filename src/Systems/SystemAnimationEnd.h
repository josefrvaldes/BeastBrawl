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
 
 
 #pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "../../include/glm/glm.hpp"
using namespace std;

class Entity;
class Camera;
class Totem;
class Car;



class SystemAnimationEnd {
   public:
    SystemAnimationEnd(Camera *cam_);
    void Animate();
    void ResetTimer();
    ~SystemAnimationEnd() = default;
    void SetWinner(Car* winner_);
    Car* GetWinner() {return winner;};

   private:
    void MoveCam(int64_t interval, int64_t duration);

    Camera *cam;
    Car *winner;
    
    float rotation {45};
    const uint8_t ROTATION_SPEED = 1;

    int64_t timeStartAnim;
};