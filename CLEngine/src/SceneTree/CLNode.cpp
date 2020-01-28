#include "CLNode.h" 
using namespace CLE;

CLNode::CLNode(){

}

CLNode::CLNode(CLEntity* entity){
    this->entity = entity;
}


bool CLNode::AddChild(CLNode* child){
    //Tipica comprobacion de Programacion 2 pero bueno por si acaso
    if(child!=nullptr){
        childs.push_back(child);
        return true;
    }
    return false;
}

bool CLNode::RemoveChild(CLNode* child){

    return true;
}

CLNode* CLNode::SearchChild(unsigned int id){

}

void CLNode::Translate(const glm::vec3 translationVec){

}

void CLNode::Rotate(const glm::vec3 rotationVec){

}

void CLNode::Scale(const glm::vec3 scaleVec){

}

