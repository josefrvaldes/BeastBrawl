#include "CLPointLight.h"

using namespace CLE;

CLPointLight::CLPointLight(unsigned int idEntity): CLEntity(idEntity) {
    intensity = glm::vec3(1.0f, 1.0f, 1.0f);
}

void CLPointLight::SetLightAttributes(glm::vec3 inte,glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float consta, float lin, float quad){
    intensity = inte;
    ambient = amb;
    diffuse = diff;
    specular = spec;
    constant = consta;
    linear = lin;
    quadratic = quad;

}

void CLPointLight::Draw(GLuint shaderID) {

}

void CLPointLight::DrawDepthMap(GLuint shaderID){}
