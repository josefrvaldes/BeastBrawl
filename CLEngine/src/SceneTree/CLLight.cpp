#include "CLLight.h"

using namespace CLE;

CLLight::CLLight(unsigned int idEntity): CLEntity(idEntity) {
    intensity = glm::vec3(1.0f, 1.0f, 1.0f);
}

void CLLight::Draw(glm::mat4&) {

}