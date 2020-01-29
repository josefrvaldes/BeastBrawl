#pragma once

#include "CLEntity.h"
#include "../../../include/glm/glm.hpp"

namespace CLE {

    class CLLight : public CLEntity {
        public:
            CLLight() {};
            CLLight(unsigned int idEntity): CLEntity(idEntity) {};
            ~CLLight() {};

            void SetIntensity(glm::vec3 &i)     {   intensity = i;      };
            void SetOn(bool b)                  {   on = b;             };

            glm::vec3 GetIntensity() const   {   return intensity;  };
            bool GetOn() const               {   return on;         };

            void Draw();


        protected:

            bool on;
            glm::vec3 intensity;

    };
}