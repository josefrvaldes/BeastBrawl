#include "CLMesh.h"

using namespace CLE;

CLMesh::CLMesh(){
    
}

void CLMesh::Draw(glm::mat4& m) {
    if (mesh) {
        mesh->Draw(m);
    }

    
}