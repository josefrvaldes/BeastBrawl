#version 450 core
// http://in2gpu.com/2014/06/23/toon-shading-effect-and-simple-contour-detection/
out vec4 FragColor;

in vec2 TexCoords; //Coordenadas de textura
in vec3 Normal;    //La normal ya reajustada con escalado
in vec3 FragPos;   //Posicion
uniform vec3 viewPos;     //Posición de la camara

//uniform samplerCube depthMap;
//uniform float far_plane;
uniform bool activeShadows;
uniform vec3 lightShadowDir;
uniform sampler2D depthMap;
in vec4 FragPosLightSpace;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D heigth;
    sampler2D normal;
    float shininess;
}; 
uniform Material material;

//Luces puntuales
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

//Luces dirigidas
struct DirectLight {
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
  
    float constant;
    float linear;
    float quadratic;
};
uniform int num_Direct_Lights;
#define NUM_DIRECT_LIGHTS 25
uniform DirectLight directLights[NUM_DIRECT_LIGHTS]; 

//Luces spot
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
  
    float constant;
    float linear;
    float quadratic;
};
uniform int num_Spot_Lights;
#define NUM_SPOT_LIGHTS 25
uniform SpotLight spotLights[NUM_SPOT_LIGHTS]; 

uniform int cartoonParts = 8;
//const float scaleFactor = 1.0 / cartoonParts;

uniform int id_luz_shadowMapping; //El numero en el array  de luces puntuales para aplicar shadowmapping

bool LUZ_ON = true;        //A 0 desactiva las luces

float ShadowCalculation(){
    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    /*float closestDepth = texture(depthMap, projCoords.xy).r;*/ 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(Normal, lightShadowDir)), 0.001); 
    //float bias = 0.001; 
    /*float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;*/  

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 6.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirectLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results

	    vec4 texColor = texture(material.diffuse, TexCoords);
    if(texColor.a < 0.5)                            // Para eliminar la transparencia, valor normal 0.1
        discard;

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

     // attenuation
    float distance    = length(light.position - fragPos); //Distancia de la luz al objeto
    float attenuation = 1.0 / ((light.constant) + (light.linear * distance) + (light.quadratic * (distance * distance))); //Formula de la atenuacion


    ambient *= attenuation;
    diffuse *= attenuation;
    specular*= attenuation;

    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int i){

    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;
    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal,lightDir), 0.0);
		    vec4 texColor = texture(material.diffuse, TexCoords);
    if(texColor.a < 0.5)                            // Para eliminar la transparencia, valor normal 0.1
        discard;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 
    /*diffuse  = light.diffuse * texture(material.diffuse, TexCoords).rgb * floor(diff * cartoonParts) /  cartoonParts;*/
    vec3 H = normalize(lightDir + viewDir);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  //Angulo reflectado

    float spec = 0.0;
    vec3 specular = vec3(1.0,1.0,1.0);

    if( dot(lightDir,normal) > 0.0)
    {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 0/*material.shininess*/); //Formula de la luz especular
        specular = light.specular * spec * texture(material.specular,TexCoords).rgb;  //Multiplicamos todo 
    }
  // attenuation
    float distance    = length(light.position - fragPos); //Distancia de la luz al objeto
    float attenuation = 1.0 / ((light.constant) + (light.linear * distance) + (light.quadratic * (distance * distance))); //Formula de la atenuacion

    //limit specular
    float specMask = (pow(dot(H, normal), 0/*material.shininess*/) > 0.4) ? 1 : 0;
    float edgeDetection = (dot(viewDir, normal) > 0.2) ? 1 : 0;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular*= attenuation;
    

    //PARA DESACTIVAR EL CARTOON DESCOMENTAR ESTA LINEA
    edgeDetection = 1.0;
    float shadow = 0.0;
    if(activeShadows && i == id_luz_shadowMapping)
        shadow = ShadowCalculation();

    return edgeDetection * (ambient + (1.0 - shadow) * (diffuse /*+ specular*specMask*/));
    //return edgeDetection * (ambient + diffuse /*+ specular*specMask*/);
} 


// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
		    vec4 texColor = texture(material.diffuse, TexCoords);
    if(texColor.a < 0.5)                            // Para eliminar la transparencia, valor normal 0.1
        discard;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main(){

    if(activeShadows){
        vec3 result = vec3(0.0);

        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - FragPos); //Vector entre nosotros y el punto del objeto

        // Luces direccionales
        int j = 0;
        while(j<num_Direct_Lights){
            result += CalcDirLight(directLights[j],norm,FragPos,viewDir);
            j++;
        }

        // Luces puntuales
        int i = 0;
        while(i<num_Point_Lights){
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, i); 
            i++;
        }

        //Luces spot
        int k = 0;
        while(k<num_Spot_Lights){
            result += CalcSpotLight(spotLights[k],norm, FragPos, viewDir);
            k++;
        }

        FragColor = vec4(result,1.0);
            //FragColor = floor(FragColor * cartoonParts) / cartoonParts;  // estaba mal aplicado, era en la luz difusa solo

        vec4 texColor = texture(material.diffuse, TexCoords);

        if(texColor.a < 0.5)                            // Para eliminar la transparencia, valor normal 0.1
            discard;
        
    }else{
        FragColor = texture(material.diffuse,TexCoords);

        if(FragColor.a < 0.5)                            // Para eliminar la transparencia, valor normal 0.1
            discard;
    }
    
    

    
}