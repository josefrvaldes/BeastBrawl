#include "CTexture.h"

CTexture::CTexture(std::string path){
    m_compType = CompType::Texture;
    m_texture  = path;
}