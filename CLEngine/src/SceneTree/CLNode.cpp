#include "CLNode.h" 

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../Frustum/CLFrustum.h"

using namespace CLE;

CLNode::CLNode(){
    translation = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scalation = glm::vec3(1.0f, 1.0f, 1.0f);
    transformationMat = glm::mat4(1.0f);

    //Inicializamos el shader de debug
    if(!debugShader){
        auto resourceDebugShader = CLResourceManager::GetResourceManager()->GetResourceShader("CLEngine/src/Shaders/debugShader.vert", "CLEngine/src/Shaders/debugShader.frag");
        debugShader = resourceDebugShader->GetProgramID();
    }
    
}

CLNode::CLNode(shared_ptr<CLEntity> entity) : CLNode() {
    this->entity = entity;
}


CLNode* CLNode::AddGroup(unsigned int id){
    shared_ptr<CLNode> node = make_shared<CLNode>();
    childs.push_back(node);
    node->SetFather(this);

    return node.get();
}

CLNode* CLNode::AddMesh(unsigned int id,string mesh){
    auto node = AddMesh(id);
    auto resourceMesh = CLResourceManager::GetResourceManager()->GetResourceMesh(mesh);
    static_cast<CLMesh*>(node->GetEntity())->SetMesh(resourceMesh);

    return node;
}

CLNode* CLNode::AddMesh(unsigned int id){

    shared_ptr<CLEntity> e = make_shared<CLMesh>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    childs.push_back(node);
    node->SetFather(this);


    return node.get();
}

CLNode* CLNode::AddLight(unsigned int id,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
    auto node = AddLight(id);
    static_cast<CLLight*>(node->GetEntity())->SetLightAttributes(intensity,ambient,diffuse,specular,constant,linear,quadratic);
    return node;
}


CLNode* CLNode::AddLight(unsigned int id){

    shared_ptr<CLEntity> e = make_shared<CLLight>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    childs.push_back(node);
    node->SetFather(this);
    lights.push_back(node.get());

    return node.get();
    
}

CLNode* CLNode::AddCamera(unsigned int id){

    shared_ptr<CLEntity> e = make_shared<CLCamera>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    childs.push_back(node);
    node->SetFather(this);
    cameras.push_back(node.get());

    return node.get();
    
}

void CLNode::AddSkybox(string right, string left, string top, string bottom, string front, string back){
    if(!skyboxShader){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/skybox.vert", "CLEngine/src/Shaders/skybox.frag");
        skyboxShader = resourceShader->GetProgramID();
        cout << skyboxShader << endl;
    }
    skybox = make_unique<CLSkybox>(right, left, top, bottom, front, back);
}


bool CLNode::RemoveChild(CLNode* child){

    if(child->GetChilds().size()>0){
        for(auto childOfChild : child->GetChilds()){
            
            child->DeleteNode(childOfChild->GetEntity()->GetID());
        }
    }

    //Childs son los hijos del padre en el que estara child
    for(unsigned int i = 0; i<childs.size(); ++i){
        if(child == childs[i].get()){
            childs.erase(childs.begin()+i);
            return true;
        }
    }
    return false;
}

bool CLNode::DeleteNode(unsigned int id){
    CLNode* node = nullptr;
    node = GetNodeByIDAux(id, node, this);
    if(!node) return false;
    auto father = node->GetFather();
    father->RemoveChild(node);
    return true;
}

bool CLNode::DeleteNode(CLNode* node){
    if(!node) return false;
    auto father = node->GetFather();
    father->RemoveChild(node);
    return true;
}

bool CLNode::HasChild(CLNode* child){

    for(auto& node : childs){
        if(node.get() == child){
            return true;
        }
    }

    return false;
}

glm::vec3 CLNode::GetGlobalTranslation() const{
    glm::vec3 dev = this->translation;
    auto fatherNode = this->father;
    while(fatherNode){
        dev += fatherNode->GetTranslation();
        fatherNode = fatherNode->GetFather();
    }
    return dev;
}

glm::vec3 CLNode::GetGlobalRotation() const{
    glm::vec3 dev = this->rotation;
    auto fatherNode = this->father;
    while(fatherNode){
        dev += fatherNode->GetRotation();
        fatherNode = fatherNode->GetFather();
    }
    return dev;
}

glm::vec3 CLNode::GetGlobalScalation() const{
    glm::vec3 dev = this->scalation;
    auto fatherNode = this->father;
    while(fatherNode){
        dev += fatherNode->GetScalation();
        fatherNode = fatherNode->GetFather();
    }
    return dev;
}

void CLNode::SetTranslation(glm::vec3 t) {
    // BOUNDING BOX
    //auto diference = t-translation;
    //extremeMinMesh += diference;
    //extremeMaxMesh += diference;
    // TRANSLATION
    translation = t;

    ActivateFlag();
}

void CLNode::SetRotation(glm::vec3 r) {
    // BPUNDONG BOX
    //if(rotation != r){
    //    extremeMinMesh = RotatePointAroundCenter(extremeMinMesh, translation, r);
    //    extremeMaxMesh = RotatePointAroundCenter(extremeMaxMesh, translation, r);
    //}
    // ROTATION
    rotation = r; 
    ActivateFlag();
}

glm::vec3 CLNode::RotatePointAroundCenter(const glm::vec3& point_ , const glm::vec3& center, const glm::vec3& rot) const{
    glm::vec3 newPoint = glm::vec3(0.0,0.0,0.0);
    // rotation Y
    if(rotation.y != rot.y){
        newPoint.x += ((point_.x - center.x) * cos(rot.y-rotation.y))  + center.x;
        newPoint.z += ((point_.z - center.z) * sin(rot.y-rotation.y))  + center.z;
    }


//x2 = x1 * cos(angulo) - y1 * sin(angulo)
//y2 = x1 * sin(angulo) - y1 * cos(angulo)


    // rotation X
    //if(rotation.x != rot.x){
    //    newPoint.y += ((point_.y - center.y) * cos(rot.x-rotation.x)) - ((center.z - point_.z) * sin(rot.x-rotation.x)) + center.y;
    //    newPoint.z += ((center.z - point_.z) * cos(rot.x-rotation.x)) - ((point_.y - center.y) * sin(rot.x-rotation.x)) + center.z;
    //}
    //// rotation Z
    //if(rotation.z != rot.z){
    //    newPoint.y += ((point_.y - center.y) * cos(rot.z-rotation.z)) - ((center.x - point_.x) * sin(rot.z-rotation.z)) + center.y;
    //    newPoint.x += ((center.x - point_.x) * cos(rot.z-rotation.z)) - ((point_.y - center.y) * sin(rot.z-rotation.z)) + center.x;
    //}

    return newPoint;
}

void CLNode::SetScalation(glm::vec3 s) {
    scalation = s; 
    ActivateFlag();
}

void CLNode::ActivateFlag() {
    changed = true;
    for (auto node : childs) {
        node->ActivateFlag();
    }
    return;
}

glm::mat4 CLNode::TranslateMatrix(){
    glm::mat4 aux = glm::mat4(1.0f);
    aux = glm::translate(aux, translation);
    return aux;
}

glm::mat4 CLNode::RotateMatrix(){
    glm::mat4 aux = glm::mat4(1.0f);
    aux = glm::rotate(aux, glm::radians(rotation.y) , glm::vec3(0,1,0));
    aux = glm::rotate(aux, glm::radians(rotation.z) , glm::vec3(0,0,1));
    aux = glm::rotate(aux, glm::radians(rotation.x) , glm::vec3(1,0,0));
    return aux;
}

glm::mat4 CLNode::ScaleMatrix(){
    glm::mat4 aux = glm::mat4(1.0f);
    aux = glm::scale(aux, scalation);
    return aux;
}

glm::mat4 CLNode::CalculateTransformationMatrix() {
    return TranslateMatrix()*RotateMatrix()*ScaleMatrix();
}

void CLNode::Translate(glm::vec3 t) {
    translation = t;
}

void CLNode::Rotate(glm::vec3 r) {
    rotation = r;
}

void CLNode::Scale(glm::vec3 s) {
    scalation = s;
}



void CLNode::DFSTree(glm::mat4 mA) {
    // > Flag
    // > > Calcular matriz
    // > Dibujar
    // > Para cada hijo
    // > > DFSTree(mT)

    if (changed) {
        transformationMat = mA*CalculateTransformationMatrix();
        changed = false;
    }

    auto& frustum_m = GetActiveCamera()->GetFrustum();
    //CLE::CLFrustum::Visibility frusVisibility = frustum_m.IsInside(translation);
    CLE::CLFrustum::Visibility frusVisibility = frustum_m.IsInside(translation, dimensionsBoundingBox);


    if(entity && visible && frusVisibility == CLE::CLFrustum::Visibility::Completly) { 
        glUseProgram(shaderProgramID);
        //Calculamos las luces
        //TODO: Hacer un sistema de que si no hemos cambiado de shader no se recalculen

        if(hasLightingEffects){
            CalculateLights();

        }

        glm::mat4 MVP = projection * view * transformationMat;
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformationMat));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
        entity->Draw(shaderProgramID);

    }

    for (auto node : childs) {
        node->DFSTree(transformationMat);
    }
}


/**
 * Calcula la matriz view y projection
 * TODO: Aun no se sabe seguro si se debe hacer asi
 */
void CLNode::CalculateViewProjMatrix(){
    for(auto camera : cameras){
        auto entityCamera = static_cast<CLCamera*>(camera->GetEntity());
        if(entityCamera->IsActive()){
            glUseProgram(camera->GetShaderProgramID());

            projection    = entityCamera->CalculateProjectionMatrix();

            // Vector posicion de la camara, vector de posicion destino y vector ascendente en el espacio mundial. 
            

            view = glm::lookAt(camera->GetGlobalTranslation(), entityCamera->GetCameraTarget(), entityCamera->GetCameraUp());


            glUniform3fv(glGetUniformLocation(camera->GetShaderProgramID(), "viewPos"),1,glm::value_ptr(camera->GetTranslation()));


            // Una vez tenemos la View y la Projection vamos a calcular Frustum para los objetos
            //auto& frustum_m = entityCamera->GetFrustum();
            //frustum->Transform(projection,modelView);

        }
    }
    auto modelView = transformationMat*view;
    GetActiveCamera()->CalculateFrustum(projection, modelView);
}


/**
 * Calcula la iluminación de la escena iterando por todas las luces
 */
void CLNode::CalculateLights(){
    GLuint i = 0;
    for(auto light : lights){
        auto lightEntity = static_cast<CLLight*>(light->GetEntity());
        
        string number = to_string(i);

        glUniform1i(glGetUniformLocation(this->GetShaderProgramID(),"num_Point_Lights"),lights.size());    
        //TODO: A ver esto deberia cambiarse y pasarselo al shader de las mallas que lo vayan a usar
        //      por si al final las luces usan otro shader
        glUniform3fv(glGetUniformLocation(this->GetShaderProgramID(), ("pointLights[" + number + "].position").c_str()),1,glm::value_ptr(light->GetGlobalTranslation()));
        glUniform3fv(glGetUniformLocation(this->GetShaderProgramID(), ("pointLights[" + number + "].ambient").c_str()), 1,glm::value_ptr(lightEntity->GetAmbient()));
        glUniform3fv(glGetUniformLocation(this->GetShaderProgramID(), ("pointLights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(lightEntity->GetDiffuse()));
        glUniform3fv(glGetUniformLocation(this->GetShaderProgramID(), ("pointLights[" + number + "].specular").c_str()), 1, glm::value_ptr(lightEntity->GetSpecular()));
        glUniform1f(glGetUniformLocation(this->GetShaderProgramID(), ("pointLights[" + number + "].constant").c_str()), lightEntity->GetConstant());
        glUniform1f(glGetUniformLocation(this->GetShaderProgramID(), ("pointLights[" + number + "].linear").c_str()), lightEntity->GetLinear());
        glUniform1f(glGetUniformLocation(this->GetShaderProgramID(), ("pointLights[" + number + "].quadratic").c_str()), lightEntity->GetQuadratic());


        i++;
    }
}

/**
 * Dibuja el skybox lo primero de todo 
 */
void CLNode::DrawSkybox(){
    if(skybox.get()){
        glDepthMask(GL_FALSE);
        glUseProgram(skyboxShader);

        glm::mat4 view2 = glm::mat4(glm::mat3(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(view2));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        skybox->Draw(skyboxShader);
    }
}

//Devuelve el nodo por la id que le mandes
//Lo hace a partir del padre que lo llame, lo suyo es llamarlo siempre con el nodo principal
CLNode* CLNode::GetNodeByID(unsigned int id){
    CLNode* node = nullptr;
    node = GetNodeByIDAux(id, node, this);
    return node;
}

CLNode* CLNode::GetNodeByIDAux(unsigned int id, CLNode* node, CLNode* root){
 
    if(node!=nullptr) return node; //Caso base, ha encontrado ya al nodo que busca
    if(root->GetChilds().size()>0){
        //Tiene hijos
        for(auto& nodo : root->GetChilds()){
            if(nodo->GetEntity() && nodo->GetEntity()->GetID() == id){
                node = nodo.get();
                return node;
            }else{
                node = GetNodeByIDAux(id, node, nodo.get());

            }
        }

    }

    return node;
}

CLCamera* CLNode::GetActiveCamera(){
    for(auto camera : cameras){
        auto entityCamera = static_cast<CLCamera*>(camera->GetEntity());
        if(entityCamera->IsActive()){
            return entityCamera;
        }
    }
    return nullptr;
}

const void CLNode::Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2) const{
    Draw3DLine(x1,y1,z1,x1,y2,z1,CLColor(255.0,0.0,0.0,255.0));
}

const void CLNode::Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2,CLColor color) const{

    float line[] = {
        x1, y1, z1,
        x1, y2, z2
    };

    // float line[] = {
    //     -0.6f,0.3f,0.0f,
    //     0.8f,0.5f,0.0f
    // };
 
    
    // glEnable(GL_LINE_SMOOTH);
    // glLineWidth(10);
    // glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), 0);
    glBindVertexArray(0);

    glm::mat4 modelMat = glm::identity<mat4>();

    //99% seguro de que estoy enviando mal las matrices vista y projeccion
    glUseProgram(debugShader);

    glm::vec4 clcolor(color.GetRedNormalized(),color.GetGreenNormalized(),color.GetBlueNormalized(),color.GetAlphaNormalized());
    glUniformMatrix4fv(glGetUniformLocation(debugShader, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(debugShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(debugShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4fv(glGetUniformLocation(debugShader, "clcolor"),1, glm::value_ptr(clcolor));
    glUniform1i(glGetUniformLocation(debugShader,"prueba"),25);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0,2); 
    glUseProgram(0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

}



/**
 * Metodo de debug para imprimir los nodos del arbol
 */
void CLNode::DrawTree(CLNode* root){
    if(root->GetChilds().size()>0){
        //Tiene hijos
        if( root->GetEntity() && root->GetEntity()->GetID())
            cout << root->GetEntity()->GetID() << " con hijos: ";
        else
            cout << "Este es un nodo sin entity con hijos: ";

        for(auto& nodo : root->GetChilds()){
            if(nodo->GetEntity() && nodo->GetEntity()->GetID())
                cout << nodo->GetEntity()->GetID() << " ";
            else
                cout << "(hijo sin ID) ";
        }
        cout << "\n";
        for(auto& nodo : root->GetChilds()){
            DrawTree(nodo.get());
        }
    }

    return;
}


float CLNode::CalculateBoundingBox(){
    glm::vec3 extremeMinMesh = glm::vec3(0.0,0.0,0.0); 
    glm::vec3 extremeMaxMesh = glm::vec3(0.0,0.0,0.0);
    //auto mesh_m = static_cast<CLMesh*>(e.get())->GetMesh();
    //auto vecMesh = mesh_m->GetvectorMesh();
    auto resource = static_cast<CLMesh*>(this->GetEntity())->GetMesh();
    auto vecMesh = static_cast<CLResourceMesh*>(resource)->GetvectorMesh();

    int i = 0;
    for(auto currentVecMesh = vecMesh.begin(); currentVecMesh != vecMesh.end(); ++currentVecMesh){
        auto vertexs = currentVecMesh->vertices;
        for(long unsigned int j=0; j< vertexs.size(); j++){
            if( i== 0 && j== 0){
                // es el primer vertice, por lo que sera tanto el mayor como el menor
                extremeMinMesh = vertexs[j].position;
                extremeMaxMesh = vertexs[j].position;
            }else{
                // comprobamos para X
                if(extremeMinMesh.x > vertexs[j].position.x ) extremeMinMesh.x = vertexs[j].position.x;
                if(extremeMaxMesh.x < vertexs[j].position.x ) extremeMaxMesh.x = vertexs[j].position.x;

                // comprobamos para Y
                if(extremeMinMesh.y > vertexs[j].position.y ) extremeMinMesh.y = vertexs[j].position.y;
                if(extremeMaxMesh.y < vertexs[j].position.y ) extremeMaxMesh.y = vertexs[j].position.y;

                // comprobamos para Z
                if(extremeMinMesh.z > vertexs[j].position.z ) extremeMinMesh.z = vertexs[j].position.z;
                if(extremeMaxMesh.z < vertexs[j].position.z ) extremeMaxMesh.z = vertexs[j].position.z;
            }
        }
        i++;
    }
    // una vez salimos, debemos transladarlo al lugar de creacion del objeto
    extremeMinMesh += this->GetTranslation();
    extremeMaxMesh += this->GetTranslation();

    dimensionsBoundingBox = glm::distance(extremeMaxMesh.x, extremeMinMesh.x);
    if(dimensionsBoundingBox < glm::distance(extremeMaxMesh.y, extremeMinMesh.y))
        dimensionsBoundingBox = glm::distance(extremeMaxMesh.y, extremeMinMesh.y);
    if(dimensionsBoundingBox < glm::distance(extremeMaxMesh.z, extremeMinMesh.z))
        dimensionsBoundingBox = glm::distance(extremeMaxMesh.z, extremeMinMesh.z);

    return dimensionsBoundingBox;

    //cout << "Los extremos son: " << endl;
    //cout << "minimo: ( " << extremeMinMesh.x<< " , " << extremeMinMesh.y<< " , " <<extremeMinMesh.z<< 
    //" ) , maximo: " << extremeMaxMesh.x<< " , " << extremeMaxMesh.y<< " , " << extremeMaxMesh.z<< " )"<< endl;
}