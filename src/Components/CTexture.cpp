#include "CTexture.h"

CTexture::CTexture(std::string path)
: texture(path) 
{
    m_compType = CompType::TextureComp;
}