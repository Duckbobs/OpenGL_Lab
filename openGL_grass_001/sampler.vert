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
out vec4 worldPosition;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_viewProjection;
uniform vec3 u_windVector;

void main()
{
	mat4 model2  = aInstanceMatrix[gl_InstanceID];
	// out
	Position = vec3(
		aPos.x,
		aPos.y,
		aPos.z+0.2f*(aPos.y*aPos.y)
	);
    //Normal = vertexNormal;
	Normal =  vertexNormal;
	TexCoord = aTexCoord;
	worldPosition = model2 * vec4(Position * u_windVector, 1.0f);

	gl_Position = u_viewProjection * worldPosition;
}