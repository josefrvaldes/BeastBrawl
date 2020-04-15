#version 450 core

smooth in vec2 vTexCoord;
uniform sampler2D texture1;

out vec4 FragColor;

void main()
{             
    vec4 texColor = texture(texture1, vTexCoord);
    if(texColor.a < 0.5)                            // Para eliminar la transparencia, valor normal 0.1
        discard;
    FragColor = texColor;
}