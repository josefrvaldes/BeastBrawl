#version 450 core

out vec4 FragColor;
uniform vec4 clcolor;

in vec3 Color;
void main()
{
    FragColor = vec4(clcolor);

    
}


