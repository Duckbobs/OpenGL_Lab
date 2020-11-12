#version 450 core

//in vec3 color;
//out vec4 fragColor;

in vec3 fColor;
out vec4 fragColor;
// 가장 중요한 일은 최소 하나의 vec4를 가진 out를 생성
void main()
{
	//fragColor = vec4(1, 0, 1, 1);
	//fragColor = vec4(color, 1);
	fragColor = vec4(fColor, 1.0f); // 1.0f 알파값
}