#include "composite.h"

#include <memory>

using namespace std;
class selector: public composite{

  public:
	  virtual bool run() override;

};