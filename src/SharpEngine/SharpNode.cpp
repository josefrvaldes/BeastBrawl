//Made with FMOD Studio by Firelight Technologies Pty Ltd.

#include "SharpNode.h"

using namespace CLSE;


SharpNode::SharpNode(uint16_t idEnt, glm::vec3& p, bool b) {
    idEntity = idEnt;
    pos = p;
    clean = b;
}