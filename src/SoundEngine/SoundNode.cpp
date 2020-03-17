#include "SoundNode.h"

using namespace CLSE;


SoundNode::SoundNode(uint16_t idEnt, glm::vec3& p, bool b) {
    idEntity = idEnt;
    pos = p;
    clean = b;
}