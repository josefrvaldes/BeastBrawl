#include "CLGrassSystem.h"
#include "../ResourceManager/CLResourceManager.h"
#include <cstdlib>
#include <ctime>


using namespace CLE;




CLGrassSystem::CLGrassSystem(float _width, float _height, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass) : width(_width), height(_height), position(_position), scale(_scale){
    auto rm = CLResourceManager::GetResourceManager();
    grassTexture = rm->GetResourceTexture(fileGrass, false);

    if(realistGrass)
        sizeGrass = (scale.x + extraScaleRealistic) * 0.5;
    else
        sizeGrass = scale.x * 0.3;
    extraSize = sizeGrass + sizeGrass*0.2;


    CalculateNumBushes();

    if(realistGrass)
        CreateRealistGrass();
    else
        CreateGrass();

    ConfigureBuffers();
    cout << "Num Text plantas: " << modelLeafVector.size() << "\n";
}



void CLGrassSystem::CalculateNumBushes(){
    numBushesRows = int(width / (extraSize));
    numBushesFiles = int(height / (extraSize));
}


// Crea las posiciones de la hierva de forma uniforme con un cuadrado, con el mismo tamanyo
void CLGrassSystem::CreateGrass(){ 
    //auto grassAux = make_unique<CLGrass>(glm::vec3(50.0f,50.0f,50.0f));
    glm::vec3 posActual = glm::vec3(position.x - numBushesRows*0.5*extraSize , position.y, position.z - numBushesFiles*0.5*extraSize);
    while(posActual.x < (position.x+numBushesRows*0.5*extraSize)){
        while(posActual.z < (position.z+numBushesFiles*0.5*extraSize)){
            AddLeafs(posActual, scale);
            posActual.z = posActual.z + extraSize;
        }
        posActual.z = position.z - numBushesFiles*0.5*extraSize;
        posActual.x = posActual.x + extraSize;
    }
}



// Trata de crear hierva realista con aleatorios
void CLGrassSystem::CreateRealistGrass(){
    float extraScale;
    float extraPosX;
    float extraPosZ;
    glm::vec3 posActual = glm::vec3(position.x - numBushesRows*0.5*extraSize , position.y, position.z - numBushesFiles*0.5*extraSize);
    glm::vec3 auxScale(scale);
    glm::vec3 auxPosition(posActual);
    std::srand(std::time(NULL));
    while(posActual.x < (position.x+numBushesRows*0.5*extraSize)){
        while(posActual.z < (position.z+numBushesFiles*0.5*extraSize)){
            auxScale = scale;
            extraScale = float(std::rand() % extraScaleRealistic);
            extraPosX =  float(std::rand() % extraPositionRealistic);
            extraPosZ =  float(std::rand() % extraPositionRealistic);
            cout << extraScale << "  --  " << extraPosX << "  --  " << extraPosZ << "\n";
            auxScale += extraScale;
            auxPosition.x = posActual.x + extraPosX;
            auxPosition.z = posActual.z + extraPosZ;
            AddLeafs(auxPosition, auxScale);
            posActual.z = posActual.z + extraSize;
        }
        posActual.z = position.z - numBushesFiles*0.5*extraSize;
        posActual.x = posActual.x + extraSize;
    }
}



// Anyade los tres planos de un arbusto al vector, calculamos la matriz modelo
void CLGrassSystem::AddLeafs(const glm::vec3& posLeaf, const glm::vec3& scaleLeaf){
    glm::mat4 auxTrans = glm::mat4(1.0f);
    auxTrans = glm::translate(auxTrans, posLeaf);

    glm::mat4 auxScale = glm::mat4(1.0f);
    auxScale = glm::scale(auxScale, scaleLeaf);

    float degrees = 0.0f;
    for(unsigned int i = 0; i<3; i++){
        glm::mat4 auxRot = glm::mat4(1.0f);
        auxRot = glm::rotate(auxRot, glm::radians(0.f) ,     glm::vec3(1,0,0));
        auxRot = glm::rotate(auxRot, glm::radians(degrees) , glm::vec3(0,1,0));
        auxRot = glm::rotate(auxRot, glm::radians(0.f) ,     glm::vec3(0,0,1));

        glm::mat4 model = auxTrans * auxRot * auxScale;
        modelLeafVector.emplace_back(model);
        degrees+=60.0;
    }
}



void CLGrassSystem::ConfigureBuffers(){
    glm::mat4 modelMatrices[modelLeafVector.size()];
    std::copy(modelLeafVector.begin(), modelLeafVector.end(), modelMatrices);
    //std::unique_ptr<glm::mat4> modelMatrices = make_unique<glm::mat4>(modelLeafVector.data());

    // buffer de las matrices modelo
    glGenBuffers(1, &bufferVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bufferVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelLeafVector.size(), &modelMatrices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO del plano
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // se instancia en 4 elementos la mat4 ya que lo maximo que admite el vertex es un vec3 
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, bufferVBO);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(2, 1);    // el uno se pone para decirle a openGL que en vez de actualizar el valor cada vertice lo haga cada instancia(objeto)
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glBindVertexArray(0);
}



void CLGrassSystem::Draw(GLuint shaderID, const glm::mat4& projection, const glm::mat4& view){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderID);

    glBindVertexArray(planeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexture->GetTextureID());

    glm::mat4 VPmatrix = projection * view;
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "VPmatrix"), 1, GL_FALSE, glm::value_ptr(VPmatrix));

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, modelLeafVector.size()); 
    glBindVertexArray(0);
}

// crea las plantas en un rectangulo
/*void CLGrassSystem::CreateGrass(){ 
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
}*/



/*void CLGrassSystem::Draw(GLuint shaderID, const glm::mat4& projection, const glm::mat4& view){
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
}*/