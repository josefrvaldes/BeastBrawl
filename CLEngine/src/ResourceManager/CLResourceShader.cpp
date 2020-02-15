#include "CLResourceShader.h"

using namespace CLE;

//Ya no se usa este 
bool CLResourceShader::LoadFile(string file) {
    
    return true;
}

bool CLResourceShader::LoadFile(string file1, string file2){
    if(!LoadShader(file1,GL_VERTEX_SHADER))
        return false;
    if(!LoadShader(file2,GL_FRAGMENT_SHADER))
        return false;
    if(!LinkShaders())
        return false;
    
    return true;
}

bool CLResourceShader::LoadShader(string file, GLenum type){
    string code;
    GLuint* shader = nullptr;

    if(type == GL_VERTEX_SHADER){
        shader = &vertexID;
    }else if(type == GL_FRAGMENT_SHADER){
        shader = &fragmentID;
    }
    *shader = glCreateShader(type); //Creamos el shader y nos guardamos su ID

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
    glShaderSource(*shader, 1, &codeptr, NULL); //(ID,1,puntero al array de char de codigo, NULL)
    glCompileShader(*shader);

    //Comprobamos si ha compilado correctamente
    int  success;
    char infoLog[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

bool CLResourceShader::LinkShaders(){
    programID = glCreateProgram(); //Como siempre nos devuelve un identificador

    //Bueno aqui es obvio, los enlaza ambos al programID
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    glLinkProgram(programID);

    
    int  success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        cout << "Ha petado el linkado de shaders :(\n";
        return false;
    }

    //Tecnicamente una vez linkados se pueden borrar los shaders
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID); 
    return true;
}

void CLResourceShader::Draw(glm::mat4 m) {}