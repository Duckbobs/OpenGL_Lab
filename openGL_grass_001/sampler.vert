#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 aTexCoord;

layout(std430, binding = 0) buffer ssbo1 // trans, rotate
{
	mat4 aInstanceMatrix[];
};
layout(std430, binding = 1) buffer ssbo2 // dqsMatrices
{
	mat2x4 dqs[];
};

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_viewProjection;
uniform float u_timer;

void main()
{
	mat4 model2  = aInstanceMatrix[gl_InstanceID];
	vec3 posMove = vec3(u_timer, 0, 0);
	// out
	Position = vec3(
		aPos.x+0.2f*(aPos.y*aPos.y),
		aPos.y,
		aPos.z
	);

    //Normal = vertexNormal;
	Normal =  vertexNormal;
	TexCoord = aTexCoord;
	gl_Position = u_viewProjection * model2 * vec4(Position, 1.0f);
}