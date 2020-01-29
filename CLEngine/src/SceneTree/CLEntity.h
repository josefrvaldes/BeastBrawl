#pragma once

namespace CLE {

    class CLEntity {
        public:
            CLEntity(){};
            CLEntity(unsigned int idEntity) : id(idEntity) {}
            ~CLEntity(){};

            unsigned int GetID() { return id; }

            virtual void Draw () = 0;

        protected:

            unsigned int id;

    };
}
