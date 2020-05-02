#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 animationOffsetPos;
layout (location = 4) in vec3 animationOffsetNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 MVP;


// A = (0,0) kf1
// A = (1,1)
// A = (2,2)
// A = (3,3)
//     .
//     .
//     .
// A = (10,10) kf2

void main()
{
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 newPos = aPos + animationOffsetPos;
    vec3 newNormal = aNormal + animationOffsetNormal;
    gl_Position = projection*view*model * vec4(newPos,1.0);
    FragPos = vec3(model * vec4(newPos,1.0));

    // Hacemos esto para ajustar las normales en caso de escalar las mallas
    //Source: http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
    //TODO: Cambiar este calculo a la CPU ya que inversas por cada vertice es muy costoso
    Normal = mat3(transpose(inverse(model))) * newNormal; 
    TexCoords = aTexCoord;
}