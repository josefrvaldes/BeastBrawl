#pragma once

#include "CLEntity.h"

namespace CLE {

    class CLLight : public CLEntity {
        public:
        
            CLLight() {};
            CLLight(unsigned int idEntity);
            ~CLLight() {};

            void SetIntensity(glm::vec3 &i)     { intensity = i; };
            void SetLightAttributes(glm::vec3 intensity,glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
            void SetOn(bool b)                  { on = b; };

            glm::vec3 GetIntensity() const      { return intensity; };
            bool GetOn() const                  { return on; };

            void Draw(GLuint shaderID);


        private:

            bool on { true };
            glm::vec3 intensity;
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            
            float constant;
            float linear;
            float quadratic;

    };
}