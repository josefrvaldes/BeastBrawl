#pragma once

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
#include "../ResourceManager/CLResourceShader.h"

#include "../Built-In-Classes/CLColor.h"

using namespace std;
//using namespace CLE;


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
        //! Devuelve la matriz view de la escena
        //! @returns view Matriz view de la escena
        static glm::mat4 GetViewMatrix()               { return view; }
        //! Devuelve la matriz projection de la escena
        //! @returns projection Matriz projection de la escena
        static glm::mat4 GetProjectionMatrix()         { return projection; }
        //! Devuelve la entidad de la camara activa
        //! @returns camera Camara activa en la escena CLE::CLCamera
        CLCamera* GetActiveCamera();
        //! Devuelve el nodo de la camara activa
        //! @returns camera Nodo de la camara activa en la escena
        CLNode*   GetActiveCameraNode();
        //! Devuelve las luces puntuales de la escena
        //! @returns pointLights Vector de luces puntuales de la escena
        vector<CLNode*> GetPointLights()              { return pointLights; };
        //! Devuelve las luces direccionales de la escena
        //! @returns directLights Vector de luces direccionales de la escena
        vector<CLNode*> GetDirectLights()             { return directLights; };
        //! Devuelve las camaras de la escena
        //! @returns cameras Vector de camaras de la escena
        vector<CLNode*> GetCameras()            { return cameras; };
        //! Devuelve el ShadowMapping
        //! @returns shadowMapping Objeto CLE::CLShadowMapping
        static CLShadowMapping* GetShadowMapping()     {return shadowMapping.get();};
        //! Devuelve el objeto shader del shadowMapping
        //! @returns depthShdder Objeto CLE::CLResourceShader
        CLResourceShader* GetDepthShader()      {return depthShadder;};
        


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
        void SetTransformationMat(glm::mat4 transfMat)  { transformationMat = transfMat; }
        void SetShaderProgramID(GLuint id)              { shaderProgramID = id; }
        

        //Methods
        //! Añade un grupo a la escena
        //! @param id Identificador del nodo
        //! @returns CLE::CLNode()
        CLNode* AddGroup(unsigned int id);
        //! Añade una malla a la escena
        //! @param id Identificador del nodo
        //! @returns CLE::CLNode()
        CLNode* AddMesh(unsigned int id);
        //! Añade una malla a la escena
        //! @param id Identificador del nodo
        //! @param mesh Ruta de la malla
        //! @returns CLE::CLNode* 
        //! @see CLE::CLMesh()
        CLNode* AddMesh(unsigned int id,string mesh);
        //! Añade una luz puntual a la escena
        //! @param id Identificador del nodo
        //! @returns CLE::CLNode* 
        //! @see CLE::CLPointLight()
        CLNode* AddPointLight(unsigned int id);
        //! Añade una luz puntual a la escena
        //! @param id Identificador del nodo
        //! @param intensity Intensidad de la luz
        //! @param ambient Valor ambiente de la luz
        //! @param diffuse Valor difuso de la luz
        //! @param specular Valor especular de la luz
        //! @param constant Valor constante de la atenuación de la luz
        //! @param linear Valor lineal de la atenuación de la luz
        //! @param quadratic Valor cuadratico de la atenuación de la luz
        //! @returns CLE::CLNode* 
        //! @see CLE::CLPointLight()
        CLNode* AddPointLight(unsigned int id,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
        //! Añade una luz direccional a la escena
        //! @param id Identificador del nodo
        //! @returns CLE::CLNode* 
        //! @see CLE::CLDirectLight()
        CLNode* AddDirectLight(unsigned int id);
        //! Añade una luz puntual a la escena
        //! @param id Identificador del nodo
        //! @param direction Vector de dirección de la luz
        //! @param intensity Intensidad de la luz
        //! @param ambient Valor ambiente de la luz
        //! @param diffuse Valor difuso de la luz
        //! @param specular Valor especular de la luz
        //! @param constant Valor constante de la atenuación de la luz
        //! @param linear Valor lineal de la atenuación de la luz
        //! @param quadratic Valor cuadratico de la atenuación de la luz
        //! @returns CLE::CLNode* 
        //! @see CLE::CLDirectLight()
        CLNode* AddDirectLight(unsigned int id,glm::vec3 direction,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
        //! Añade una luz spot a la escena
        //! @param id Identificador del nodo
        //! @returns CLE::CLNode* 
        //! @see CLE::CLSpotLight()
        CLNode* AddSpotLight(unsigned int id);
        //! Añade una luz spot a la escena
        //! @param id Identificador del nodo
        //! @param direction Vector de dirección de la luz
        //! @param cutOff Valor de cutOff de la luz
        //! @param outerCutOff Valor de outerCutOff de la luz
        //! @param intensity Intensidad de la luz
        //! @param ambient Valor ambiente de la luz
        //! @param diffuse Valor difuso de la luz
        //! @param specular Valor especular de la luz
        //! @param constant Valor constante de la atenuación de la luz
        //! @param linear Valor lineal de la atenuación de la luz
        //! @param quadratic Valor cuadratico de la atenuación de la luz
        //! @returns CLE::CLNode* 
        //! @see CLE::CLSpotLight()
        CLNode* AddSpotLight(unsigned int id,glm::vec3 direction,float cutOff,float outerCutOff,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
        //! Añade una camara a la escena
        //! @param id Identificador del nodo
        //! @returns CLE::CLNode* 
        //! @see CLE::CLCamera()
        CLNode* AddCamera(unsigned int id);
        //! Añade un sistema de particulas version **Punto**
        //! @param id Identificador del nodo
        //! @param nParticles Número de particulas
        //! @param velocity Vector de velocidad
        //! @param texture Ruta de la textura de la particula
        //! @param width Anchura de la particula
        //! @param height Altura de la particula
        //! @param spawnDelay Tiempo entre cada spawn de particula
        //! @param particlesToSpawn Particulas que se spawnean a la vez
        //! @param lifeSpan Vida de las particulas
        //! @param flags Mascara de bytes para aplicarle efectos
        //! @returns CLE::CLNode()
        //! @see CLE::CLParticleSystem()
        CLNode* AddParticleSystem(unsigned int id,ulong nParticles,glm::vec3 velocity,string texture,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan, std::uint_fast8_t flags);
        //! Añade un sistema de particulas version **Linea, Cuadrado o Cubo**
        //! @param id Identificador del nodo
        //! @param nParticles Número de particulas
        //! @param velocity Vector de velocidad
        //! @param texture Ruta de la textura de la particula
        //! @param width Anchura de la particula
        //! @param height Altura de la particula
        //! @param spawnDelay Tiempo entre cada spawn de particula
        //! @param particlesToSpawn Particulas que se spawnean a la vez
        //! @param lifeSpan Vida de las particulas
        //! @param offset Dimensiones del objeto donde se spawnean las particulas
        //! @param orientation Orientacion de dicho spawner
        //! @param flags Mascara de bytes para aplicarle efectos
        //! @returns CLE::CLNode()
        //! @see CLE::CLParticleSystem()
        CLNode* AddParticleSystem(unsigned int id,ulong nParticles,glm::vec3 velocity,string texture,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,glm::vec3 offset, glm::vec3 orientation, std::uint_fast8_t flags);
        //! Añade un sistema de particulas version **Esfera**
        //! @param id Identificador del nodo
        //! @param nParticles Número de particulas
        //! @param velocity Vector de velocidad
        //! @param texture Ruta de la textura de la particula
        //! @param width Anchura de la particula
        //! @param height Altura de la particula
        //! @param spawnDelay Tiempo entre cada spawn de particula
        //! @param particlesToSpawn Particulas que se spawnean a la vez
        //! @param lifeSpan Vida de las particulas
        //! @param radious Radio de la esfera
        //! @param flags Mascara de bytes para aplicarle efectos
        //! @returns CLE::CLNode()
        //! @see CLE::CLParticleSystem()
        CLNode* AddParticleSystem(unsigned int id,ulong nParticles,glm::vec3 velocity,string texture,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,float radious, std::uint_fast8_t flags);
        //! Añade un sistema de particulas version **Circulo**
        //! @param id Identificador del nodo
        //! @param nParticles Número de particulas
        //! @param velocity Vector de velocidad
        //! @param texture Ruta de la textura de la particula
        //! @param width Anchura de la particula
        //! @param height Altura de la particula
        //! @param spawnDelay Tiempo entre cada spawn de particula
        //! @param particlesToSpawn Particulas que se spawnean a la vez
        //! @param lifeSpan Vida de las particulas
        //! @param radious Radio del circulo
        //! @param flags Mascara de bytes para aplicarle efectos
        //! @returns CLE::CLNode()
        //! @see CLE::CLParticleSystem()
        CLNode* AddParticleSystem(unsigned int id,ulong nParticles,glm::vec3 velocity,string texture,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,float radious,glm::vec3 orientation, std::uint_fast8_t flags);

        //! Añade un skybox a la escena
        //! @param right Textura right del cubo
        //! @param left Textura left del cubo
        //! @param top Textura top del cubo
        //! @param bottom Textura bottom del cubo
        //! @param front Textura front del cubo
        //! @param back Textura back del cubo
        //! @see CLE::CLSkybox()
        void AddSkybox(string right, string left, string top, string bottom, string front, string back);
        //! Añade la luz para usar shadow mapping
        //! @param lightId ID de del nodo de la luz a utilizar
        //! @see CLE::CLShadowMapping()
        void AddShadowMapping(GLuint lightId);
        //! Añade un billboard
        //! @param file Fichero de la textura
        //! @param vertically Parametro para poner vertical la textura
        //! @param width_ Anchura del billboard
        //! @param height Altura del billboard
        //! @returns CLE::CLNode()
        //! @see CLE::CLBillboard()
        CLNode* AddBillBoard(unsigned int id,string& file, bool vertically, float width_, float height_);
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
        CLNode* GetNodeByID(unsigned int id);
        //! Elimina un hijo del arbol por su id
        //! @param id Identificador al hijo para eliminar
        //! @returns bool Si ha sido finalizado con exito o no
        bool DeleteNode(unsigned int id);
        //! Elimina un hijo del arbol por su dirección de memoria
        //! @param node Puntero al hijo para eliminar
        //! @returns bool Si ha sido finalizado con exito o no
        bool DeleteNode(CLNode* node);
        glm::mat4 TranslateMatrix();
        glm::mat4 RotateMatrix();
        glm::mat4 ScaleMatrix();
        void Translate(glm::vec3);
        void Rotate(glm::vec3);
        void Scale(glm::vec3);
        glm::mat4 CalculateTransformationMatrix();
        //! Calcula las matriecs view y projection
        void CalculateViewProjMatrix();
        //! Calcula las luces de la escena
        void CalculateLights();
        //! Calcula la bounding box del objeto
        float CalculateBoundingBox();
        //! Elimina todas las luces y camaras de la escena
        void RemoveLightsAndCameras();

        //! Dibuja la escena
        void DFSTree(glm::mat4);
        void DFSTree(glm::mat4 mA, GLuint shaderID);
        //! Dibuja el skybox
        void DrawSkybox();

        
        //! Dibuja una linea primitiva en 3D
        //! @param x1 X inicial
        //! @param y1 Y inicial
        //! @param z1 Z inicial
        //! @param x2 X final
        //! @param y2 Y final
        //! @param z2 Z final
        //! @param color Color de la linea
        //! @see CLE::CLColor()
        const void Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2,CLColor color) const;
        //! Dibuja una linea primitiva en 3D
        //! @param x1 X inicial
        //! @param y1 Y inicial
        //! @param z1 Z inicial
        //! @param x2 X final
        //! @param y2 Y final
        //! @param z2 Z final
        const void Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2) const;
        //! Asigna la anchura de la linea
        //! @param x1 X inicial
        //! @param y1 Y inicial
        //! @param z1 Z inicial
        //! @param x2 X final
        //! @param y2 Y final
        //! @param z2 Z final
        void SetDrawLineWidth(int w) {lineWidth = w;};
        void SetLightingEffects(bool l) { hasLightingEffects = l;};

        //DEBUG
        //! Imprime el arbol
        //! @param root Nodo padre
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
        inline static vector<CLNode*> pointLights;
        inline static vector<CLNode*> directLights;
        inline static vector<CLNode*> spotLights;
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

        inline static GLuint billboardShader = 0;

        inline static unique_ptr<CLShadowMapping> shadowMapping = nullptr;
        inline static GLuint simpleDepthShader = 0;
        inline static CLResourceShader* depthShadder = nullptr;

        //Particle system
        inline static GLuint particleSystemShader = 0;

};

}
