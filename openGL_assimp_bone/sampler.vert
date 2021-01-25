#version 450 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in ivec4 BoneIDs;
layout(location = 3) in vec4 Weights;

out vec3 Position;
out vec3 Normal;

uniform mat3 nmat;
uniform mat4 mvp;

uniform mat4 gBones[150];

void main()
{
	mat4 BoneTransform = mat4(1.0);
	
	BoneTransform = gBones[BoneIDs[0]] * Weights[0];
	BoneTransform += gBones[BoneIDs[1]] * Weights[1];
	BoneTransform += gBones[BoneIDs[2]] * Weights[2];
	BoneTransform += gBones[BoneIDs[3]] * Weights[3];
	
	Position = vec3(BoneTransform * vec4(vertexPosition, 1.0));
	Normal = normalize(nmat * vertexNormal);
	gl_Position = mvp * BoneTransform * vec4(vertexPosition, 1.0);
}