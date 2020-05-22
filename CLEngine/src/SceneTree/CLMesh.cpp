/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * @author Rubén Rubio Martínez
 * @author Jose Valdés Sirvent
 * 
 */
 
 
 #include "CLMesh.h"
#include "../../../src/Systems/Utils.h"

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

bool CLMesh::ItsTimeToAnimate() {
    int64_t now = Utils::getMillisSinceEpoch();
    int64_t interval = now - lastTimeFrameChanged;
    if(interval > DELTA_ANIMATION) {
        lastTimeFrameChanged = now;
        // cambiamos de frame
        return true;
    }
    return false;
}

void CLMesh::GoToNextKeyFrames() {
    // if(loop || (!loop && currentKeyFrameIndex < keyframes.size()) - 2)
    if(currentAnimation->loop || currentKeyFrameIndex < currentAnimation->keyframes.size() - 2)
        currentKeyFrameIndex++;

    if (currentKeyFrameIndex > currentAnimation->keyframes.size() - 1)
        currentKeyFrameIndex = 0;

    nextKeyFrameIndex = currentKeyFrameIndex + 1;
    if (nextKeyFrameIndex > currentAnimation->keyframes.size() - 1)
        nextKeyFrameIndex = 0;

    mesh = currentAnimation->keyframes[currentKeyFrameIndex];
    nextMesh = currentAnimation->keyframes[nextKeyFrameIndex];
}

void CLMesh::Animate() {
    if(ItsTimeToAnimate()) {
        // cambiamos de frame
        GoToNextKeyFrames();
    }
}

void CLMesh::ResetAnimation() {
    currentKeyFrameIndex = 0;
}

void CLMesh::AnimateInterpolated() {
    if(ItsTimeToAnimate()) {
            // si tenemos que cambiar ya de keyFrame...
        if (currentDistance == distanceBetweenKeyFrames[currentKeyFrameIndex]) {
            GoToNextKeyFrames();
            currentDistance = 0;
        }

        float percentTick = std::min(1.0f, (static_cast<float>(currentDistance) / distanceBetweenKeyFrames[currentKeyFrameIndex]));

        // de cada keyframe, recorremos sus mallas
        for (size_t idxMesh = 0; idxMesh < mesh->GetvectorMesh().size(); idxMesh++) {
            auto& prevSubMesh = mesh->GetvectorMeshPtr()->at(idxMesh);
            auto& nextSubMesh = nextMesh->GetvectorMeshPtr()->at(idxMesh);
            for (size_t idxVertex = 0; idxVertex < prevSubMesh.vertices.size(); idxVertex++) {
                auto& prevVertex = prevSubMesh.vertices.at(idxVertex);
                auto& nextVertex = nextSubMesh.vertices.at(idxVertex);

                glm::vec3 position = mix(prevVertex.position, nextVertex.position, percentTick);
                glm::vec3 normal = mix(prevVertex.normal, nextVertex.normal, percentTick);
                prevVertex.animationOffsetPos = position - prevVertex.position;
                prevVertex.animationOffsetNormal = normal - prevVertex.normal;

                // los comentarios de abajo son puramente debug, podrían borrarse
                // cout << "Tenemos un vértice en " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl;
                // cout << "El next un vértice es " << nextVertex.position.x << "," << nextVertex.position.y << "," << nextVertex.position.z << endl;
                // if (idxMesh == 0 && idxVertex == 0) {
                // cout << "El timeElapsed es " << timeElapsed << " y el percentTick es " << percentTick << ". La nueva pos es " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl;
                // cout << "La pos variable local es " << currVertex.position.x << "," << currVertex.position.y << "," << currVertex.position.z << endl
                //     << "La pos variable miembro es " << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.x
                //     << "," << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.y
                //     << "," << resourceMeshCubeAnim3->GetvectorMeshPtr()->at(idxMesh).vertices.at(idxVertex).position.z << endl << endl;
                // }
            }
        }
        currentDistance++;
        // cout << "currentKeyFrameIndex -> " << unsigned(currentKeyFrameIndex) << ", nextKeyFrameIndex -> " << unsigned(nextKeyFrameIndex) << endl;
        // cout << "los archivos cargados son " << mesh->GetName() << " y " << nextMesh->GetName()  << " y la distancia es " << unsigned(distanceBetweenKeyFrames[currentKeyFrameIndex]) << endl;
    }
}

void CLMesh::DrawDepthMap(GLuint shaderID) {
    /*if(material){
        material->Draw(shaderID); 
    }*/
    if (mesh) {
        mesh->DrawDepthMap(shaderID);
    }
}