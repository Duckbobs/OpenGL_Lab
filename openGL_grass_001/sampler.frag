#version 450 core

in vec3 Position;
in vec3 Normal;

struct LightInfo {
  vec4 Position; // Light position in eye coords.
  vec3 Intensity;       // Ia = Id = Is
};
uniform LightInfo[1] Light;
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
	vec3 fLightIntensity;
	vec3 N, L, V, H;
	vec3 ambient;
	vec3 diffuse;
	vec3 spec;
	

	for(int i=0; i<0; i++) {
		N = Normal;
		L = normalize(Light[i].Position - pos).xyz;
		V = normalize(-pos).xyz;
		H = normalize(V + L);

		ambient = Light[i].Intensity * Material.Ka;
		diffuse = Light[i].Intensity * Material.Kd * max(dot(L, N),0);
		spec = Light[i].Intensity * Material.Ks * pow(max(dot(H, N), 0.0), Material.Shiness);
	
		//float dist = length(Position - Light[i].Position.xyz);
		//float attenuation = 1.0 / (1.0f + 0.0014f * dist + 0.000007f * (dist * dist));
		//ambient  *= attenuation;
		//diffuse  *= attenuation;
		//spec *= attenuation;

		fLightIntensity += (diffuse + spec);
	}

	fLightIntensity = ambient + fLightIntensity;

	//vec4 _texColor = texture2D(ourTexture, TexCoord);
    //if(_texColor.a < 0.5)
    //    discard;
    //FragColor = _texColor;

    FragColor = vec4(0f, 0.5f+Position.x*0.5f, 0f, 1.0f);
}