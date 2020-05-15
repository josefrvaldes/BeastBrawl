#pragma once

#include "../ResourceManager/CLResourceMaterial.h"
#include "../ResourceManager/CLResourceMesh.h"
#include "../ResourceManager/CLResourceTexture.h"
#include "CLEntity.h"

namespace CLE {
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

    //! Asigna una animación
    //! @param m Objecto CLResourceMesh donde tiene la malla
    void SetAnimation(vector<CLResourceMesh*> keyframes_) {
        keyframes = keyframes_;
        mesh = keyframes[0];
        nextMesh = keyframes[1];
        isInterpolated = false;
    }

    //! Asigna una animación
    //! @param m Objecto CLResourceMesh donde tiene la malla
    void SetAnimationInterpolated(vector<CLResourceMesh*> keyframes_, vector<uint8_t> framesBetweenKeyFrames_) {
        if (keyframes != keyframes_) {
            keyframes = keyframes_;
            mesh = keyframes[0];
            nextMesh = keyframes[1];
            distanceBetweenKeyFrames = framesBetweenKeyFrames_;
            isInterpolated = true;
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
    void DrawDepthMap(GLuint shaderID);

   private:
    void Update();
    // las distintas mallas que corresponden con cada keyframe
    vector<CLResourceMesh*> keyframes;

    // indica el idx del keyframe interpolado. Por ejemplo, si entre
    // dos keyframes hay 8 frames, este número indicaría que ahora mismo vamos por el 3
    uint8_t currentDistance{0};

    // indica el keyframe que estamos procesando ahora
    uint8_t currentKeyFrameIndex{0};
    uint8_t nextKeyFrameIndex{1};
    bool isInterpolated{false};
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