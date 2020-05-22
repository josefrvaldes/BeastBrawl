#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;


uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 MVP;
uniform mat4 lightSpaceMatrix;


void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    //gl_Position = MVP * vec4(aPos,1.0);
    FragPos = vec3(model * vec4(aPos,1.0));
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    // Hacemos esto para ajustar las normales en caso de escalar las mallas
    //Source: http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
    //TODO: Cambiar este calculo a la CPU ya que inversas por cada vertice es muy costoso
    Normal = mat3(transpose(inverse(model))) * aNormal; 
    TexCoords = aTexCoord;
}