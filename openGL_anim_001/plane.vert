#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 viewProjection;

void main()
{
	// out
	Position = aPos;
    Normal = vertexNormal;
	gl_Position = viewProjection * vec4(aPos, 1.0f);
}