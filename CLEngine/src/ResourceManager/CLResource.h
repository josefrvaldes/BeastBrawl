#pragma once

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "../../include/glew/glew.h"

using namespace std;
namespace CLE {

    class CLResource {
        public:
            CLResource() {};
            ~CLResource() {};

            virtual void Draw(glm::mat4) = 0;
            virtual bool LoadFile(string) = 0;

            void SetName(string n)  { name = n; }

            const string GetName()       const   { return name; }

        private:

            string name;
    };

}