#pragma once

#ifndef CLNODE_H
#define CLNODE_H


#include <vector>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../../include/glew/glew.h"

#include "CLEntity.h"
#include "CLMesh.h"
#include "CLPointLight.h"
#include "CLDirectLight.h"
#include "CLSpotLight.h"
#include "CLCamera.h"
#include "CLParticleSystem.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "CLSkybox.h"
#include "CLShadowMapping.h"
#include "CLBillboard.h"

#include "../Frustum/CLFrustum.h"
#include "../ResourceManager/CLResourceManager.h"
#include "CLGrassSystem.h"

#include "../Built-In-Classes/CLColor.h"

using namespace std;

namespace CLE{
//! Clase Nodo de la escena
//! En la escena gestionaremos todo mediante nodos de manera recursiva
//! aqui puedes crear nodos y asignarles cualquier tipo de entidad disponible
//! @see CLE::CLEntity
class CLNode{
    public:
        CLNode();
        CLNode(shared_ptr<CLEntity> entity);
        ~CLNode(){};

        void AddChild(shared_ptr<CLNode> child);

        //Getters
        //! Devuelve la entidad del nodo
        //! @returns entity Puntero a la entidad CLE::CLEntity
        CLEntity* GetEntity()                    const   { return entity.get(); }
        //! Devuelve el nodo padre
        //! @returns father Puntero al nodo padre CLE::CLNode
        CLNode*   GetFather()                    const   { return father; }
        //! Devuelve un vector de hijos
        //! @returns childs vector<shader_ptr<CLNode>> los hijos de ese nodo
        vector<shared_ptr<CLNode>>   GetChilds() const   { return childs; }
        //! Devuelve la traslacion del nodo
        //! @returns traslation Traslacion del nodo
        glm::vec3 GetTranslation() const       { return translation; }
        //! Devuelve la rotación del nodo
        //! @returns rotation Rotación del nodo
        glm::vec3 GetRotation()    const       { return rotation; }
        //! Devuelve el escalado del nodo
        //! @returns scalation Escalado del nodo
        glm::vec3 GetScalation()   const       { return scalation; }
        //! Devuelve la matriz de trasformacion del nodo
        //! @returns transformationMat Matriz de transformacion del nodo
        glm::mat4 GetTransformationMat() const { return transformationMat; }
        //! Devuelve el ID del shader utilizando
        //! @returns shaderProgramID ID de OpenGL para el shader
        GLuint GetShaderProgramID() const      { return shaderProgramID;}
        //! Devuelve la bounding box del nodo
        //! @returns dimensionsBoundingBox Dimensiones del objeto
        float GetBoundingBox() const           { return dimensionsBoundingBox;}
        //! Devuelve la traslacion global del nodo
        //! @returns traslation Traslacion global del nodo
        glm::vec3 GetGlobalTranslation() const;
        //! Devuelve la rotación global del nodo
        //! @returns rotation Rotación global del nodo
        glm::vec3 GetGlobalRotation() const;
        //! Devuelve el escalado global del nodo
        //! @returns scalation Escalado global del nodo
        glm::vec3 GetGlobalScalation() const;
        //! Devuelve si estan activadas las particulas 
        //! @returns particlesActivated
        bool static GetParticlesActivated() { return particlesActivated; };

        


        //Setters
        //! Asigna un padre al nodo
        //! @param f Nodo padre
        bool SetFather(CLNode* f)                        { father = f; return true; }
        //! Asigna una traslacion al nodo
        //! @param t Traslacion del nodo
        void SetTranslation(glm::vec3 t); 
        //! Asigna una rotación al nodo
        //! @param r Rotación del nodo
        void SetRotation(glm::vec3 r);
        //! Asigna un escalado al nodo
        //! @param s Escalado del nodo
        void SetScalation(glm::vec3 s);
        //! Asigna la visibilidad del nodo
        //! @param v Si es visible o no
        void SetVisible(bool v) {visible = v;};
        //! Asigna la visibilidad del nodo dependiendo del octree
        //! @param id El id del nodo
        //! @param v Si es visible o no
        void SetOctreeVisibleById(unsigned int id, bool v);
        //! Activa o desactiva las particulas
        //! @param particlesActivated
        void SetParticlesActivated(bool a) { particlesActivated = a; };

        void SetShaderProgramID(GLuint id)              { shaderProgramID = id; }
        
        void SetOctreeVisible(bool v) { octreeVisible = v; };
        
        //! Elimina un hijo de ese nodo
        //! @param child Puntero al hijo para eliminar
        //! @returns bool Si ha sido finalizado con exito o no
        bool RemoveChild(CLNode* child);
        //! Comprueba si tiene ese hijo
        //! @param child Puntero al hijo para comprobar
        //! @returns bool Si ha sido finalizado con exito o no
        bool HasChild(CLNode* child);
        //! Busca un nodo en el arbol por su ID
        //! @param id Identificador para buscar
        //! @returns CLNode* Puntero al nodo
        float GetBoundingSizeById(unsigned int id);

        glm::mat4 TranslateMatrix();
        glm::mat4 RotateMatrix();
        glm::mat4 ScaleMatrix();
        glm::mat4 CalculateTransformationMatrix();
        //! Calcula la bounding box del objeto
        float CalculateBoundingBox();


        //! Dibuja la escena
        void DFSTree(glm::mat4,CLCamera* cam, const glm::mat4& VPmatrix);
        void DFSTree(glm::mat4 mA, GLuint shaderID, const glm::mat4& lightSpaceMatrix);


        void SetIgnoreFrustrum(bool f) { ignoreFrustrum = f;};



        //DEBUG
        //! Imprime el arbol
        //! @param root Nodo padre
        void DrawTree(CLNode* root); 
    private:

        void ActivateFlag();

        bool changed { true };
        bool visible { true };
        bool octreeVisible { true };
        bool ignoreFrustrum { false }; //Si esta a true no le afecta el frustrum culling
        inline static bool particlesActivated { true }; //Si esta a true se dibujan las particulas
        shared_ptr<CLEntity> entity {nullptr};
        CLNode* father {nullptr};
        vector<shared_ptr<CLNode>> childs;
        GLuint shaderProgramID;

        glm::vec3 translation       {glm::vec3(0.0f)};  // posicion del nodo
        glm::vec3 rotation          {glm::vec3(0.0f)};  // rotacion del nodo
        glm::vec3 scalation         {glm::vec3(1.0f)};  // escalado del nodo
        glm::mat4 transformationMat {glm::mat4(1.0f)};  // matriz modelo del nodo




        // BOUNDING BOX
        //glm::vec3 extremeMinMesh    {glm::vec3(0.0,0.0,0.0)}; // definimos el vertice mayor de la malla para el BoundingBpx
        //glm::vec3 extremeMaxMesh    {glm::vec3(0.0,0.0,0.0)}; // definimos el vertice menos de la malla para el BoundingBox
        float dimensionsBoundingBox {0.0}; // width , height, depht
        glm::vec3 RotatePointAroundCenter(const glm::vec3& point_ , const glm::vec3& center, const glm::vec3& rot) const;
        glm::vec3 TranslatePointAroundCenter(const glm::vec3& point_ , const glm::vec3& center, const glm::vec3& trans) const;





};

}

#endif