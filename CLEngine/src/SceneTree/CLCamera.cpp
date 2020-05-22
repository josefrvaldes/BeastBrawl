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
 
 
 #include "CLCamera.h"
#include "../Frustum/CLFrustum.h"

using namespace CLE;

CLCamera::CLCamera(unsigned int idEntity) : CLEntity(idEntity) {
    left = 0.0f;
    right = 1280.0f;
    top = 0.0f;
    bottom = 720.0f;
    m_near= 0.1f;
    m_far = 5000.0f;
    fov = 70.0f;
    aspect = 1280.f / 720.0f; // 2,05

    frustum = make_unique<CLFrustum>();
}

/**
 * Cambia los valores de configuracion de la camara.
 */
void CLCamera::SetCameraConfig(GLfloat n, GLfloat f, GLfloat r, GLfloat l, GLfloat t, GLfloat b) {
    m_near = n; m_far = f; right = r; left = l; top = t; bottom = b;
    CalculateProjectionMatrix();
}

/**
 * Calcula la matriz de proyeccion segun si usamos perspectiva o ortografico.
 */
glm::mat4 CLCamera::CalculateProjectionMatrix() {
    glm::mat4 projection = glm::mat4(0.0f);
    if (perspective) {
        //TO-DO: Poner variable el spect
        projection = glm::perspective(glm::radians(fov), aspect, m_near, m_far);
    }
    else {
        projection = glm::ortho(left, right, bottom, top, m_near, m_far);
    }

    return projection;
}


void CLCamera::Draw(GLuint shaderID) {

}

void CLCamera::DrawDepthMap (GLuint shaderID){}



void CLCamera::CalculateFrustum(glm::mat4& proj, glm::mat4& view) {
    frustum->Transform(proj, view);
}


/*
CLE::CLFrustum CLCamera::CalculateFrustum(float fnear, float ffar) {
    CLFrustum returnFrustum;
    
    // Calculate the near and far plane points
    float nearHeight = 2 * tan(fov / 2) * fnear;
    float nearWidth = nearHeight * aspect;
    float farHeight = 2 * tan(fov / 2) * ffar;
    float farWidth = farHeight * aspect;
    
    // And their centers
    vec3 nearCenter = cameraUp + (cameraTarget * fnear);
    vec3 farCenter = cameraUp + (cameraTarget * ffar);
    
    returnFrustum.center = cameraUp + (cameraTarget * (ffar / 2.0f));

    auto hola1 = nearCenter - (top * (nearHeight / 2.0f)) - (right * (nearWidth / 2.0f));
    auto hola2 = returnFrustum.fnear[CLFrustum::COORD_BOTTOMLEFT];
    
    // And the actual coordinates
    returnFrustum.fnear[CLFrustum::COORD_BOTTOMLEFT] = nearCenter - (top * (nearHeight / 2.0f)) - (right * (nearWidth / 2.0f));
    returnFrustum.fnear[CLFrustum::COORD_BOTTOMRIGHT] = nearCenter - (top * (nearHeight / 2.0f)) + (right * (nearWidth / 2.0f));
    returnFrustum.fnear[CLFrustum::COORD_TOPLEFT] = nearCenter + (top * (nearHeight / 2.0f)) - (right * (nearWidth / 2.0f));
    returnFrustum.fnear[CLFrustum::COORD_TOPRIGHT] = nearCenter + (top * (nearHeight / 2.0f)) + (right * (nearWidth / 2.0f));
    
    returnFrustum.ffar[CLFrustum::COORD_BOTTOMLEFT] = farCenter - (top * (farHeight / 2.0f)) - (right * (farWidth / 2.0f));
    returnFrustum.ffar[CLFrustum::COORD_BOTTOMRIGHT] = farCenter - (top * (farHeight / 2.0f)) + (right * (farWidth / 2.0f));
    returnFrustum.ffar[CLFrustum::COORD_TOPLEFT] = farCenter + (top * (farHeight / 2.0f)) - (right * (farWidth / 2.0f));
    returnFrustum.ffar[CLFrustum::COORD_TOPRIGHT] = farCenter + (top * (farHeight / 2.0f)) + (right * (farWidth / 2.0f));
    
    // Then create our frustum planes
    returnFrustum.planes[CLFrustum::PLANE_NEAR].CreateFromPoints(returnFrustum.fnear[CLFrustum::COORD_BOTTOMLEFT], returnFrustum.fnear[CLFrustum::COORD_BOTTOMRIGHT], returnFrustum.fnear[CLFrustum::COORD_TOPLEFT]);
    returnFrustum.planes[CLFrustum::PLANE_FAR].CreateFromPoints(returnFrustum.ffar[CLFrustum::COORD_TOPLEFT],returnFrustum.ffar[CLFrustum::COORD_TOPRIGHT], returnFrustum.ffar[CLFrustum::COORD_BOTTOMLEFT]);
    returnFrustum.planes[CLFrustum::PLANE_LEFT].CreateFromPoints(returnFrustum.fnear[CLFrustum::COORD_BOTTOMLEFT], returnFrustum.fnear[CLFrustum::COORD_TOPLEFT], returnFrustum.ffar[CLFrustum::COORD_BOTTOMLEFT]);
    returnFrustum.planes[CLFrustum::PLANE_RIGHT].CreateFromPoints(returnFrustum.ffar[CLFrustum::COORD_TOPRIGHT], returnFrustum.fnear[CLFrustum::COORD_TOPRIGHT], returnFrustum.ffar[CLFrustum::COORD_BOTTOMRIGHT]);
    returnFrustum.planes[CLFrustum::PLANE_TOP].CreateFromPoints(returnFrustum.ffar[CLFrustum::COORD_TOPLEFT], returnFrustum.fnear[CLFrustum::COORD_TOPLEFT], returnFrustum.ffar[CLFrustum::COORD_TOPRIGHT]);
    returnFrustum.planes[CLFrustum::PLANE_BOTTOM].CreateFromPoints(returnFrustum.fnear[CLFrustum::COORD_BOTTOMLEFT], returnFrustum.ffar[CLFrustum::COORD_BOTTOMLEFT], returnFrustum.fnear[CLFrustum::COORD_BOTTOMRIGHT]);
    
    return returnFrustum;
}
*/