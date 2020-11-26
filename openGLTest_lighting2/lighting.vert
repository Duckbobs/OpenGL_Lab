#version 450 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 fLightIntensity;

uniform vec4 lightLocation;
uniform vec3 Kd;
uniform vec3 Ld;

uniform mat4 mview;
uniform mat3 nmat;
uniform mat4 mvp;
void main()
{
	vec4 pos = mview * vec4(vertexPosition, 1.0);
	vec3 N = normalize(nmat * vertexNormal);
	vec4 P = mview * vec4(vertexPosition, 1.0);
	vec3 L = normalize(lightLocation - pos).xyz;
	fLightIntensity = max(Kd * Ld * dot(L, N), 0);
	gl_Position = mvp * vec4(vertexPosition, 1.0f);;
}