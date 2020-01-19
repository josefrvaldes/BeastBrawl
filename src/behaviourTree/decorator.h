#pragma once

#include "behaviourTree.h"
#include <memory>

using namespace std;

struct Blackboard;

class Decorator: public behaviourTree{
    public:
        virtual bool run(Blackboard* blackboard) = 0;
        void addChild(shared_ptr<behaviourTree> child);
        shared_ptr<behaviourTree> getChild(){ return child; };

    private:
        shared_ptr<behaviourTree> child;
};

