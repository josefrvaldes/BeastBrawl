#include "CLNode.h" 

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace CLE;

CLNode::CLNode(){
    translation = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scalation = glm::vec3(1.0f, 1.0f, 1.0f);
    transformationMat = glm::mat4(1.0f);
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

CLNode* CLNode::AddMesh(unsigned int id){

    shared_ptr<CLEntity> e = make_shared<CLMesh>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    childs.push_back(node);
    node->SetFather(this);

    return node.get();
    
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


bool CLNode::RemoveChild(CLNode* child){

    for(unsigned int i = 0; i<childs.size(); ++i){
        if(child == childs[i].get()){
            childs.erase(childs.begin()+i);
            return true;
        }
    }
    return false;
}

void CLNode::DeleteNode(unsigned int id){
    CLNode* node = nullptr;
    node = GetNodeByIDAux(id, node, this);
    auto father = node->GetFather();
    father->RemoveChild(node);
}

void CLNode::DeleteNode(CLNode* node){
    auto father = node->GetFather();
    father->RemoveChild(node);
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
    translation = t; 
    ActivateFlag();
}

void CLNode::SetRotation(glm::vec3 r) {
    rotation = r; 
    ActivateFlag();
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
    aux = glm::rotate(aux, glm::radians(rotation.x) , glm::vec3(1,0,0));
    aux = glm::rotate(aux, glm::radians(rotation.y) , glm::vec3(0,1,0));
    aux = glm::rotate(aux, glm::radians(rotation.z) , glm::vec3(0,0,1));
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

    if(entity && visible) { 
        // La matriz model se pasa aqui wey
        glUseProgram(shaderProgramID);
        glm::mat4 MVP = projection * view * transformationMat;
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformationMat));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
        //entity->Draw(transformationMat);
        entity->Draw(shaderProgramID);
    }

    for (auto node : childs) {
        node->DFSTree(transformationMat);
    }
}


/**
 * Calcula la matriz view y projection
 * TODO: Aun no se sabe seguro si se debe hacer asi
 * 
 */
void CLNode::CalculateViewProjMatrix(){
    for(auto camera : cameras){
        auto entityCamera = static_cast<CLCamera*>(camera->GetEntity());
        if(entityCamera->IsActive()){
            glUseProgram(camera->GetShaderProgramID());

            projection    = entityCamera->CalculateProjectionMatrix();
            //glUniformMatrix4fv(glGetUniformLocation(camera->GetShaderProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Vector posicion de la camara, vector de posicion destino y vector ascendente en el espacio mundial. 
            

            //view = glm::lookAt(camera->GetTranslation(), -entityCamera->GetCameraTarget(), entityCamera->GetCameraUp());
            view = glm::lookAt(camera->GetGlobalTranslation(), entityCamera->GetCameraTarget(), entityCamera->GetCameraUp());

            glUniform3fv(glGetUniformLocation(camera->GetShaderProgramID(), "viewPos"),1,glm::value_ptr(camera->GetTranslation()));

            //glUniformMatrix4fv(glGetUniformLocation(camera->GetShaderProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        }
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



/**
 * Metodo de debug para imprimir los nodos del arbol
 */
void CLNode::DrawTree(CLNode* root){
    if(root->GetChilds().size()>0){
        //Tiene hijos
        if( root->GetEntity() && !root->GetEntity()->GetID())
            cout << root->GetEntity()->GetID() << " con hijos: ";
        else
            cout << "Este es un nodo sin entity con hijos: ";

        for(auto& nodo : root->GetChilds()){
            if(nodo->GetEntity() && nodo->GetEntity()->GetID())
                cout << nodo->GetEntity()->GetID() << " ";
            else
                cout << "(Este es un nodo sin entity)\n";
        }
        cout << "\n";
        for(auto& nodo : root->GetChilds()){
            DrawTree(nodo.get());
        }
    }

    return;
}