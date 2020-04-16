#version 450 core

uniform sampler2D ourTexture;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{

    FragColor = texture2D(ourTexture, TexCoords);

    //We discard this kind of fragment to achieve transparency in our billboards
    if(FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0 || FragColor.a < 0.1)
    {
        discard;
    }

}

//http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html