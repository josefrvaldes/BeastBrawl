#pragma once

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
    CarComp,
    PowerUpComp,
    BoxPowerUpComp,
    ShieldComp,
    NitroComp,
    RoboJoroboComp,
    TotemComp,
    TargetEntityComp,
    WayPointEdgesComp,
    DimensionsComp,
    CompBoundingSphere,
    CompBoundingAABox,
    CompBoundingPlane,
    PathComp,
    CollidingComp
};

class Component
{
public:
    Component() {};
    ~Component() {};

    CompType getType() { return m_compType; };

protected:
    CompType m_compType;

private:
    
};
