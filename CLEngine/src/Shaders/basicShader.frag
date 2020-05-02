#version 450 core
out vec4 FragColor;

in vec2 TexCoords; //Coordenadas de textura
in vec3 Normal;    //La normal ya reajustada con escalado
in vec3 FragPos;   //Posicion
uniform vec3 viewPos;     //Posición de la camara



struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D heigth;
    sampler2D normal;
    float shininess;
}; 
uniform Material material;


void main(){


    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos); //Vector entre nosotros y el punto del objeto

    //Si comentas esta linea se ve con luces
    FragColor = texture(material.diffuse,TexCoords);
}