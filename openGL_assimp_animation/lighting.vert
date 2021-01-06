#version 450 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texCoord;


out vec3 Position;
out vec3 Normal;

uniform mat4 mview;
uniform mat3 nmat;
uniform mat4 mvp;

void main()
{
	Position = vec3(mview * vec4(vertexPosition,1.0));
	Normal = normalize(nmat * vertexNormal);
	gl_Position = mvp * vec4(vertexPosition, 1.0f);;
}