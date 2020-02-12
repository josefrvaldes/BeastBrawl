#pragma once

#include "Component.h"

#include <string>
#include <iostream>
#include <stack>

using namespace std;

class CPath : public Component{
public:
    CPath();
    CPath(stack<int> path);
    ~CPath(){};


    stack<int> stackPath;
};