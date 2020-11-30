#version 450 core

in vec3 fLightIntensity;
out vec4 FragColor;

void main()
{
	FragColor = vec4(fLightIntensity, 1);
} 