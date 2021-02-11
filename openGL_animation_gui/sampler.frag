#version 450 core

in vec3 Position;
in vec3 Normal;

struct LightInfo {
  vec4 Position; // Light position in eye coords.
  vec3 Intensity;       // Ia = Id = Is
};
uniform LightInfo[5] Light;
struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shiness;    // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
	vec4 pos = vec4(Position, 1);
	vec3 N, L, V, R, H;
	vec3 ambient;
	vec3 diffuse;
	vec3 spec;
	vec3 fLightIntensity;
	V = normalize(-pos).xyz;
	N = Normal;

	for(int i=0; i<1; i++) {
		L = normalize(Light[i].Position - pos).xyz;
		H = normalize(V + L);

		ambient = Light[i].Intensity * Material.Ka;
		diffuse = Light[i].Intensity * Material.Kd * max(dot(L, N),0);
		spec = Light[i].Intensity * Material.Ks * pow(max(dot(H,N), 0.0), Material.Shiness);
	
		fLightIntensity += (diffuse + spec);
	}
	fLightIntensity = ambient + fLightIntensity;

    FragColor = texture2D(ourTexture, TexCoord) * vec4(fLightIntensity, 1);
	//FragColor = vec4(fLightIntensity, 1);
}

	//R = normalize(reflect(-L, N));