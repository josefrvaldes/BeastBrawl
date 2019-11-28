#include "CTexture.h"

CTexture::CTexture(std::string path)
: m_texture(path) 
{
    m_compType = CompType::TextureComp;
}