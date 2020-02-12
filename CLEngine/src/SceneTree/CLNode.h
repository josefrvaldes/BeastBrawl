#pragma once

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

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

        //Setters
        bool SetEntity(CLEntity* e)                    { entity = e; return true; }
        bool SetFather(CLNode* f)                    { father = f; return true; }
        void SetTranslation(glm::vec3 trans)           { translation = trans; changed = true; }
        void SetRotation(glm::vec3 rot)                { rotation = rot; changed = true; }
        void SetScalation(glm::vec3 scale)             { scalation = scale; changed = true; }
        void SetTransformationMat(glm::mat4 transfMat) { transformationMat = transfMat; }

        //Methods
        bool AddChild(CLNode* child);
        bool RemoveChild(CLNode* child);
        bool HasChild(CLNode* child);
        CLNode* GetNodeByID(unsigned int id);
        void DeleteNode(unsigned int id);
        void DeleteNode(CLNode* node);
        glm::mat4 Translate();
        glm::mat4 Rotate();
        glm::mat4 Scale();
        glm::mat4 CalculateTransformationMatrix();

        void DrawTree(CLNode* root);
        void DFSTree(glm::mat4);

    private:

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

};

}
