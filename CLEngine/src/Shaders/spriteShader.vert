#version 450 core

// layout (location = 0) in vec3 aPos; // <vec2 position, vec2 texCoords>

// out vec2 TexCoords;

// uniform mat4 model;
// uniform mat4 projection;
// uniform mat4 MVP;

// void main()
// {
//     //TexCoords = vertex.zw;
//     //gl_Position = MVP * vec4(vertex.xy, 0.0, 1.0);
//     // TexCoords = vertex.zw;
//     // gl_Position = vec4(vertex.xy, 1.0, 1.0);

//     gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

// }

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}