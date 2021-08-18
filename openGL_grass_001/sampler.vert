#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 aTexCoord;

layout(binding = 0) buffer ssbo1 // trans, rotate
{
	mat4 aInstanceMatrix[];
};
layout(binding = 1) buffer ssbo2 // windVector
{
	float fdata[]; // vec3 에서 float 배열로 변경 2021-08-02
};

out vec3 Position;
out vec4 worldPosition;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_viewProjection;

void main()
{
	int f = gl_InstanceID*3;
	mat4 model2 = aInstanceMatrix[gl_InstanceID];
	vec3 wind = vec3(fdata[f++], fdata[f++], fdata[f++]);
	float powY = aPos.y*aPos.y;
	// out
	Position = vec3(
		aPos.x,
		aPos.y-0.05f*powY,
		aPos.z+0.2f*powY
	);
    //Normal = vertexNormal;
	Normal =  vertexNormal;
	TexCoord = aTexCoord;

	//vec4 modelPos = model2 * vec4(Position, 1.0f);
	//worldPosition = length(modelPos)*normalize(modelPos + vec4(wind.x*powY, -wind.x*powY, wind.z*powY, 0));
	
	float len = length(wind);
	worldPosition = model2 * vec4(Position, 1.0f) + vec4(wind.x*powY, -len*powY, wind.z*powY, 0);
	
	gl_Position = u_viewProjection * worldPosition;
}