#version 450 core

// in vec2 TexCoords;
// out vec4 FragColor;

// uniform sampler2D image;
// uniform vec3 spriteColor;

// void main()
// {    
//     //FragColor = texture(image, TexCoords);
//     FragColor = vec4(1.0,0.0,0.5,1.0);
// }


out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}