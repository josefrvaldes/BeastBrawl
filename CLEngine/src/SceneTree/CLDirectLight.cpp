#include "CLDirectLight.h"

using namespace CLE;

CLDirectLight::CLDirectLight(unsigned int idEntity): CLEntity(idEntity) {
    intensity = glm::vec3(1.0f, 1.0f, 1.0f);
}

void CLDirectLight::SetLightAttributes(glm::vec3 dir,glm::vec3 inte,glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float consta, float lin, float quad){
    direction = dir;
    intensity = inte;
    ambient = amb;
    diffuse = diff;
    specular = spec;
    constant = consta;
    linear = lin;
    quadratic = quad;

}

void CLDirectLight::Draw(GLuint shaderID) {

}

void CLDirectLight::DrawDepthMap(GLuint shaderID){}
