#include "MyGlWindow.h"
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp> // translate()
#include <glm/gtc/type_ptr.hpp> // value_ptr()

#include <glm/gtx/string_cast.hpp> // to_string()

#include <vector>

void MyGlWindow::setupBuffer()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("simpler.vert", "simpler.frag"); // 쉐이더 지정
	m_cube = new ColorCube();
}
void MyGlWindow::initialize() {
	MyGlWindow::setupBuffer();
	shaderProgram->addUniform("mvp");
}
MyGlWindow::MyGlWindow(int w, int h)
{
	width = w;
	height = h;
	m_cube = 0;


	static float DEFAULT_VIEW_POINT[3] = { 5, 5, 5 };
	static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
	static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };
	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	initialize();
}
const glm::mat4 _mat = glm::mat4(1.0f);
const glm::vec3 _vec = glm::vec3(1, 1, 1);
// 매 프레임 호출 됨
void MyGlWindow::draw() {
	// @------------------------------------------------------------------------------@
	// | Settings																	  |
	// @------------------------------------------------------------------------------@
#pragma region Settings
	glClearColor(0.2, 0.2, 0.2, 1); // 백그라운드 색상 선택
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height); //뷰포트(viewport) 설정
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion


	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();
	view = lookAt(eye, look, up);

	glm::mat4 trans = glm::translate(_mat, glm::vec3(0, 0, 0));
	glm::mat4 rot = glm::rotate(_mat, glm::radians(45.0f), _vec);
	glm::mat4 scale = glm::scale(_mat, _vec);
	model = trans * rot * scale;

	projection = glm::perspective(45.0f, 1.0f * width / height, 0.1f, 500.0f);
	
	mvp = projection * view * model;

	shaderProgram->use(); // shader 호출


	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	if (m_cube) {
		m_cube->draw();
	}

	shaderProgram->disable();
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
	return Mat4 * Mat4_1;
}
glm::mat4 MyGlWindow::perspective(float fovY, float aspect, float near, float far) {
	float A = -(far + near) / (far - near);
	float B = -(2 * far + near) / (far - near);
	glm::mat4 Mat4 = {
		{1 / (aspect * glm::tan(fovY / 2)), 0, 0, 1},
		{0, 1 / glm::tan(fovY / 2), 0, 1},
		{0, 0, A, -1},
		{0, 0, B, 0}
	};
	return Mat4;
}

