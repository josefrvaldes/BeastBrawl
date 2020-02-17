#pragma once

#include "CLEntity.h"

namespace CLE {

    class CLLight : public CLEntity {
        public:
        
            CLLight() {};
            CLLight(unsigned int idEntity);
            ~CLLight() {};

            void SetIntensity(glm::vec3 &i)     { intensity = i; };
            void SetOn(bool b)                  { on = b; };

            glm::vec3 GetIntensity() const      { return intensity; };
            bool GetOn() const                  { return on; };

            void Draw(glm::mat4&);


        protected:

            bool on { true };
            glm::vec3 intensity;

    };
}