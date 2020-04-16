#pragma once

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "../../include/glew/glew.h"

using namespace std;

namespace CLE {
    //! Clase virtual para los recursos
    //! Utilizada por el polimorfismo en todas las clases de recursos
    class CLResource {
        public:
            CLResource() {};
            ~CLResource() {};

            //! Metodo virtual para algunos recursos que necesiten dibujado
            virtual void Draw(GLuint shaderID) = 0;
            //! Metodo virtual para leer ficheros
            virtual bool LoadFile(string, bool) = 0;

            void SetName(string n)  { name = n; }
            //! @returns El nombre del fichero leido
            const string GetName()       const   { return name; }

        private:

            string name;
    };

}