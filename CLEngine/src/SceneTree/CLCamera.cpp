#include "CLCamera.h"

using namespace CLE;

void CLCamera::Draw(glm::mat4&) {}

/**
 * Cambia los valores de configuracion de la camara.
 */
void CLCamera::SetCameraConfig(float n, float f) {
    near = n; far = f;
}