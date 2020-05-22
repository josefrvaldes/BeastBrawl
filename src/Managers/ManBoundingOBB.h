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

#include "Manager.h"
#include "../../include/glm/vec3.hpp"

using namespace std;
using namespace glm;

struct CBoundingPlane;

class ManBoundingOBB : public Manager {
   public:
    ManBoundingOBB();
    ~ManBoundingOBB();
    void CreateBoundingOBB(const vector<glm::vec3> &, const vector<glm::vec3> &);   // Debemos pasarle todos los vertices que componente nuestro CUBO 3D
    void Integrate(float) override;
    void SubscribeToEvents() override;

   private:
   //vector<shared_ptr<CBoundingPlane>> planes;
   //double EuclideanDis(const glm::vec3 &p1, const glm::vec3 &p2) const;
};
