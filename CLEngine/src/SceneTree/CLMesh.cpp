#include "CLMesh.h"

using namespace CLE;

CLMesh::CLMesh() {
}

void CLMesh::Draw(GLuint shaderID) {
    if (material) {
        material->Draw(shaderID);
    }
    if (mesh) {
        mesh->Draw(shaderID);
    }
}

void CLMesh::Animate() {
    
    // si tenemos que cambiar ya de keyFrame
    if (currentDistance == distanceBetweenKeyFrames[currentKeyFrameIndex]) {
        currentKeyFrameIndex++;
        if(currentKeyFrameIndex > keyframes.size() - 1) 
            currentKeyFrameIndex = 0;
        
        uint8_t nextKeyframeIndex = currentKeyFrameIndex + 1;
        if(nextKeyframeIndex > keyframes.size() - 1) 
            nextKeyframeIndex = 0;
        mesh = keyframes[currentKeyFrameIndex];
        nextMesh = keyframes[nextKeyframeIndex];
        currentDistance = 0;
    }

    float percentTick = std::min(1.0f, (static_cast<float>(currentDistance) / distanceBetweenKeyFrames[currentKeyFrameIndex]));

    // de cada keyframe, recorremos sus mallas
    for (size_t idxMesh = 0; idxMesh < mesh->GetvectorMesh().size(); idxMesh++) {
        auto& prevSubMesh = mesh->GetvectorMeshPtr()->at(idxMesh);
        auto& nextSubMesh = nextMesh->GetvectorMeshPtr()->at(idxMesh);
        //auto& currMesh = resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh);
        // cout << "Las mallas " << idxMesh << " tienen " << prevMesh.vertices.size() << ", " << currMesh.vertices.size() << " y " << nextMesh.vertices.size() << " vértices respectivamente" << endl;
        for (size_t idxVertex = 0; idxVertex < prevSubMesh.vertices.size(); idxVertex++) {
            auto& prevVertex = prevSubMesh.vertices.at(idxVertex);
            auto& nextVertex = nextSubMesh.vertices.at(idxVertex);
            //auto& currVertex = currMesh.vertices[idxVertex];

            glm::vec3 position = mix(prevVertex.position, nextVertex.position, percentTick);
            glm::vec3 normal = mix(prevVertex.normal, nextVertex.normal, percentTick);
            prevVertex.animationOffsetPos = position - prevVertex.position;
            prevVertex.animationOffsetNormal = normal - prevVertex.normal;

            // cout << "Tenemos un vértice en " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl;
            // cout << "El next un vértice es " << nextVertex.position.x << "," << nextVertex.position.y << "," << nextVertex.position.z << endl;
            if (idxMesh == 0 && idxVertex == 0) {
                // cout << "El timeElapsed es " << timeElapsed << " y el percentTick es " << percentTick << ". La nueva pos es " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl;
                // cout << "La pos variable local es " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl
                //     << "La pos variable miembro es " << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.x
                //     << "," << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.y
                //     << "," << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.z << endl << endl;
            }
        }
    }
    currentDistance++;
}

void CLMesh::DrawDepthMap(GLuint shaderID) {
    /*if(material){
        material->Draw(shaderID); 
    }*/
    if (mesh) {
        mesh->DrawDepthMap(shaderID);
    }
}