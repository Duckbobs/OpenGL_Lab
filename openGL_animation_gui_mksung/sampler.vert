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
	unsigned int _offset = dqsize * gl_InstanceID;

	mat2x4 dq0 = dqs[_offset + BoneIDs[0]];
	mat2x4 dq1 = dqs[_offset + BoneIDs[1]];
	mat2x4 dq2 = dqs[_offset + BoneIDs[2]];
	mat2x4 dq3 = dqs[_offset + BoneIDs[3]];
	if (dot(dq0[0], dq1[0]) < 0.0) dq1 *= -1.0;
	if (dot(dq0[0], dq2[0]) < 0.0) dq2 *= -1.0;
	if (dot(dq0[0], dq3[0]) < 0.0) dq3 *= -1.0;
	mat2x4 blendDQ = dq0 * Weights[0];
	blendDQ += dq1 * Weights[1];
	blendDQ += dq2 * Weights[2];
	blendDQ += dq3 * Weights[3];

	float len = length(blendDQ[0]);
	blendDQ /= len;
	vec3 trans = 2.0*(blendDQ[0].w * blendDQ[1].xyz - blendDQ[1].w * blendDQ[0].xyz + cross(blendDQ[0].xyz, blendDQ[1].xyz));

	vec3 position = aPos.xyz + 2.0 * cross(blendDQ[0].xyz, cross(blendDQ[0].xyz, aPos.xyz) + blendDQ[0].w * aPos.xyz);
	position += trans;
	
	mat4 model2  = aInstanceMatrix[gl_InstanceID];
	//mat4 modelview = view * model2;
	//Normal = mat3(transpose(inverse(DQmat))) * vertexNormal;

	// out
	Position = vec3(model2 * vec4(position, 1.0f));
    Normal = vertexNormal;
	//Normal = mat3(transpose((model2))) * vertexNormal;
	TexCoord = aTexCoord;
	gl_Position = viewProjection * model2 * vec4(position, 1.0f);
}