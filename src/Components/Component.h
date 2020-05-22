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
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once

#include <stdint.h>

enum CompType {
    LastPositionComp,
    SpeedComp,
    IdComp,
    TypeComp,
    TextureComp,
    MeshComp,
    TransformableComp,
    CameraComp,
    WheelComp,
    WayPointComp,
    PosDestination,
    RemovableObjectComp,
    CarComp,
    HurtComp,
    PowerUpComp,
    BoxPowerUpComp,
    BufferOnline,
    CIDOnlineComp,
    ShieldComp,
    NitroComp,
    RoboJoroboComp,
    TotemComp,
    ClockComp,
    TargetEntityComp,
    WayPointEdgesComp,
    DimensionsComp,
    CompBoundingSphere,
    CompBoundingAABox,
    CompExternalForce,
    CompGravity,
    CompBoundingPlane,
    CompBoundingClindre,
    CompBoundingOBB,
    CompBoundingChassis,
    CompBoundingRay,
    CollidingComp,
    NavMeshComp,
    CurrentNavMeshComp,
    CompRay,
    NamePlateComp,
    OnlineComp,
    ShaderComp,
    LightComp,
    ParentNodeComp,
    ParticleSystemComp,
    AnimationComp,
    BrainAIComp,
    EventHudComp
};

class Component {
   public:
    Component(){};
    ~Component(){};

    CompType getType() { return m_compType; };
    static const uint32_t ID_DIFFERENCE{100000};
    static uint32_t nextId;
    uint32_t CompID{nextId++};

   protected:
    CompType m_compType;

   private:
};
