#pragma once

#include "CLEntity.h"
#include "CLNode.h"
#include "../../../src/Constants.h"


#include <iostream>
#include <memory>
#include <vector>

using namespace std;

namespace CLE {
class CLNode;

    class CLParticleSystem : public CLEntity {

        public:
            //CLParticleSystem() = default;
            //CLParticleSystem(unsigned int idEntity) : CLEntity(idEntity) {};
            CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection);
            ~CLParticleSystem() = default;

            void Draw(GLuint shaderID) override;
            void DrawDepthMap(GLuint shaderID) override {};

            void SetCLNode(CLNode* clnode) {node = clnode;}
            const CLNode* GetCLNode() const { return node;}

        private:
            class CLParticle; //Forward declaration

            vector<shared_ptr<CLParticle>> particles;
            CLNode* node {nullptr};     //CLNode para poder acceder a su matriz modelo, vista, proyeccion y demas cosas utiles
            ulong nParticles { 10 }; //Numero de particulas que vamos a tener
            glm::vec3 speedDirection;


            class CLParticle{

                public:
                    CLParticle() = default;
                    CLParticle(CLParticleSystem* emitter);
                    ~CLParticle() = default;

                    void Draw(GLuint shaderID);

                private:
                    void Update();              //Aqui haremos los calculos necesarios de vida, posicion, etc

                    unsigned int VAO,VBO,EBO;
                    CLParticleSystem* particleSystem {nullptr};

            };
            
    };
}