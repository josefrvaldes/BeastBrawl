#version 450 core

layout (location = 0) in vec3 aInstancedWorldPosition;  // posiciones para cada instancia que se le pasa
layout (location = 1) in mat4 aInstancedModel;          // matriz modelo para cada instancia que se le pasa

uniform mat4 VPmatrix;
out mat4 MVP;
out vec3 worldPositionPoint;

void main()
{
    MVP = VPmatrix * aInstancedModel;
    worldPositionPoint = aInstancedWorldPosition;
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);  // se le pasa la posicion 0.0 ya que es solo un punto en el centro de lo que seria la "malla"
}