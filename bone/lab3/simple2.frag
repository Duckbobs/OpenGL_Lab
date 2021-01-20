#version 330



out vec4 col;
in vec3 nrm;
in vec3 color;



void main()
{
  //col = vec4(1,0,0,1.0);
  col = vec4(nrm,1.0);

}
