/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #include "CLResourceShader.h"

using namespace CLE;


bool CLResourceShader::LoadFile(string file, bool vertically) {
    
    return true;
}

bool CLResourceShader::LoadFile(string vertex, string fragment){
    if(!LoadShader(vertex,GL_VERTEX_SHADER))
        return false;
    if(!LoadShader(fragment,GL_FRAGMENT_SHADER))
        return false;
    if(!LinkShaders())
        return false;
    
    return true;
}

bool CLResourceShader::LoadFile(string vertex, string fragment, string geometry){
    if(!LoadShader(vertex,GL_VERTEX_SHADER))
        return false;
    if(!LoadShader(fragment,GL_FRAGMENT_SHADER))
        return false;
    if(!LoadShader(geometry,GL_GEOMETRY_SHADER))
        return false;
    if(!LinkShadersGeometry())
        return false;
    
    return true;
}

bool CLResourceShader::LoadShader(string file, GLenum type){
    string code;
    string typeStr;
    GLuint* shader = nullptr;

    if(type == GL_VERTEX_SHADER){
        shader = &vertexID;
        typeStr = "VERTEX";
    }else if(type == GL_FRAGMENT_SHADER){
        shader = &fragmentID;
        typeStr = "FRAGMENT";

    }else if(type == GL_GEOMETRY_SHADER){
        shader = &geometryID;
        typeStr = "GEOMETRY";
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
        //cout << "Pues no se han podido leer los archivos " << file << endl;
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
        //std::cout << "ERROR::SHADER::"<<typeStr<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
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
        //cout << "Ha petado el linkado de shaders de geometry:( --> " << infoLog << "\n";
        return false;
    }

    //Tecnicamente una vez linkados se pueden borrar los shaders
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID); 
    return true;
}

bool CLResourceShader::LinkShadersGeometry(){
    programID = glCreateProgram(); //Como siempre nos devuelve un identificador

    //Bueno aqui es obvio, los enlaza ambos al programID
    glAttachShader(programID, vertexID);
    glAttachShader(programID, geometryID);
    glAttachShader(programID, fragmentID);
    glLinkProgram(programID);

    
    int  success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        //cout << "Ha petado el linkado de shaders de geometry:( --> " << infoLog << "\n";
        return false;
    }

    //Tecnicamente una vez linkados se pueden borrar los shaders
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID); 
    glDeleteShader(geometryID); 
    return true;
}

void CLResourceShader::Draw(GLuint shaderID) {}