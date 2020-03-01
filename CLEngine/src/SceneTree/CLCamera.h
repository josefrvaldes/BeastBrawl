#pragma once

#include "CLEntity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CLE {

    class CLCamera : public CLEntity {
        public:
            CLCamera() {};
            CLCamera(unsigned int);
            ~CLCamera() {};

            void SetCameraConfig(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

            void SetPerspective(bool b)     { perspective = b; };
            void SetFOV(GLfloat f)          { fov = f; };
            void SetActive(bool a)          { active = a; };
            void SetCameraTarget(glm::vec3 pos)      { cameraTarget = pos; };
            void SetCameraUp(glm::vec3 pos)      { cameraUp = pos; };


            bool IsPerspective() const      { return perspective; };
            bool IsActive() const           { return active; };

            glm::vec3 GetCameraTarget()      { return cameraTarget; };
            glm::vec3 GetCameraUp()         { return cameraUp; };

            void Draw(GLuint shaderID);
            glm::mat4 CalculateProjectionMatrix();

        private:

            GLfloat near;
            GLfloat far;
            GLfloat right;
            GLfloat left;
            GLfloat top;
            GLfloat bottom;

            GLfloat fov;

            bool perspective { true };
            bool active { true };

            glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);;
            glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);;
            //glm::vec3 target;

    };
}