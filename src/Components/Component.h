#ifndef __COMPONENT_H__
#define __COMPONENT_H__

enum CompType { 
    PositionComp, 
    SpeedComp, 
    IdComp, 
    TypeComp, 
    TextureComp, 
    MeshComp,
    TransformableComp,
    CameraComp,
    CarComp,
    WheelComp
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

#endif