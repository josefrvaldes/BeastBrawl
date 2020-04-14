#version 450 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 12) out;

uniform mat4 VPmatrix;
uniform mat4 MVP;
uniform float timeElapsedWing;
uniform vec3 posVec3;

smooth out vec2 vTexCoord;



void main(){
    //vec3 position = gl_in[0].gl_Position.xyz;
    vec3 position = vec3(0.0, 0.0, 0.0);
    float grassSize = 1.0;

    // Rotaciones de la plantas
	float PIover180 = 3.1415/180.0;
    vec3 orientationText[] = {
		vec3(1.0, 0.0, 0.0),
		vec3(float(cos(60.0*PIover180)), 0.0f, float(sin(60.0*PIover180))),
		vec3(float(cos(-60.0*PIover180)), 0.0f, float(sin(-60.0*PIover180)))
	};

    // Ajustes del viento
	float windForce = 10.0;
    float distanceRepeat = 20.0;                // periodo con el que se repite el giro
    vec3 windDirection = vec3(1.0, 0.0, 1.0);
	windDirection = normalize(windDirection);

    for(int i = 0; i < 3; i++){
        // calcular viento
        float windPower = 0.5f+sin( (posVec3.x*windDirection.x+posVec3.z*windDirection.y)/distanceRepeat + timeElapsedWing*windForce/5.0);
        
        // obtenemos el poder del viento dependiendo de la distancia a la que se encuentre
        //float windPower = 0.5f + sin( (posVec3.x*windDirection.x+posVec3.z*windDirection.y)/distanceRepeat + timeElapsedWing*windForce/20);


        // Esto va a hacer que se pliege mas en un sentido que en otro para que de sensación mas realista
		if(windPower < 0.0f)
			windPower = windPower*0.2f;
		else 
            windPower = windPower*0.3f;
		
		windPower *= windForce;


        // generar los 4 vertices
        vec3 vTL = position - orientationText[i]*grassSize*0.5f + windDirection*windPower/20.0;
        vTL.y += grassSize;  
        gl_Position = MVP * vec4(vTL, 1.0);
        vTexCoord = vec2(0.0,0.0);
        EmitVertex();

        vec3 vBL = position - orientationText[i]*grassSize*0.5f;
        gl_Position = MVP * vec4(vBL, 1.0);
        vTexCoord = vec2(0.0, 1.0);
        EmitVertex();

        vec3 vTR = position + orientationText[i]*grassSize*0.5f + windDirection*windPower/20.0;
        vTR.y += grassSize;  
        gl_Position = MVP * vec4(vTR, 1.0);
        vTexCoord = vec2(1.0, 0.0);
        EmitVertex();

        vec3 vBR = position + orientationText[i]*grassSize*0.5f;
        gl_Position = MVP* vec4(vBR, 1.0);
        vTexCoord = vec2(1.0,1.0);
        EmitVertex();



        EndPrimitive();
    }
}