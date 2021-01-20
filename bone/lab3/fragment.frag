#version 450 core 


out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 color;


uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int type;







void main()
{
	//FragColor = texture(ourTexture, TexCoord);
	


	////object color : corral color
	//{0.415f, 0.827f, 0.917

	//vec3 objectColor = vec3(0.415f, 0.827f, 0.917);
	
	vec3 objectColor = color;
//	if (type == 1)
//		objectColor = vec3(0.8f, 0.0f, 0.0f);
	////Ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	////Diffuse 
	////normalize both the normal and the resulting direction vector;
	
	
	vec3 lightDir = normalize(lightPos-FragPos);
	lightDir = normalize(vec3(-1,1,1));

	float diff = max(dot(lightDir,Normal), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 lightDir2 = -lightDir;
	float diff2 = max(dot(lightDir2,Normal), 0.0);
	vec3 diffuse2 = diff2 * lightColor;

	diffuse += diffuse2;

	////Specular
	/*
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor;
	*/



	vec3 result = (ambient  + diffuse) * objectColor;

	FragColor = vec4(result, 1.0);

}