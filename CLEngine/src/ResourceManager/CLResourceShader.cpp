#include "CLResourceShader.h"

using namespace CLE;

bool CLResourceShader::LoadFile(string file) {
    string code;

    shaderID = glCreateShader(shaderType); //Creamos el shader y nos guardamos su ID

    //Vamos a leer el fichero
    ifstream stream(file,ios::in);
    if(stream.is_open()){
        stringstream ss;
        ss << stream.rdbuf();
        code = ss.str();
        stream.close();
    }else{
        cout << "Pues no se han podido leer los archivos " << file << endl;
        return false;
    }

    char const* codeptr = code.c_str();

    // Vamos a intentar compilar los dichosos shaders
    glShaderSource(shaderID, 1, &codeptr, NULL); //(ID,1,puntero al array de char de codigo, NULL)
    glCompileShader(shaderID);

    //Comprobamos si ha compilado correctamente
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

void CLResourceShader::Draw(glm::mat4 m) {}