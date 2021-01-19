#include "MyGlWindow.h"
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp> // translate()
#include <glm/gtc/type_ptr.hpp> // value_ptr()

#include <glm/gtx/string_cast.hpp> // to_string()

MyGlWindow::MyGlWindow(int w, int h)
{
	width = w;
	height = h;

	setupBuffer(); // �� �ѹ� ȣ��
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
	shaderProgram->initFromFiles("simpler.vert", "simpler.frag"); // ���̴� ����
	shaderProgram->addUniform("model"); // ������ �߰�

	//interleave ���
	const float vertex[] = { // �����̳ʵ��� ���� local ��ǥ ��
		-0.2f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // r
		-0.2f, 0.4f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // g
		0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // b
		0.2f, 0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f // r
	};

	// 1. vao ����
	glGenVertexArrays(1, &vaoHandle); // 1 �� ����
	glBindVertexArray(vaoHandle); // = �� vao�� ������ �ҰŴ�..activate..picking..

	glGenBuffers(1, &vboVertex); // vbo 1�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // �� vbo�� �����Ѵ�, activate�Ѵ�, picking�Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer( // position
		0, // id ��ȣ : 0 (position)
		4, // �� vertex �� ����Ÿ ��
		GL_FLOAT, // ����Ÿ Ÿ��
		GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
		sizeof(float) * 7, // �� vertex �� ũ��
		0 //  offset
	);
	glEnableVertexAttribArray(0); // id 0�� enable

	glVertexAttribPointer( // color
		1, // id ��ȣ : 1 (color)
		3, // �� vertex �� ����Ÿ ��
		GL_FLOAT, // ����Ÿ Ÿ��
		GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
		sizeof(float) * 7, // �� vertex �� ũ��
		(void*)(sizeof(float) * 4) // offset
	);
	glEnableVertexAttribArray(1); // id 1�� enable

	//VAO close
	glBindVertexArray(0); // ���ڸ� 0 ������ VAO close
}


// �� ������ ȣ�� ��
void MyGlWindow::draw() {
	glClearColor(0.2, 0.2, 0.2, 1); // ��׶��� ���� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // ��� �տ� �ְ� ��� �ڿ� �ִ��� �׽�Ʈ

	//����Ʈ(viewport) ����
	glViewport(0, 0, width, height);

	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0.2, 0.2, 0));

	//shader ȣ��
	shaderProgram->use();

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //0: vertex ���� �ε���, 3: number of vertices

	shaderProgram->disable();
}