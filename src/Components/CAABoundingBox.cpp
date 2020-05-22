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
 
 
 #include "CAABoundingBox.h"

#include <Systems/Utils.h>

// https://www.youtube.com/watch?v=Iu6nAXFm2Wo&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=4
IntersectData CAABoundingBox::IntersectCAABoundingBox(CAABoundingBox &other) const {
    vec3 distances1 = other.minExtents - maxExtents;    // distancia en todos los ejes entre uno y otro
    vec3 distances2 = minExtents - other.maxExtents;    // distancia en todos los ejes entre uno y otro
    vec3 distances = glm::max(distances1, distances2);  // mayor de esas distancias
    float maxDistance = Utils::Vec3MaxValue(distances);
    bool intersects = maxDistance < 0;
    return IntersectData(intersects, distances);
}    