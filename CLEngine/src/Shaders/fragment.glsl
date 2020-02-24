#version 450 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float attenuationValue;

struct Material {
    vec3 ambient;
    vec3 diffuse;
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
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

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