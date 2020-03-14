#version 450 core

out vec4 FragColor;

in vec2 TexCoords; //Coordenadas de textura
in vec3 Normal;    //La normal ya reajustada con escalado
in vec3 FragPos;   //Posicion

void main()
{
    FragColor = vec4(1.0,1.0,1.0,1.0);

}


