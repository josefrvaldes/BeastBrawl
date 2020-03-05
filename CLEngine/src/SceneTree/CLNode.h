#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../../include/glew/glew.h"

#include "CLEntity.h"
#include "CLMesh.h"
#include "CLLight.h"
#include "CLCamera.h"


using namespace std;
//using namespace CLE;

namespace CLE{

class CLNode{
    public:
        CLNode();
        CLNode(shared_ptr<CLEntity> entity);
        ~CLNode(){};

        //Getters
        CLEntity* GetEntity() const                      { return entity.get(); }
        CLNode*   GetFather() const                      { return father; }
        vector<shared_ptr<CLNode>>   GetChilds() const   { return childs; }
        glm::vec3 GetTranslation() const       { return translation; }
        glm::vec3 GetRotation()    const       { return rotation; }
        glm::vec3 GetScalation()   const       { return scalation; }
        glm::mat4 GetTransformationMat() const { return transformationMat; }
        GLuint GetShaderProgramID() const      { return shaderProgramID;}
        glm::vec3 GetGlobalTranslation() const;
        glm::vec3 GetGlobalRotation() const;
        glm::vec3 GetGlobalScalation() const;
        CLCamera* GetActiveCamera();

        //Setters
        bool SetFather(CLNode* f)                       { father = f; return true; }
        void SetTranslation(glm::vec3); 
        void SetRotation(glm::vec3);
        void SetScalation(glm::vec3);
        void SetTransformationMat(glm::mat4 transfMat)  { transformationMat = transfMat; }
        void SetShaderProgramID(GLuint id)         { shaderProgramID = id; }

        //Methods
        CLNode* AddGroup(unsigned int id);
        CLNode* AddMesh(unsigned int id);
        CLNode* AddLight(unsigned int id);
        CLNode* AddCamera(unsigned int id);
        bool RemoveChild(CLNode* child);
        bool HasChild(CLNode* child);
        CLNode* GetNodeByID(unsigned int id);
        void DeleteNode(unsigned int id);
        void DeleteNode(CLNode* node);
        glm::mat4 TranslateMatrix();
        glm::mat4 RotateMatrix();
        glm::mat4 ScaleMatrix();
        void Translate(glm::vec3);
        void Rotate(glm::vec3);
        void Scale(glm::vec3);
        glm::mat4 CalculateTransformationMatrix();
        void CalculateViewProjMatrix();

        void DrawTree(CLNode* root);
        void DFSTree(glm::mat4);

        void SetVisible(bool v) {visible = v;};

    private:

        void ActivateFlag();

        bool changed { true };
        bool visible { true };

        shared_ptr<CLEntity> entity {nullptr};
        CLNode* father {nullptr};
        vector<shared_ptr<CLNode>> childs;

        glm::vec3 translation = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 scalation = glm::vec3(1.0f);
        glm::mat4 transformationMat = glm::mat4(1.0f);

        inline static glm::mat4 projection;
        inline static glm::mat4 view;

        //Methods
        CLNode* GetNodeByIDAux(unsigned int id, CLNode* node, CLNode* root);

        // Identificadores de las variables que cambia para pasarle info al shader.
        GLuint shaderProgramID;

        inline static vector<CLNode*> lights;
        inline static vector<CLNode*> cameras;

};

}
