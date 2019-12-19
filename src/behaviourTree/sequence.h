#pragma once

#include "composite.h"
#include <memory>

using namespace std;

struct Blackboard;

class sequence: public composite{
  
  public:
	  virtual bool run(Blackboard* blackboard) override;

};