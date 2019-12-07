#ifndef DECORATOR_H
#define DECORATOR_H

#include "behaviourTree.h"
#include <memory>

using namespace std;

class Decorator: public behaviourTree{


    public:
        virtual bool run() = 0;
        void addChild(shared_ptr<behaviourTree> child);
        shared_ptr<behaviourTree> getChild(){ return child; };

    private:
        shared_ptr<behaviourTree> child;
};


#endif // DECORATOR_H