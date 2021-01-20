
#version 440 core

in vec2 TexCoords;
in vec3 fragPos;
in vec3 fragNormal;

uniform vec4 LightPosition; 


const vec3 LightIntensity = vec3(1,1,1);
const vec3 Ka = vec3(0.3,0.3,0.3);
const vec3 Kd = vec3(0.7,0.7,0.7);



out vec4 color;


vec3 ads(vec4 position, vec3 norm)
{
   vec3 l = normalize(vec3(LightPosition-position));

   return LightIntensity * (Ka + Kd * max(dot(l,norm),0.0));
}


void main()
{    

    vec3 Color = ads(vec4(fragPos,1.0),fragNormal);

	color = vec4(Color,1.0);

}

