#pragma once

#include "CLEntity.h"

namespace CLE {

    class CLSpotLight : public CLEntity {
        public:
        
            CLSpotLight() {};
            CLSpotLight(unsigned int idEntity);
            ~CLSpotLight() {};

            void SetDirection(glm::vec3 dir)        { direction = dir;  };
            void SetCutOff(float cut)               { cutOff = cut;     };
            void SetOuterCutOff(float cut)          { outerCutOff = cut;};
            void SetIntensity(glm::vec3 i)          { intensity = i;    };
            void SetAmbient(glm::vec3 amb)          { ambient = amb;    };
            void SetDiffuse(glm::vec3 diff)         { diffuse = diff;   };
            void SetSpecular(glm::vec3 spec)        { specular = spec;  };
            void SetConstant(float cons)            { constant = cons;  };
            void SetLinear(float lin)               { linear = lin;     };
            void SetQuadratic(float quad)           { quadratic = quad; };  
            void SetLightAttributes(glm::vec3 direction,float cutOff,float outerCutOff, glm::vec3 intensity,glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
            void SetOn(bool b)                      { on = b; };

            glm::vec3 GetDirection() const      { return direction; };
            float     GetCutOff() const         { return cutOff;    };
            float     GetOuterCutOff() const    { return outerCutOff;};
            glm::vec3 GetIntensity() const      { return intensity; };
            glm::vec3 GetAmbient() const        { return ambient;   };
            glm::vec3 GetDiffuse() const        { return diffuse;   };
            glm::vec3 GetSpecular() const       { return specular;  };
            float     GetConstant() const       { return constant;  };
            float     GetLinear() const         { return linear;    };
            float     GetQuadratic() const      { return quadratic; };
            bool IsActive() const               { return on; };

            void Draw(GLuint shaderID);
            void DrawDepthMap(GLuint shaderID);


        private:

            bool on { true };
            glm::vec3 direction;
            float cutOff;
            float outerCutOff;
            glm::vec3 intensity;
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            
            float constant;
            float linear;
            float quadratic;

    };
}