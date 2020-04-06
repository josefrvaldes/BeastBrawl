#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

uniform mat4 VPMatrix;
uniform vec3 cameraPosition;
uniform float width;
uniform float height;

out vec2 TexCoords;


void main()
{
    vec3 position = gl_in[0].gl_Position.xyz;
    vec3 to_camera = normalize(cameraPosition - position);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(to_camera, up);

    position -= (0.5 * right * width);
    gl_Position = VPMatrix * vec4(position, 1.0);
    TexCoords = vec2(1.0,1.0);
    EmitVertex();

    position.y += height;
    gl_Position = VPMatrix * vec4(position, 1.0);
    TexCoords = vec2(1.0, 0.0);
    EmitVertex();

    position.y -= height;
    position += right * width;
    gl_Position = VPMatrix * vec4(position, 1.0);
    TexCoords = vec2(0.0, 1.0);
    EmitVertex();

    position.y += height;
    gl_Position = VPMatrix* vec4(position, 1.0);
    TexCoords = vec2(0.0,0.0);
    EmitVertex();

    EndPrimitive();
}

//http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html