#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in ivec4 BoneIDs;
layout(location = 3) in vec4 Weights;
layout(location = 4) in vec2 aTexCoord;

layout(std430, binding = 0) buffer ssbo1  //for specifying joints : full
 {
	mat4 aInstanceMatrix[];
 };

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 viewProjection;
uniform mat2x4 dqs[150];

mat4x4 DQtoMat(vec4 real, vec4 dual) {
	mat4x4 m;
	float len2 = dot(real, real);
	float w = real.w, x = real.x, y = real.y, z = real.z;
	float t0 = dual.w, t1 = dual.x, t2 = dual.y, t3 = dual.z;

	m[0][0] = w * w + x * x - y * y - z * z;
	m[1][0] = 2 * x * y - 2 * w * z;
	m[2][0] = 2 * x * z + 2 * w * y;
	m[0][1] = 2 * x * y + 2 * w * z;
	m[1][1] = w * w + y * y - x * x - z * z;
	m[2][1] = 2 * y * z - 2 * w * x;
	m[0][2] = 2 * x * z - 2 * w * y;
	m[1][2] = 2 * y * z + 2 * w * x;
	m[2][2] = w * w + z * z - x * x - y * y;

	m[3][0] = -2 * t0 * x + 2 * w * t1 - 2 * t2 * z + 2 * y * t3;
	m[3][1] = -2 * t0 * y + 2 * t1 * z - 2 * x * t3 + 2 * w * t2;
	m[3][2] = -2 * t0 * z + 2 * x * t2 + 2 * w * t3 - 2 * t1 * y;

	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = len2;
	m /= len2;

	return m;
}
void main()
{
	mat2x4 dq0 = dqs[BoneIDs[0]];
	mat2x4 dq1 = dqs[BoneIDs[1]];
	mat2x4 dq2 = dqs[BoneIDs[2]];
	mat2x4 dq3 = dqs[BoneIDs[3]];
	if (dot(dq0[0], dq1[0]) < 0.0) dq1 *= -1.0;
	if (dot(dq0[0], dq2[0]) < 0.0) dq2 *= -1.0;
	if (dot(dq0[0], dq3[0]) < 0.0) dq3 *= -1.0;
	mat2x4 blendDQ = dq0 * Weights[0];
	blendDQ += dq1 * Weights[1];
	blendDQ += dq2 * Weights[2];
	blendDQ += dq3 * Weights[3];
	mat4 DQmat = DQtoMat(blendDQ[0], blendDQ[1]);
	mat4 model2  = aInstanceMatrix[gl_InstanceID];
	
	float len = length(blendDQ[0]);
	blendDQ /= len;
	vec3 position = aPos.xyz + 2.0*cross(blendDQ[0].xyz, cross(blendDQ[0].xyz, aPos.xyz) + blendDQ[0].w * aPos.xyz);
	vec3 trans = 2.0*(blendDQ[0].w * blendDQ[1].xyz - blendDQ[1].w * blendDQ[0].xyz + cross(blendDQ[0].xyz, blendDQ[1].xyz));
	position += trans;

	//Position = vec3(vec4(aPos, 1.0));
	//Normal = normalize(nmat * vertexNormal);
	TexCoord = aTexCoord;
	Position = vec3(model2* vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(DQmat))) * vertexNormal;
	gl_Position = viewProjection * model2 * vec4(position, 1.0f);
}