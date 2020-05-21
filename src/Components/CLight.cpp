#include "CLight.h"

CLight::CLight()
{
    m_compType = CompType::LightComp; 
}

CLight::CLight(glm::vec3 in, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float cons, float lin, float quadr){
    m_compType = CompType::LightComp;
    
    intensity = in;
    ambient   = amb;
    diffuse   = diff;
    specular  = spec;        
    constant  = cons;    
    linear    = lin;     
    quadratic  = quadr;          
}         