#include "CLMesh.h"

using namespace CLE;

void CLMesh::Draw(glm::mat4& m) {
    if (mesh) {
        mesh->Draw(m);
    }
}