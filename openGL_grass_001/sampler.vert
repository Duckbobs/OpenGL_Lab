#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

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

//uniform mat4 aInstanceMatrix;
uniform mat4 viewProjection;
//uniform int offset;
//uniform int ins;
uniform unsigned int dqsize;


void main()
{
	mat4 model2  = aInstanceMatrix[gl_InstanceID];
	//mat4 modelview = view * model2;
	//Normal = mat3(transpose(inverse(DQmat))) * vertexNormal;

	// out
	Position = aPos.xyz;
    //Normal = vertexNormal;
	Normal =  vertexNormal;
	TexCoord = aTexCoord;
	gl_Position = viewProjection * model2 * vec4(aPos.xyz, 1.0f);
}