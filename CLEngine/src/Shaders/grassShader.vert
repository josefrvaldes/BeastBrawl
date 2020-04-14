#version 450 core

//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoord;
/*layout (location = 2) in vec3 aInstancedPosition; // posiciones para cada instancia que se le pasa
layout (location = 3) in mat4 aInstancedModel; // matriz modelo para cada instancia que se le pasa

out vec2 TexCoord;

uniform mat4 VPmatrix;

void main()
{
    TexCoord = aTexCoord;
    gl_Position = VPmatrix * aInstancedModel * vec4(aPos, 1.0);
    
}*/


layout (location = 2) in vec3 aPosition; // posiciones para cada instancia que se le pasa

uniform mat4 VPmatrix;
uniform mat4 MVP;

void main()
{
    gl_Position = vec4(aPosition, 1.0);
}