#include "MyGlWindow.h"
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp> // translate()
#include <glm/gtc/type_ptr.hpp> // value_ptr()

#include <glm/gtx/string_cast.hpp> // to_string()

MyGlWindow::MyGlWindow(int w, int h)
{
	width = w;
	height = h;

	setupBuffer(); // 단 한번 호출
}
void MyGlWindow::resize(int w, int h) {
	width = w;
	height = h;
}
void MyGlWindow::setSize(int w, int h) {
	width = w;
	height = h;
}
glm::mat4 MyGlWindow::lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up) {
	glm::vec3 zc = glm::normalize(campos - look);
	glm::vec3 xc = glm::normalize(glm::cross(up, zc));
	glm::vec3 yc = glm::normalize(glm::cross(zc, xc));

	glm::mat4 Mat4 = {
		{xc.x, yc.x, zc.x, 0},
		{xc.y, yc.y, zc.y, 0},
		{xc.z, yc.z, zc.z, 0},
		{0, 0, 0, 1}
	};
	glm::mat4 Mat4_1 = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{-campos.x, -campos.y, -campos.z, 1}
	};
	std::cout << glm::to_string(Mat4 * Mat4_1) << std::endl;
	return Mat4;
}
glm::mat4 MyGlWindow::perspective(float fovy, float aspect, float near, float far) {
	float A = -(far + near) / (far - near);
	float B = -(2*far + near) / (far - near);
	glm::mat4 Mat4 = {
		{1/(aspect*glm::tan(fovy/2)), 0, 0, 1},
		{0, 1 / glm::tan(fovy / 2), 0, 1},
		{0, 0, A, -1},
		{0, 0, B, 0}
	};
	std::cout << glm::to_string(Mat4) << std::endl;
	return Mat4;
}

GLuint vaoHandle;
GLuint vboPosition;
GLuint vboColor;
GLuint vboVertex;
void MyGlWindow::setupBuffer()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("simpler.vert", "simpler.frag"); // 쉐이더 지정
	shaderProgram->addUniform("model"); // 유니폼 추가

	//interleave 방법
	const float vertex[] = { // 디자이너들이 만든 local 좌표 값
		-0.2f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // r
		-0.2f, 0.4f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // g
		0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // b
		0.2f, 0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f // r
	};

	// 1. vao 생성
	glGenVertexArrays(1, &vaoHandle); // 1 개 생성
	glBindVertexArray(vaoHandle); // = 이 vao에 뭔가를 할거다..activate..picking..

	glGenBuffers(1, &vboVertex); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // 이 vbo를 선택한다, activate한다, picking한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer( // position
		0, // id 번호 : 0 (position)
		4, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		sizeof(float) * 7, // 각 vertex 당 크기
		0 //  offset
	);
	glEnableVertexAttribArray(0); // id 0을 enable

	glVertexAttribPointer( // color
		1, // id 번호 : 1 (color)
		3, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		sizeof(float) * 7, // 각 vertex 당 크기
		(void*)(sizeof(float) * 4) // offset
	);
	glEnableVertexAttribArray(1); // id 1을 enable

	//VAO close
	glBindVertexArray(0); // 인자를 0 넣으면 VAO close
}


// 매 프레임 호출 됨
void MyGlWindow::draw() {
	glClearColor(0.2, 0.2, 0.2, 1); // 백그라운드 색상 선택
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // 어떤게 앞에 있고 어떤게 뒤에 있는지 테스트

	//뷰포트(viewport) 설정
	glViewport(0, 0, width, height);

	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0.2, 0.2, 0));

	//shader 호출
	shaderProgram->use();

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //0: vertex 시작 인덱스, 3: number of vertices

	shaderProgram->disable();
}