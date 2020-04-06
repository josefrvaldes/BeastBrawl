#version 450 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}

//http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html