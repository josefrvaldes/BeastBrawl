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
