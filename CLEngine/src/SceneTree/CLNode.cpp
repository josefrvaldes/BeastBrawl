#include "CLNode.h" 

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>


#include "../Frustum/CLFrustum.h"
#include "../../../src/Constants.h"


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

void CLNode::AddChild(shared_ptr<CLNode> child){
    childs.push_back(child);
    child->SetFather(this);
}





bool CLNode::RemoveChild(CLNode* child){

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
    translation = t;
    ActivateFlag();
}

void CLNode::SetRotation(glm::vec3 r) {
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
    return newPoint;
}

void CLNode::SetScalation(glm::vec3 s) {
    scalation = s; 
    ActivateFlag();
}



void CLNode::SetOctreeVisibleById(unsigned int id, bool v){
    CLNode* node = GetNodeByID(id);
    node->octreeVisible = v;
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




void CLNode::DFSTree(glm::mat4 mA) {
    // > Flag
    // > > Calcular matriz
    // > Dibujar
    // > Para cada hijo
    // > > DFSTree(mT)
    
    //if(!octreeVisible)
    //    return;

    if (changed) {
        transformationMat = mA*CalculateTransformationMatrix();
        changed = false;
    }
    //auto& frustum_m = device->GetActiveCamera()->GetFrustum();

    //CLE::CLFrustum::Visibility frusVisibility = frustum_m.IsInside(translation);
    //CLE::CLFrustum::Visibility frusVisibility = frustum_m.IsInside(translation, dimensionsBoundingBox);

    //Voy a comentar de momento el frustrum ya que para el particle system puede dar problemas
    if(entity && visible /*&& (frusVisibility == CLE::CLFrustum::Visibility::Completly || !ignoreFrustrum)*/) { 
        //cout << entity->GetID() << endl;
        //auto m = transformationMat;
        glUseProgram(shaderProgramID);
        //Calculamos las luces

        // if(hasLightingEffects){
        //     device->CalculateLights(this);

        // }
        //IMPORTANTE Esto pero con el ID del nodo no de la camara
        //glUniform3fv(glGetUniformLocation(cam->GetShaderProgramID(),"viewPos"),1,glm::value_ptr(cam->GetGlobalTranslation()));

        // glm::mat4 MVP = device->GetProjectionMatrix() * device->GetViewMatrix() * transformationMat;
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformationMat));
        // glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
        // glUniform1i(glGetUniformLocation(shaderProgramID, "shadows"), true); 
        // glUniform1f(glGetUniformLocation(shaderProgramID, "far_plane"), Constants::FAR_PLANE); 

        // glm::mat4 viewProjection = device->GetProjectionMatrix()*device->GetViewMatrix();
        // glm::vec3 camPos = device->GetActiveCameraNode()->GetGlobalTranslation();
        // glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "VPMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjection));
        // glUniform3fv(glGetUniformLocation(shaderProgramID, "cameraPosition"), 1, glm::value_ptr(camPos));
        glm::vec3 pos    = GetGlobalTranslation();
        glUniform3fv(glGetUniformLocation(shaderProgramID, "position"), 1, glm::value_ptr(pos));
        entity->Draw(shaderProgramID);

    }

    for (auto node : childs) {
        node->DFSTree(transformationMat);
    }
}





void CLNode::DFSTree(glm::mat4 mA, GLuint shaderID) {
    // if(!octreeVisible)
    //     return;

    if (changed) {
        transformationMat = mA*CalculateTransformationMatrix();
        changed = false;
    }

    //auto& frustum_m = device->GetActiveCamera()->GetFrustum();
    //CLE::CLFrustum::Visibility frusVisibility = frustum_m.IsInside(translation, dimensionsBoundingBox);

    if(entity && visible /*&& frusVisibility == CLE::CLFrustum::Visibility::Completly*/) { 
        
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(transformationMat));
        entity->DrawDepthMap(shaderID);

    }

    for (auto node : childs) {
        node->DFSTree(transformationMat, shaderID);
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

float CLNode::GetBoundingSizeById(unsigned int id){
    CLNode* node = GetNodeByID(id);
    return node->CalculateBoundingBox();
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






// comprueba si el bounding del octree se encuentra en camera
bool CLNode::OctreeIncamera(float size, const glm::vec3& pos){
    //CalculateViewProjMatrix();
    //if (changed) {
    //    transformationMat = CalculateTransformationMatrix();
    //    changed = false;
    //}

    //auto& frustum_m = device->GetActiveCamera()->GetFrustum();
    //CLE::CLFrustum::Visibility frusVisibility = frustum_m.IsInside(pos, size)//;
//
    //if(frusVisibility == CLE::CLFrustum::Visibility::Invisible)//
    //    return false;//
    //else
    //    return true;
}