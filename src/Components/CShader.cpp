#include "CShader.h"

CShader::CShader(std::string vertex, std::string fragment)
: vertexShader(vertex), fragmentShader(fragment)
{
    m_compType = CompType::ShaderComp;
}