#version 450 core

layout(location=0) in vec3 VertexPosition;
uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(VertexPosition, 1.0);
}