#pragma once
#include "behaviourTree.h"

#include <iostream>
#include <list>
#include <memory>

using namespace std;

class composite : public behaviourTree{

	public:
		const list<shared_ptr<behaviourTree>>& getChildren() const {return children;}
		void addChild (shared_ptr<behaviourTree> child) {children.push_back(child);}

    private:
		list<shared_ptr<behaviourTree>> children;


};