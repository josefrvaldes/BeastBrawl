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
 * @author Antonio Jose Martinez Garcia
 * 
 */
 
 
 #include "CLGrassSystem.h"

#include <chrono>
#include "../ResourceManager/CLResourceManager.h"
#include "../../../src/Systems/Utils.h"
#include "../../../src/Constants.h"


using namespace CLE;



CLGrassSystem::CLGrassSystem(float _width, float _height, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass) : width(_width), height(_height), position(_position), scale(_scale){
    auto rm = CLResourceManager::GetResourceManager();
    grassTexture = rm->GetResourceTexture(fileGrass, false);

    if(realistGrass){
        extraScaleRealistic = scale.x - scale.x*0.20;        // cual va a ser la cadencia de la hierva
        sizeGrass = (scale.x + extraScaleRealistic) * 0.5;  // se elige el punto intermedio
    }else{
        sizeGrass = scale.x * 0.3;
    }
    extraSize = sizeGrass + sizeGrass*0.2;


    CalculateNumBushes();

    if(realistGrass)
        CreateRealistGrass();
    else
        CreateGrass();

    ConfigureBuffers();
    // cout << "Num Text plantas: " << modelLeafVector.size() << "\n";
}


float CLGrassSystem::GetSize(){
    if(width > height)
        return width;
    else
        return height;
}



void CLGrassSystem::CalculateNumBushes(){
    numBushesRows = int(width / (extraSize));
    numBushesFiles = int(height / (extraSize));
}


// Crea las posiciones de la hierva de forma uniforme con un cuadrado, con el mismo tamanyo
void CLGrassSystem::CreateGrass(){ 
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
    srand(chrono::high_resolution_clock::now().time_since_epoch().count());

    while(posActual.x < (position.x+numBushesRows*0.5*extraSize)){
        while(posActual.z < (position.z+numBushesFiles*0.5*extraSize)){
            auxScale = scale;
            // Obtener numeros aleatorios
            extraScale = Utils::getRandomFloat(0, extraScaleRealistic);
            extraPosX = Utils::getRandomFloat(0, extraPositionRealistic);
            extraPosZ = Utils::getRandomFloat(0, extraPositionRealistic);
            auxScale += extraScale;
            auxPosition.x = posActual.x + extraPosX;
            auxPosition.z = posActual.z + extraPosZ;
            // generar las hojas
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

    glm::mat4 auxRot = glm::mat4(1.0f);
    auxRot = glm::rotate(auxRot, glm::radians(0.f) ,     glm::vec3(1,0,0));
    auxRot = glm::rotate(auxRot, glm::radians(0.f) , glm::vec3(0,1,0));
    auxRot = glm::rotate(auxRot, glm::radians(0.f) ,     glm::vec3(0,0,1));

    glm::mat4 model = auxTrans * auxRot * auxScale;
    modelLeafVector.emplace_back(model);
    posLeafVector.emplace_back(posLeaf);
}



void CLGrassSystem::ConfigureBuffers(){
    glm::mat4 modelMatrices[modelLeafVector.size()];
    std::copy(modelLeafVector.begin(), modelLeafVector.end(), modelMatrices);
    glm::vec3 posMatrices[posLeafVector.size()];
    std::copy(posLeafVector.begin(), posLeafVector.end(), posMatrices);

    // buffer de las matrices modelo
    glGenBuffers(1, &bufferModelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bufferModelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelLeafVector.size(), &modelMatrices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // buffer de las posiciones
    glGenBuffers(1, &bufferPosVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bufferPosVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * posLeafVector.size(), &posMatrices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO del plano
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);

    // se instancia el array de posiciones (vec3)
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, bufferPosVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(0, 1);

    // se instancia en 4 elementos la mat4 ya que lo maximo que admite el vertex es un vec3 
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, bufferModelVBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribDivisor(1, 1);    // el uno se pone para decirle a openGL que en vez de actualizar el valor cada vertice lo haga cada instancia(objeto)
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

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
    glUniform1f(glGetUniformLocation(shaderID, "timeElapsedWing"), wingTimer);

    glDrawArraysInstanced(GL_POINTS, 0, 1, modelLeafVector.size()); 
    glBindVertexArray(0);

    wingTimer += 0.6 * Constants::DELTA_TIME;
    if(wingTimer >= 360)
        wingTimer = 0;
}





// en caso de circulos
CLGrassSystem::CLGrassSystem(float radious, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass) : width(radious), height(radious), position(_position), scale(_scale){
    auto rm = CLResourceManager::GetResourceManager();
    grassTexture = rm->GetResourceTexture(fileGrass, false);

    if(realistGrass){
        extraScaleRealistic = scale.x - scale.x*0.20;        // cual va a ser la cadencia de la hierva
        sizeGrass = (scale.x + extraScaleRealistic) * 0.5;  // se elige el punto intermedio
    }else{
        sizeGrass = scale.x * 0.3;
    }
    extraSize = sizeGrass + sizeGrass*0.2;


    CalculateNumBushes();

    if(realistGrass)
        CreateRealistGrassCircle();
    else
        CreateGrassCircle();

    ConfigureBuffers();
    // cout << "Num Text plantas: " << modelLeafVector.size() << "\n";
}


// Crea las posiciones de la hierva de forma uniforme con un CIRCULO, con el mismo tamanyo
void CLGrassSystem::CreateGrassCircle(){ 
    glm::vec3 posActual = glm::vec3(position.x - numBushesRows*0.5*extraSize , position.y, position.z - numBushesFiles*0.5*extraSize);
    while(posActual.x < (position.x+numBushesRows*0.5*extraSize)){
        while(posActual.z < (position.z+numBushesFiles*0.5*extraSize)){
            if(glm::distance(posActual, position) < width*0.5)
                AddLeafs(posActual, scale);
            posActual.z = posActual.z + extraSize;
        }
        posActual.z = position.z - numBushesFiles*0.5*extraSize;
        posActual.x = posActual.x + extraSize; 
    }
}


// Trata de crear hierva realista con aleatorios CIRCULO
void CLGrassSystem::CreateRealistGrassCircle(){
    float extraScale;
    float extraPosX;
    float extraPosZ;
    glm::vec3 posActual = glm::vec3(position.x - numBushesRows*0.5*extraSize , position.y, position.z - numBushesFiles*0.5*extraSize);
    glm::vec3 auxScale(scale);
    glm::vec3 auxPosition(posActual);
    srand(chrono::high_resolution_clock::now().time_since_epoch().count());

    while(posActual.x < (position.x+numBushesRows*0.5*extraSize)){
        while(posActual.z < (position.z+numBushesFiles*0.5*extraSize)){
            auxScale = scale;
            // Obtener numeros aleatorios
            extraScale = Utils::getRandomFloat(0, extraScaleRealistic);
            extraPosX = Utils::getRandomFloat(0, extraPositionRealistic);
            extraPosZ = Utils::getRandomFloat(0, extraPositionRealistic);
            auxScale += extraScale;
            auxPosition.x = posActual.x + extraPosX;
            auxPosition.z = posActual.z + extraPosZ;
            // generar las hojas
            if(glm::distance(auxPosition, position) < width*0.5)
                AddLeafs(auxPosition, auxScale);
            posActual.z = posActual.z + extraSize;
        }
        posActual.z = position.z - numBushesFiles*0.5*extraSize;
        posActual.x = posActual.x + extraSize;
    }
}


/*
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
        posLeafVector.emplace_back(posLeaf);
        degrees+=60.0;
    }
}



void CLGrassSystem::ConfigureBuffers(){
    glm::mat4 modelMatrices[modelLeafVector.size()];
    std::copy(modelLeafVector.begin(), modelLeafVector.end(), modelMatrices);
    glm::vec3 posMatrices[posLeafVector.size()];
    std::copy(posLeafVector.begin(), posLeafVector.end(), posMatrices);
    //std::unique_ptr<glm::mat4> modelMatrices = make_unique<glm::mat4>(modelLeafVector.data());

    // buffer de las matrices modelo
    glGenBuffers(1, &bufferModelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bufferModelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelLeafVector.size(), &modelMatrices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // buffer de las posiciones
    glGenBuffers(1, &bufferPosVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bufferPosVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * posLeafVector.size(), &posMatrices[0], GL_STATIC_DRAW);
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // se instancia el array de posiciones (vec3)
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, bufferPosVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    // se instancia en 4 elementos la mat4 ya que lo maximo que admite el vertex es un vec3 
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, bufferModelVBO);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribDivisor(3, 1);    // el uno se pone para decirle a openGL que en vez de actualizar el valor cada vertice lo haga cada instancia(objeto)
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

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
}*/

