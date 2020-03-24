#version 450 core
// http://in2gpu.com/2014/06/23/toon-shading-effect-and-simple-contour-detection/
out vec4 FragColor;

in vec2 TexCoords; //Coordenadas de textura
in vec3 Normal;    //La normal ya reajustada con escalado
in vec3 FragPos;   //Posicion
uniform vec3 viewPos;     //Posición de la camara

uniform samplerCube depthMap;
uniform float far_plane;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D heigth;
    sampler2D normal;
    float shininess;
}; 
uniform Material material;

struct PointLight {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
  
    float constant;
    float linear;
    float quadratic;
};
uniform int num_Point_Lights;
#define NUM_POINT_LIGHTS 25
uniform PointLight pointLights[NUM_POINT_LIGHTS]; 
//uniform sampler2D texture_diffuse1;

uniform int cartoonParts = 8;
//const float scaleFactor = 1.0 / cartoonParts;


float ShadowCalculation(vec3 fragPos, vec3 posLight)
{
     // get vector between fragment position and light position
    vec3 fragToLight = fragPos - posLight;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    /*vec3 sampleOffsetDirections[20] = vec3[](
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );  

    float shadow = 0.0;
    float bias   = 0.15;
    int samples  = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;  
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);*/

    return shadow;
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int i){

    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;
    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal,lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 
    diffuse  = light.diffuse * texture(material.diffuse, TexCoords).rgb * floor(diff * cartoonParts) /  cartoonParts;
    vec3 H = normalize(lightDir + viewDir);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  //Angulo reflectado

    float spec = 0.0;
    vec3 specular = vec3(1.0,1.0,1.0);

    if( dot(lightDir,normal) > 0.0)
    {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //Formula de la luz especular
        specular = light.specular * spec * texture(material.specular,TexCoords).rgb;  //Multiplicamos todo 
    }
  // attenuation
    float distance    = length(light.position - fragPos); //Distancia de la luz al objeto
    float attenuation = 1.0 / ((light.constant) + (light.linear * distance) + (light.quadratic * (distance * distance))); //Formula de la atenuacion

    //limit specular
    float specMask = (pow(dot(H, normal), material.shininess) > 0.4) ? 1 : 0;
    float edgeDetection = (dot(viewDir, normal) > 0.2) ? 1 : 0;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular*= attenuation;
    
    float shadow = 0.0;
    /*if(i == 0)
        shadow = ShadowCalculation(FragPos, light.position); */
    return edgeDetection * (ambient + (1.0 - shadow) * (diffuse /*+ specular*specMask*/));
    //return edgeDetection * (ambient + diffuse /*+ specular*specMask*/);
} 

void main(){

    vec3 totalPointLight = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos); //Vector entre nosotros y el punto del objeto
    // phase 2: Point lights
    
    int i = 0;
    while(i<num_Point_Lights){
        totalPointLight += CalcPointLight(pointLights[i], norm, FragPos, viewDir, i); 
        i++;
    }

    FragColor = vec4(totalPointLight,1.0);
    //FragColor = floor(FragColor * cartoonParts) / cartoonParts;  // estaba mal aplicado, era en la luz difusa solo

    //Si comentas esta linea se ve con luces
    FragColor = texture(material.diffuse,TexCoords);
}