#version 450 core

out vec4 FragColor;

in vec2 TexCoords; //Coordenadas de textura
in vec3 Normal;    //La normal ya reajustada con escalado
in vec3 FragPos;   //Posicion

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor; //Color del objeto
uniform vec3 lightColor;  //Color de la luz
uniform vec3 lightPos;    //Posición de la luz
uniform vec3 viewPos;     //Posición de la camara

uniform float attenuationValue; //Atenuación

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

  
uniform Material material;
uniform Light light; 

void main()
{
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos); //Vector entre nosotros y el punto del objeto
    vec3 reflectDir = reflect(-lightDir, norm);  //Angulo reflectado
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //Formula de la luz especular
    vec3 specular = light.specular * (spec * material.specular);  //Multiplicamos todo 

    // float distanceToLight = length(lightPos - FragPos);
    // float attenuation = 1.0 / (1.0 + attenuationValue * pow(distanceToLight,2)) ;

    // vec3 result = (ambient+attenuation*(diffuse + specular)) * objectColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}