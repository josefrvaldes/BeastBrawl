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

#include <Components/Component.h>
#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

class Entity{

public:
    Entity(){};
    ~Entity(){};

    unordered_map<CompType, shared_ptr<Component>> GetComponents() { return m_components; };
    shared_ptr<Component> GetComponent(CompType type);
    void AddComponent(const shared_ptr<Component>& c);

    bool HasComponent(CompType type);

protected:
    unordered_map<CompType, shared_ptr<Component>> m_components;

private:
};
