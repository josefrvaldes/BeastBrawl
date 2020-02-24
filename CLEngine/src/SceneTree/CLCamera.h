#pragma once

#include "CLEntity.h"
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

            bool IsPerspective() const      { return perspective; };

            void Draw(glm::mat4&);
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

            //glm::vec3 target;

    };
}