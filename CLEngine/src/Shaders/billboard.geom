#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

uniform mat4 VPMatrix;
uniform vec3 cameraPosition;

out vec2 TexCoords;


void main()
{
    
    vec3 p = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(cameraPosition - p);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);

    p -= (0.5 * right);
    gl_Position = VPMatrix * vec4(p, 1.0);
    TexCoords = vec2(1.0, 1.0);
    EmitVertex();

    p.y += 1.0;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TexCoords = vec2(1.0, 0.0);
    EmitVertex();

    p.y -= 1.0;
    p += right;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TexCoords = vec2(0.0, 1.0);
    EmitVertex();

    p.y += 1.0;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TexCoords = vec2(0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}