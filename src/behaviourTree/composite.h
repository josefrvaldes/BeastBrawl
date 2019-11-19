#pragma once
#include "behaviourTree.h"

#include <iostream>
#include <list>

using namespace std;

class composite: public behaviourTree{

	public:
		const list<behaviourTree*>& getChildren() const {return children;}
		void addChild (behaviourTree* child) {children.push_back(child);}

    private:
		list<behaviourTree*> children;


};