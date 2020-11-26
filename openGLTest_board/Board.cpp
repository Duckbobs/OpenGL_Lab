#include "Board.h"

GLfloat positions[] = {
-0.5f, -0.5f, 0.5f, 1.0f,  //0
-0.5f, 0.5f, 0.5f, 1.0f, //1
0.5f, 0.5f, 0.5f, 1.0f, //2
0.5f, -0.5f, 0.5f, 1.0f, //3
-0.5f, -0.5f, -0.5f, 1.0f, //4
-0.5f, 0.5f, -0.5f, 1.0f, //5
0.5f, 0.5f, -0.5f, 1.0f, //6
0.5f, -0.5f, -0.5f, 1.0f //7
};
GLfloat positions_color[] = {
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
};

Board::Board() {
	Board::setup();
}
void Board::setup() {

	// 1. vao 생성
	glGenVertexArrays(1, &vaoHandle); // vao 1 개 생성
	glBindVertexArray(vaoHandle); // = 이 vao에 뭔가를  할거다..activate..picking..

	// 2. vbo 생성
	glGenBuffers(1, &vboVertex); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // 이 vbo를 선택한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // id 번호
		4, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		0, // 각 vertex 당 크기
		0 //  offset
	);
	glEnableVertexAttribArray(0); // id 2을 enable
	// 2. vbo 생성
	glGenBuffers(1, &vboVertex); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // 이 vbo를 선택한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions_color), &positions_color, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // id 번호
		3, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		0, // 각 vertex 당 크기
		0 //  offset
	);
	glEnableVertexAttribArray(1); // id 2을 enable

	// 3. vao close
	glBindVertexArray(0); // 인자를 0 넣으면 VAO close
}

void Board::draw() {
	glBindVertexArray(vaoHandle);
	int size;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(float), GL_FLOAT, 0);
}
