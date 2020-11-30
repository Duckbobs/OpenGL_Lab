#version 450 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 fLightIntensity;

struct LightInfo {
  vec4 Position; // Light position in eye coords.
  vec3 La;       // Ambient light intensity
  vec3 Ld;       // Diffuse light intensity
  vec3 Ls;       // Specular light intensity
};
uniform LightInfo Light;
struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shiness;    // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

uniform mat4 mview;
uniform mat3 nmat;
uniform mat4 mvp;

void main()
{
	vec4 pos = mview * vec4(vertexPosition, 1.0);
	vec3 N, L, V, R;
	N = normalize(nmat * vertexNormal);
	L = normalize(Light.Position - pos).xyz;
	V = normalize(-pos).xyz;
	R = normalize(reflect(-L, N));

	vec3 ambient = Light.La * Material.Ka;
	vec3 diffuse = Light.Ld * Material.Kd * max(dot(L, N),0);
	vec3 spec = Light.Ls * Material.Ks * pow(max(dot(R,V), 0.0), Material.Shiness);
	
	fLightIntensity = ambient + diffuse + spec;

	gl_Position = mvp * vec4(vertexPosition, 1.0f);;
}