#pragma once

#include "CLEntity.h"
#include <vector>
#include <iostream>
#include "../../../include/glm/glm.hpp"

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
        void SetTranslation(glm::vec3 trans)           { translation = trans; }
        void SetRotation(glm::vec3 rot)                { rotation = rot; }
        void SetScalation(glm::vec3 scale)             { scalation = scale; }
        void SetTransformationMat(glm::mat4 transfMat) { transformationMat = transfMat; }

        //Methods
        bool AddChild(CLNode* child);
        bool RemoveChild(CLNode* child);
        bool HasChild(CLNode* child);
        void Translate(const glm::vec3 translationVec);
        void Rotate(const glm::vec3 rotationVec);
        void Scale(const glm::vec3 scaleVec);

        void DrawTree(CLNode* root);

    private:
        CLEntity* entity {nullptr};
        CLNode* father {nullptr};
        vector<CLNode*> childs;

        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scalation;
        glm::mat4 transformationMat;

};

}
