#pragma once

#include "CLEntity.h"

namespace CLE {

    class CLCamera : public CLEntity {
        public:
            CLCamera() {};
            CLCamera(unsigned int idEntity) : CLEntity(idEntity) {};
            ~CLCamera() {};

            void SetCameraConfig(float n, float f);

            void SetPerspective(bool b)     { perspective = b; };

            bool IsPerspective() const      { return perspective; };

            void Draw(glm::mat4&);

        protected:

            float near;
            float far;

            bool perspective;

    };
}