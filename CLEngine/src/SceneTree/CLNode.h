#pragma once

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "../../include/glew/glew.h"

#include "CLEntity.h"

using namespace std;
//using namespace CLE;

namespace CLE{

class CLNode{
    public:
        CLNode();
        CLNode(CLEntity* entity);
        ~CLNode(){};

        //Getters
        CLEntity* GetEntity() const           { return entity; }
        CLNode*   GetFather() const           { return father; }
        vector<CLNode*>   GetChilds() const   { return childs; }

        glm::vec3 GetTranslation() const       { return translation; }
        glm::vec3 GetRotation()    const       { return rotation; }
        glm::vec3 GetScalation()   const       { return scalation; }
        glm::mat4 GetTransformationMat() const { return transformationMat; }
        GLuint GetModelMatrixID()       { return shaderProgramID;}

        //Setters
        bool SetEntity(CLEntity* e)                     { entity = e; return true; }
        bool SetFather(CLNode* f)                       { father = f; return true; }
        void SetTranslation(glm::vec3); 
        void SetRotation(glm::vec3);
        void SetScalation(glm::vec3);
        void SetTransformationMat(glm::mat4 transfMat)  { transformationMat = transfMat; }
        void SetShaderProgramID(GLuint id)         { shaderProgramID = id; }

        //Methods
        bool AddChild(CLNode* child);
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

        void DrawTree(CLNode* root);
        void DFSTree(glm::mat4);

    private:

        void ActivateFlag();

        bool changed { true };

        CLEntity* entity {nullptr};
        CLNode* father {nullptr};
        vector<CLNode*> childs;

        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scalation;
        glm::mat4 transformationMat;

        //Methods
        CLNode* GetNodeByIDAux(unsigned int id, CLNode* node, CLNode* root);

        // Identificadores de las variables que cambia para pasarle info al shader.
        GLuint shaderProgramID;

};

}
