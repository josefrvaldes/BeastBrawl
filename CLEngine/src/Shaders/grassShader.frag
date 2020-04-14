#version 450 core

/*out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{             
    vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.5)
        discard;
    FragColor = texColor;
}*/

smooth in vec2 vTexCoord;

out vec4 FragColor;


uniform sampler2D texture1;

void main()
{             
    vec4 texColor = texture(texture1, vTexCoord);
    if(texColor.a < 0.5)
        discard;
    FragColor = texColor;
}