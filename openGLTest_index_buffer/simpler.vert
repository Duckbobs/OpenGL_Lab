#version 450 core
layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 fColor;
void main()
{
	fColor = vertexColor;
	gl_Position = vertexPosition;
}