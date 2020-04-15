#version 450 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 12) out;

in mat4 MVP[];
in vec3 worldPositionPoint[];

uniform float timeElapsedWing;

smooth out vec2 vTexCoord;


void main(){
    vec3 position = gl_in[0].gl_Position.xyz;  // va a ser 0.0 ya que solo hay un punto en la "malla"
    float grassSize = 1.0;                     // el tamanyo ya lo trae la matriz MVP

    // Rotaciones de la plantas
	float PI180 = 3.1415/180.0;
    vec3 orientationTexture[] = {
		vec3(1.0, 0.0, 0.0),
		vec3(float(cos(60.0*PI180)), 0.0f, float(sin(60.0*PI180))),
		vec3(float(cos(-60.0*PI180)), 0.0f, float(sin(-60.0*PI180)))
	};

    // Ajustes del viento
	float windForce = 10.0;
    float distanceRepeat = 20.0;                // periodo con el que se repite el giro
    vec3 windDirection = vec3(1.0, 0.0, 1.0);
	windDirection = normalize(windDirection);

    for(int i = 0; i < 3; i++){
        // calcular viento, obtenemos el poder del viento dependiendo de la distancia a la que se encuentre
        float windPower = 0.5f+sin( (worldPositionPoint[0].x*windDirection.x+worldPositionPoint[0].z*windDirection.y)/distanceRepeat + timeElapsedWing*windForce/5.0);
        //float windPower = 0.5f + sin( (posVec3.x*windDirection.x+posVec3.z*windDirection.y)/distanceRepeat + timeElapsedWing*windForce/20);

        // Esto va a hacer que se pliege mas en un sentido que en otro para que de sensación mas realista
		if(windPower < 0.0f)
			windPower = windPower*0.2f;
		else 
            windPower = windPower*0.3f;
		
		windPower *= windForce;


        // generar los 4 vertices
        vec3 vertTL = position - orientationTexture[i]*grassSize*0.5f + windDirection*windPower/20.0;
        vertTL.y += grassSize*0.9;                                          // se multiplica por 0.9 para que se achate un poco y no se deforme mucho
        gl_Position = MVP[0] * vec4(vertTL, 1.0);
        vTexCoord = vec2(0.0,0.0);
        EmitVertex();

        vec3 vertBL = position - orientationTexture[i]*grassSize*0.5f;
        gl_Position = MVP[0] * vec4(vertBL, 1.0);
        vTexCoord = vec2(0.0, 1.0);
        EmitVertex();

        vec3 vertTR = position + orientationTexture[i]*grassSize*0.5f + windDirection*windPower/20.0;
        vertTR.y += grassSize*0.9;  
        gl_Position = MVP[0] * vec4(vertTR, 1.0);
        vTexCoord = vec2(1.0, 0.0);
        EmitVertex();

        vec3 vertBR = position + orientationTexture[i]*grassSize*0.5f;
        gl_Position = MVP[0] * vec4(vertBR, 1.0);
        vTexCoord = vec2(1.0,1.0);
        EmitVertex();


        EndPrimitive();
    }
}