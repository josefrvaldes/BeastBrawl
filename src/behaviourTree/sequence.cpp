#include "sequence.h"

bool sequence::run(){
	for (shared_ptr<behaviourTree> child : getChildren()) {  // The generic Sequence implementation.
		if (!child->run())  // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
			return false;
	}
	return true;  // All children suceeded, so the entire run() operation succeeds.
}