#include "CLMesh.h"

using namespace CLE;

CLMesh::CLMesh(){
    
}

void CLMesh::Draw(GLuint shaderID) {
    if(material){
        material->Draw(shaderID); 
    }
    if (mesh) {
        mesh->Draw(shaderID);
    }
    
}