#pragma once

#include <string>
#include "../../../include/glm/glm.hpp"

namespace CLE {

    class CLResource {
        public:
            CLResource() {};
            virtual ~CLResource() = 0;

            virtual void Draw(glm::mat4&) = 0;
            virtual bool LoadFile(std::string) = 0;

            void SetName(std::string n)  { name = n; }

            const std::string GetName()       const   { return name; }

        private:

            std::string name { nullptr };
    };

}