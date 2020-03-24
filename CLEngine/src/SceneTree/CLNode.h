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
#include "../ResourceManager/CLResourceMesh.h"
#include "CLSkybox.h"
#include "CLShadowMapping.h"


#include "../Frustum/CLFrustum.h"
#include "../ResourceManager/CLResourceManager.h"
#include "../ResourceManager/CLResourceShader.h"

#include "../Built-In-Classes/CLColor.h"

using namespace std;
//using namespace CLE;

namespace CLE{

class CLNode{
    public:
        CLNode();
        CLNode(shared_ptr<CLEntity> entity);
        ~CLNode(){};

        //Getters
        CLEntity* GetEntity()                    const   { return entity.get(); }
        CLNode*   GetFather()                    const   { return father; }
        vector<shared_ptr<CLNode>>   GetChilds() const   { return childs; }
        glm::vec3 GetTranslation() const       { return translation; }
        glm::vec3 GetRotation()    const       { return rotation; }
        glm::vec3 GetScalation()   const       { return scalation; }
        glm::mat4 GetTransformationMat() const { return transformationMat; }
        GLuint GetShaderProgramID() const      { return shaderProgramID;}
        float GetBoundingBox() const           { return dimensionsBoundingBox;}
        glm::vec3 GetGlobalTranslation() const;
        glm::vec3 GetGlobalRotation() const;
        glm::vec3 GetGlobalScalation() const;
        static glm::mat4 GetViewMatrix()               { return view; }
        static glm::mat4 GetProjectionMatrix()         { return projection; }
        CLCamera* GetActiveCamera();
        vector<CLNode*> GetLights()             { return lights; };
        vector<CLNode*> GetCameras()            { return cameras; };
        CLShadowMapping* GetShadowMapping()     {return shadowMapping.get();};
        GLuint GetSimpleDepthShader()           {return simpleDepthShader;};
        CLResourceShader* GetDepthShader()      {return DepthShadder;};
        void RemoveLightsAndCameras();


        //Setters
        bool SetFather(CLNode* f)                        { father = f; return true; }
        void SetTranslation(glm::vec3); 
        void SetRotation(glm::vec3);
        void SetScalation(glm::vec3);
        void SetTransformationMat(glm::mat4 transfMat)  { transformationMat = transfMat; }
        void SetShaderProgramID(GLuint id)              { shaderProgramID = id; }

        //Methods
        CLNode* AddGroup(unsigned int id);
        CLNode* AddMesh(unsigned int id);
        CLNode* AddMesh(unsigned int id,string mesh);
        CLNode* AddLight(unsigned int id);
        CLNode* AddLight(unsigned int id,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
        CLNode* AddCamera(unsigned int id);
        void AddSkybox(string right, string left, string top, string bottom, string front, string back);
        void AddShadowMapping(GLuint lightId);

        bool RemoveChild(CLNode* child);
        bool HasChild(CLNode* child);
        CLNode* GetNodeByID(unsigned int id);
        bool DeleteNode(unsigned int id);
        bool DeleteNode(CLNode* node);
        glm::mat4 TranslateMatrix();
        glm::mat4 RotateMatrix();
        glm::mat4 ScaleMatrix();
        void Translate(glm::vec3);
        void Rotate(glm::vec3);
        void Scale(glm::vec3);
        glm::mat4 CalculateTransformationMatrix();
        void CalculateViewProjMatrix();
        void CalculateLights();
        float CalculateBoundingBox();

        void DFSTree(glm::mat4);
        void DFSTree(glm::mat4 mA, GLuint shaderID);
        void DrawSkybox();
        

        void SetVisible(bool v) {visible = v;};
        const void Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2,CLColor color) const;
        const void Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2) const;
        void SetDrawLineWidth(int w) {lineWidth = w;};
        void SetLightingEffects(bool l) { hasLightingEffects = l;};

        //DEBUG
        void DrawTree(CLNode* root); 
    private:

        void ActivateFlag();

        bool changed { true };
        bool visible { true };
        bool hasLightingEffects { true }; //Sirve para que no se le aplique luce y ahorrarse el calculo de luces

        shared_ptr<CLEntity> entity {nullptr};
        CLNode* father {nullptr};
        vector<shared_ptr<CLNode>> childs;

        glm::vec3 translation       {glm::vec3(0.0f)};  // posicion del nodo
        glm::vec3 rotation          {glm::vec3(0.0f)};  // rotacion del nodo
        glm::vec3 scalation         {glm::vec3(1.0f)};  // escalado del nodo
        glm::mat4 transformationMat {glm::mat4(1.0f)};  // matriz modelo del nodo

        inline static glm::mat4 projection;             // matriz proyeccion del modelo
        inline static glm::mat4 view;                   // matriz view del modelo

        //Methods
        CLNode* GetNodeByIDAux(unsigned int id, CLNode* node, CLNode* root);

        // Identificadores de las variables que cambia para pasarle info al shader.
        GLuint shaderProgramID;
        int lineWidth = 1;
        inline static vector<CLNode*> lights;
        inline static vector<CLNode*> cameras;
        inline static GLuint debugShader = 0;

        // BOUNDING BOX
        //glm::vec3 extremeMinMesh    {glm::vec3(0.0,0.0,0.0)}; // definimos el vertice mayor de la malla para el BoundingBpx
        //glm::vec3 extremeMaxMesh    {glm::vec3(0.0,0.0,0.0)}; // definimos el vertice menos de la malla para el BoundingBox
        float dimensionsBoundingBox {0.0}; // width , height, depht
        glm::vec3 RotatePointAroundCenter(const glm::vec3& point_ , const glm::vec3& center, const glm::vec3& rot) const;
        glm::vec3 TranslatePointAroundCenter(const glm::vec3& point_ , const glm::vec3& center, const glm::vec3& trans) const;
        //Skybox
        inline static unique_ptr<CLSkybox> skybox = nullptr;
        inline static GLuint skyboxShader = 0;

        unique_ptr<CLShadowMapping> shadowMapping = nullptr;
        GLuint simpleDepthShader = 0;
        CLResourceShader* DepthShadder = nullptr;

};

}
