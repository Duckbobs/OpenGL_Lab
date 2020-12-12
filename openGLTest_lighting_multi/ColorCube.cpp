#include "ColorCube.h"

GLfloat cube_vertices[] = {
	// front
	-1.0, -1.0, 1.0,
	1.0, -1.0, 1.0,
	1.0, 1.0, 1.0,
	-1.0, 1.0, 1.0,
	// back
	-1.0, -1.0, -1.0,
	1.0, -1.0, -1.0,
	1.0, 1.0, -1.0,
	-1.0, 1.0, -1.0,
};
GLfloat cube_colors[] = {
	// front colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	// back colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
};
GLushort cube_elements[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

ColorCube::ColorCube() {
	ColorCube::setup();
}
void ColorCube::setup() {

	// 1. vao 생성
	glGenVertexArrays(1, &vaoHandle); // vao 1 개 생성
	glBindVertexArray(vaoHandle); // = 이 vao에 뭔가를  할거다..activate..picking..

	// 2. vbo 생성
	glGenBuffers(1, &vboVertex); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // 이 vbo를 선택한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // id 번호
		3, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		0, // 각 vertex 당 크기
		0 //  offset
	);
	glEnableVertexAttribArray(0); // id 0을 enable
	// 2. vbo 생성
	glGenBuffers(1, &vbo_cube_colors); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors); // 이 vbo를 선택한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // id 번호
		3, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		0, // 각 vertex 당 크기
		0 //  offset
	);
	glEnableVertexAttribArray(1); // id 1을 enable
	// 2. ibo 생성
	glGenBuffers(1, &ibo_cube_elements); // ibo 1개 생성
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements); // 이 vbo를 선택한다.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(1); // id 1을 enable

// 3. vao close
	glBindVertexArray(0); // 인자를 0 넣으면 VAO close
}

void ColorCube::draw() {
	glBindVertexArray(vaoHandle);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}
