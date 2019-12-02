#ifndef DECORATOR_H
#define DECORATOR_H

#include "behaviourTree.h"

class Decorator: public behaviourTree{


    public:
        virtual bool run() = 0;
        void addChild(behaviourTree* child);
        behaviourTree *getChild(){ return child; };

    private:
        behaviourTree* child;
};


#endif // DECORATOR_H