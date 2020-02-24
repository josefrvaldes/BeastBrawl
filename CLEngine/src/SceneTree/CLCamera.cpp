#include "CLCamera.h"

using namespace CLE;

CLCamera::CLCamera(unsigned int idEntity) : CLEntity(idEntity) {
    left = 0.0f;
    right = 1280.0f;
    top = 0.0f;
    bottom = 720.0f;
    near= 0.1f;
    far = 5000.0f;
    fov = 45.0f;
}

/**
 * Cambia los valores de configuracion de la camara.
 */
void CLCamera::SetCameraConfig(GLfloat n, GLfloat f, GLfloat r, GLfloat l, GLfloat t, GLfloat b) {
    near = n; far = f; right = r; left = l; top = t; bottom = b;
    CalculateProjectionMatrix();
}

/**
 * Calcula la matriz de proyeccion segun si usamos perspectiva o ortografico.
 */
glm::mat4 CLCamera::CalculateProjectionMatrix() {
    glm::mat4 projection = glm::mat4(0.0f);
    if (perspective) {
        //TO-DO: Poner variable el spect
        projection = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, near, far);
    }
    else {
        projection = glm::ortho(left, right, bottom, top, near, far);
    }

    return projection;
}


void CLCamera::Draw(glm::mat4&) {

}