#version 450 core

uniform sampler2D ourTexture;
uniform float alpha;
in vec2 TexCoords;
out vec4 FragColor;

void main()
{

    FragColor = texture2D(ourTexture, TexCoords);


    

    //We discard this kind of fragment to achieve transparency in our billboards
    if(FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0)
    {
        discard;
    }else{
        if(alpha<=1.0){
            float trasp = max(FragColor.a - alpha,0.0);
            FragColor.a = trasp;
        }
    }

    if(FragColor.a < 0.01){
        discard;
    }


    // if(FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0)
    // {
    //     discard;
    // }

    // if(FragColor.a < 0.01){
    //     discard;
    // }
    
}

//http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html