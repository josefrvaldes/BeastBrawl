#ifndef __COMPONENT_H__
#define __COMPONENT_H__

enum CompType { 
    Position, 
    Speed, 
    Id, 
    Type, 
    Texture, 
    Mesh,
    Transformable 
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