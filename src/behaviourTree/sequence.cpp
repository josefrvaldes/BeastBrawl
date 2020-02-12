#include "sequence.h"
#include "Blackboard.h"

bool sequence::run(Blackboard* blackboard){
	for (const shared_ptr<behaviourTree>& child : getChildren()) {  // The generic Sequence implementation.
		if (!child->run(blackboard))  						 // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
			return false;
	}
	return true;  											 // All children suceeded, so the entire run() operation succeeds.
}