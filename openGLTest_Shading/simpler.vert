#version 450 core

// x, y, z, w = vec4
layout(location = 0) in vec4 vertexPosition; // id 0
// R, G, B = vec3
layout(location = 1) in vec3 vertexColor; // id 1
//cpp 에 있는 id와 값이 매치돼야한다.

uniform mat4 model; // 유니폼 app에서 전송 받은 것..
// 유니폼은 오직 read만 가능


//out vec3 color;
out vec3 fColor; // 색은 frag에서 해야함. 전달 역할. = 프레그먼트 쉐이더로 보낼 데이타
void main()
{
	//color = vec3(1, 0, 0);
	fColor = vertexColor;

	//삼각형을 옮겨보자.. (0.2, 0.2) 만큼 : translation 행렬
	//mat4 model = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0.2f, 0.2f, 0, 1} };
	
	
	// gl_Position(vec4) 설정 (NDC 좌표계)
	// vertexPosition : local, model, object 좌표값
	// gl_Position = NDC, Clipped 좌표값,
	// 여기에 값을 설정하는 순간 perspective division 실행...
	gl_Position = model * vertexPosition;


}