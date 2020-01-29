#pragma once

#include <string>

namespace CLE {

    class CLResource {
        public:
            CLResource() {};
            virtual ~CLResource() = 0;

            virtual void Draw() = 0;

            void SetName(std::string &n)  { name = n; }

            const std::string GetName()       const   { return name; }

        protected:

            std::string name = nullptr;
    };

}