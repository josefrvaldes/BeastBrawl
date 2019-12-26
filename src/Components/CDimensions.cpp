#include "CDimensions.h"

CDimensions::CDimensions(float _width,float _height, float _depth)
: width(_width), height(_height), depth(_depth)
{
    m_compType = CompType::DimensionsComp;
}
