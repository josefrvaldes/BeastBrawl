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
 * 
 */
 
 
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




void CLNode::DFSTree(glm::mat4 mA, CLCamera* cam, const glm::mat4& VPmatrix) {
    // > Flag
    // > > Calcular matriz
    // > Dibujar
    // > Para cada hijo
    // > > DFSTree(mT)
    
    if(Constants::CLIPPING_OCTREE && !octreeVisible)
        return;

    if (changed) {
        transformationMat = mA*CalculateTransformationMatrix();
        changed = false;
    }
    glm::vec3 pos    = GetGlobalTranslation();
    CLE::CLFrustum::Visibility frusVisibility = CLE::CLFrustum::Visibility::Invisible;
    if(!ignoreFrustrum){
        auto& frustrum_m = cam->GetFrustum();
        if(frustum_ == typeFrustum::AABB)   frusVisibility = frustrum_m.IsInside(pos, dimensionsBoundingBox.x);
        else                                frusVisibility = frustrum_m.IsInside(extremeMinMesh, extremeMaxMesh);
    }

    //Voy a comentar de momento el frustrum ya que para el particle system puede dar problemas
    if( (entity && visible && frusVisibility == CLE::CLFrustum::Visibility::Completly) || (entity && visible && ignoreFrustrum) ){ 
        glUseProgram(shaderProgramID); 
        glm::mat4 MVP = VPmatrix * transformationMat;
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformationMat));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
        glUniform3fv(glGetUniformLocation(shaderProgramID, "position"), 1, glm::value_ptr(pos));
        auto particleEntity = dynamic_cast<CLParticleSystem*>(entity.get());
        if((particleEntity && particlesActivated) || !particleEntity){
            entity->Draw(shaderProgramID);
        }
    }

    for (auto node : childs) {
        node->DFSTree(transformationMat, cam, VPmatrix);
    }
}





void CLNode::DFSTree(glm::mat4 mA,  CLCamera* cam, GLuint shaderID, const glm::mat4& lightSpaceMatrix) {

    if(Constants::CLIPPING_OCTREE && !octreeVisible)
        return;

    if (changed) {
        transformationMat = mA*CalculateTransformationMatrix();
        changed = false;
    }
    glm::vec3 pos    = GetGlobalTranslation();
    CLE::CLFrustum::Visibility frusVisibility = CLE::CLFrustum::Visibility::Invisible;
    if(!ignoreFrustrum){
        auto& frustrum_m = cam->GetFrustum();
        if(frustum_ == typeFrustum::AABB)   frusVisibility = frustrum_m.IsInside(pos, dimensionsBoundingBox.x);
        else                                frusVisibility = frustrum_m.IsInside(extremeMinMesh, extremeMaxMesh);
    }


    //auto& frustum_m = device->GetActiveCamera()->GetFrustum();
    //CLE::CLFrustum::Visibility frusVisibility = frustum_m.IsInside(translation, dimensionsBoundingBox);

    if( (entity && visible && frusVisibility == CLE::CLFrustum::Visibility::Completly) || (entity && visible && ignoreFrustrum) ){ 
        glm::mat4 lightSpaceModel = lightSpaceMatrix * transformationMat;
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "lightSpaceModel"), 1, GL_FALSE, glm::value_ptr(lightSpaceModel));

        
        entity->DrawDepthMap(shaderID);

    }

    for (auto node : childs) {
        node->DFSTree(transformationMat, cam, shaderID, lightSpaceMatrix);
    }
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

    float dimGeneral = glm::distance(extremeMaxMesh.x, extremeMinMesh.x);
    if(dimGeneral < glm::distance(extremeMaxMesh.y, extremeMinMesh.y))
        dimGeneral = glm::distance(extremeMaxMesh.y, extremeMinMesh.y);
    if(dimGeneral < glm::distance(extremeMaxMesh.z, extremeMinMesh.z))
        dimGeneral = glm::distance(extremeMaxMesh.z, extremeMinMesh.z);

    dimGeneral = (dimGeneral * GetScalation().x)/2;
    dimensionsBoundingBox.x = dimGeneral;
    dimensionsBoundingBox.y = dimGeneral;
    dimensionsBoundingBox.z = dimGeneral;    

    //cout << "Los extremos son: " << endl;
    //cout << "minimo: ( " << extremeMinMesh.x<< " , " << extremeMinMesh.y<< " , " <<extremeMinMesh.z<< 
    //" ) , maximo: " << extremeMaxMesh.x<< " , " << extremeMaxMesh.y<< " , " << extremeMaxMesh.z<< " )"<< endl;
    return dimGeneral;
}


void CLNode::CalculateBoundingBoxOBB(){
    extremeMinMesh = glm::vec3(0.0,0.0,0.0); 
    extremeMaxMesh = glm::vec3(0.0,0.0,0.0);
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

    extremeMinMesh.x *= -1;
    extremeMaxMesh.x *= -1;

    dimensionsBoundingBox.x = (glm::distance(extremeMaxMesh.x, extremeMinMesh.x) *GetScalation().x)/2;
    dimensionsBoundingBox.y = (glm::distance(extremeMaxMesh.y, extremeMinMesh.y) *GetScalation().y)/2;
    dimensionsBoundingBox.z = (glm::distance(extremeMaxMesh.z, extremeMinMesh.z) *GetScalation().z)/2;

    frustum_ = typeFrustum::OBB;

    //cout << "Los extremos son: " << endl;
    //cout << "minimo: ( " << extremeMinMesh.x<< " , " << extremeMinMesh.y<< " , " <<extremeMinMesh.z<< 
    //" ) , maximo: " << extremeMaxMesh.x<< " , " << extremeMaxMesh.y<< " , " << extremeMaxMesh.z<< " )"<< endl;
}
