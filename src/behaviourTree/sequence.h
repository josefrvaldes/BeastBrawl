#include "composite.h"

#include <memory>

using namespace std;

class sequence: public composite{
  
  public:
	  virtual bool run() override;

};