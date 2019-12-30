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
    PathComp,
    NavMeshComp,
    CurrentNavMeshComp
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
