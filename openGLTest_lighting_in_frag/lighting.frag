#version 450 core

in vec3 Position;
in vec3 Normal;

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

out vec4 FragColor;

void main()
{
	vec4 pos = vec4(Position, 1);
	vec3 N, L, V, R, H;
	N = Normal;
	L = normalize(Light.Position - pos).xyz;
	V = normalize(-pos).xyz;
	H = normalize(V + L);
	//R = normalize(reflect(-L, N));

	vec3 ambient = Light.La * Material.Ka;
	vec3 diffuse = Light.Ld * Material.Kd * max(dot(L, N),0);
	vec3 spec = Light.Ls * Material.Ks * pow(max(dot(H,N), 0.0), Material.Shiness);
	
	vec3 fLightIntensity = ambient + diffuse + spec;
	FragColor = vec4(fLightIntensity, 0.2);
} 