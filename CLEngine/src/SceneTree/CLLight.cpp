#include "CLLight.h"

using namespace CLE;

CLLight::CLLight(unsigned int idEntity): CLEntity(idEntity) {
    intensity = glm::vec3(1.0f, 1.0f, 1.0f);
}

void CLLight::SetLightAttributes(glm::vec3 inte,glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float consta, float lin, float quad){
    intensity = inte;
    ambient = amb;
    diffuse = diff;
    specular = spec;
    constant = consta;
    linear = lin;
    quadratic = quad;

}

void CLLight::Draw(GLuint shaderID) {

}

void CLLight::DrawDepthMap(GLuint shaderID){}
