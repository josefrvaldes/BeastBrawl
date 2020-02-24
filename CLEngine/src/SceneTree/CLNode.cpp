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

CLNode::CLNode(CLEntity* entity) : CLNode() {
    this->entity = entity;
}


bool CLNode::AddChild(CLNode* child){
    //Tipica comprobacion de Programacion 2 pero bueno por si acaso
    if(child!=nullptr){
        childs.push_back(child);
        child->SetFather(this);
        return true;
    }
    return false;
}

bool CLNode::RemoveChild(CLNode* child){

    for(unsigned int i = 0; i<childs.size(); ++i){
        if(child == childs[i]){
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
        if(node == child){
            return true;
        }
    }

    return false;
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
    return glm::translate(aux, translation);
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
    return glm::scale(aux, scalation);
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

void CLNode::DrawTree(CLNode* root){
    if(root->GetChilds().size()>0){
        //Tiene hijos
        cout << "Nodo " << root->GetEntity()->GetID() << " con hijos: ";
        for(auto& nodo : root->GetChilds()){
            cout << nodo->GetEntity()->GetID() << " ";
        }
        cout << "\n";
        for(auto& nodo : root->GetChilds()){
            DrawTree(nodo);
        }
    }

    return;
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

    if(entity) {
        // La matriz model se pasa aqui wey
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformationMat));
        entity->Draw(transformationMat);
    }

    for (auto node : childs) {
        node->DFSTree(transformationMat);
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
            if(nodo->GetEntity()->GetID() == id){
                node = nodo;
                return node;
            }else{
                node = GetNodeByIDAux(id, node, nodo);

            }
        }

    }

    return node;
}
