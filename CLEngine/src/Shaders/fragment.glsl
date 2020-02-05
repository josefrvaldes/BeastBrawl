 #version 450 core

out vec4 FragColor;

//uniform vec4 ourColor;    // Variable de entrada del vertex shader
in vec3 ourColor;

void main() 
{ 
    FragColor = vec4(ourColor, 1.0); 
};