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
 * @author Rubén Rubio Martínez <https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/> 
 * @author Jose Valdés Sirvent
 * 
 */
 
 
 #pragma once

#include "../ResourceManager/CLResourceMaterial.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"
#include "CLEntity.h"
#include <memory>
#include <unordered_map>

namespace CLE {


class CLAnimation {
    public:
    CLAnimation() = delete;
    CLAnimation(string name_, vector<CLResourceMesh *> keyframes_, bool loop_) : name{name_}, keyframes{keyframes_}, loop{loop_}, isInterpolated{false} {};
    CLAnimation(string name_, vector<CLResourceMesh *> keyframes_, bool loop_, vector<uint8_t> framesBetweenKeyframes_) : name{name_}, keyframes{keyframes_}, loop{loop_}, framesBetweenKeyframes{framesBetweenKeyframes_}, isInterpolated{false} {};

    string name;
    vector<CLResourceMesh *> keyframes;
    bool loop;
    vector<uint8_t> framesBetweenKeyframes;
    bool isInterpolated;
};


//! Clase para crear mallas
//! Clase que permite crear mallas en nuestro motor gráfico
class CLMesh : public CLEntity {
   public:
    CLMesh();
    CLMesh(unsigned int idEntity) : CLEntity(idEntity){};
    ~CLMesh(){};

    //! Asigna una malla
    //! @param m Objecto CLResourceMesh donde tiene la malla
    void SetMesh(CLResourceMesh* m) { mesh = m; }

    void AddAnimation(string name, shared_ptr<CLAnimation> animation) {
        animations[name] = animation;
    }


    //! Asigna una animación
    //! @param name String con el nombre de la animación
    void ActivateAnimation(string name) {
        if (currentAnimation == nullptr || currentAnimation->name != name) {
            currentAnimation = animations[name];
            mesh = currentAnimation->keyframes[0];
            nextMesh = currentAnimation->keyframes[1];
            if(currentAnimation->isInterpolated)
                distanceBetweenKeyFrames = currentAnimation->framesBetweenKeyframes;
        }
    }

    //! Asigna un material
    //! @param m Objeto CLResourceMaterial donde tiene el material
    void SetMaterial(CLResourceMaterial* m) { material = m; }

    //GETTERS
    //! Devuelve la malla
    //! @returns mesh Objeto CLResourceMesh almacenado
    CLResourceMesh* GetMesh() const { return mesh; }

    void Draw(GLuint shaderID);
    void Animate();
    void AnimateInterpolated();
    void ResetAnimation();
    void DrawDepthMap(GLuint shaderID);

   private:
    void Update();
    // las distintas mallas que corresponden con cada keyframe
    unordered_map<string, shared_ptr<CLAnimation>> animations;

    shared_ptr<CLAnimation> currentAnimation;

    // indica el idx del keyframe interpolado. Por ejemplo, si entre
    // dos keyframes hay 8 frames, este número indicaría que ahora mismo vamos por el 3
    uint8_t currentDistance{0};

    // indica el keyframe que estamos procesando ahora
    uint8_t currentKeyFrameIndex{0};
    uint8_t nextKeyFrameIndex{1};
    int64_t lastTimeFrameChanged{0};
    const uint8_t ANIMATION_FPS {25};
    const float DELTA_ANIMATION {1.0f/ANIMATION_FPS*1000.f};

    // vector que indica las distancias entre los distintos keyframes
    vector<uint8_t> distanceBetweenKeyFrames;

    CLResourceMesh* mesh = nullptr;
    CLResourceMesh* nextMesh = nullptr;
    CLResourceMaterial* material = nullptr;
    CLResourceTexture* texture = nullptr;

    void GoToNextKeyFrames();
    bool ItsTimeToAnimate();
};
}  // namespace CLE