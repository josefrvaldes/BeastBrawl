#include "CLGrassManager.h"
#include "../ResourceManager/CLResourceManager.h"


using namespace CLE;




CLGrassManager::CLGrassManager(float _width, float _height, const glm::vec3& _position, const glm::vec3& _scale) : width(_width), height(_height), position(_position), scale(_scale){
    auto rm = CLResourceManager::GetResourceManager();
    grassTexture = rm->GetResourceTexture(fileGrass, false);

    sizeGrass = scale.x * 0.3;
    extraSize = sizeGrass + sizeGrass*0.2;


    CalculateNumBushes();
    CreateGrass();
}



void CLGrassManager::CalculateNumBushes(){
    numBushesRows = int(width / (extraSize));
    numBushesFiles = int(height / (extraSize));
}


// crea las plantas en un rectangulo
void CLGrassManager::CreateGrass(){ 
    //auto grassAux = make_unique<CLGrass>(glm::vec3(50.0f,50.0f,50.0f));
    glm::vec3 posActual = glm::vec3(position.x - numBushesRows*0.5*extraSize , position.y, position.z - numBushesFiles*0.5*extraSize);
    while(posActual.x < (position.x+numBushesRows*0.5*extraSize)){
        while(posActual.z < (position.z+numBushesFiles*0.5*extraSize)){
            bushes.emplace_back(std::make_unique<CLGrass>(posActual));
            posActual.z = posActual.z + extraSize;
        }
        posActual.z = position.z - numBushesFiles*0.5*extraSize;
        posActual.x = posActual.x + extraSize;
    }
    cout << bushes.size() << " -> tamanyo\n";
}


void CLGrassManager::Draw(GLuint shaderID, const glm::mat4& projection, const glm::mat4& view){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderID);
    glUniform1i(glGetUniformLocation(shaderID, "texture1"), 0);

    glm::mat4 auxScale = glm::mat4(1.0f);
    auxScale = glm::scale(auxScale, scale);
    glm::mat4 VPmatrix = projection * view;

    // for recorriendo todas
    for(const auto& actualArass : bushes){
        actualArass->Draw(shaderID, VPmatrix, auxScale);
    }
}