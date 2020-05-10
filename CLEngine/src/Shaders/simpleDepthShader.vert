#version 450 core
/*layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
} */

layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceModel;

void main()
{
    gl_Position = lightSpaceModel * vec4(aPos, 1.0);
}  