#version 450 core

out vec4 FragColor;
uniform vec4 clcolor;

in vec3 Color;
void main()
{
    FragColor = vec4(1.0,0.0,0.5,1.0);
    FragColor = vec4(Color,1.0);

    // if(clcolor.r == 0){
    //     FragColor = vec4(1.0,1.0,0.0,1.0);
    // }else{
    //     FragColor = vec4(1.0,1.0,1.0,1.0);

    // }
}


