#version 450 core

in vec3 fLightIntensity;

void main()
{
  gl_FragColor = vec4(fLightIntensity, 1);
} 