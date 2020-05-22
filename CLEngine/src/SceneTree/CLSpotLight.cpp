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
 
 
 #include "CLSpotLight.h"

using namespace CLE;

CLSpotLight::CLSpotLight(unsigned int idEntity): CLEntity(idEntity) {
    intensity = glm::vec3(1.0f, 1.0f, 1.0f);
}

void CLSpotLight::SetLightAttributes(glm::vec3 dir,float cut,float outCut,glm::vec3 inte,glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float consta, float lin, float quad){
    direction = dir;
    cutOff  = cut;
    outerCutOff = outCut;
    intensity = inte;
    ambient = amb;
    diffuse = diff;
    specular = spec;
    constant = consta;
    linear = lin;
    quadratic = quad;

}

void CLSpotLight::Draw(GLuint shaderID) {

}

void CLSpotLight::DrawDepthMap(GLuint shaderID){}
