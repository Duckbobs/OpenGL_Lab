#pragma once
#include "Loader.h"
#include "Viewer.h"
#include <vector>
#include "global.h"
#include "Model.h"
#include "Plane.h"
#include <glm/gtc/matrix_transform.hpp> // translate()
#include <glm/gtc/type_ptr.hpp> // value_ptr()
#include <glm/gtx/string_cast.hpp> // to_string()
#include "time.h"
#include "Gui.h"
#include "Instance.h"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "imGuIZMOquat.h"
#include "Line.h"

class MyGlWindow {
public:
	int max_index = 0;
	int lab_index = 1;
	float animationSpeed = 1.0f;

	float animationSpeed_1 = 0.8f;
	float animationSpeed_2 = 0.6f;
	float animationSpeed_3 = 0.5f;
	int max_amount = 50000; // 최대 인스턴스 개수
	int amount = 500; // 인스턴스 개수

	MyGlWindow(int w, int h);
	void resize(int w, int h);
	void setSize(int w, int h);
	void draw(float animationTime);
	glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up);
	glm::mat4 perspective(float fov, float aspect, float n, float f);

	void setAspect(float r) { m_viewer->setAspectRatio(r); }
	void initialize();

	Viewer* m_viewer;
	Model* m_model;
	Plane* m_plane;
	Line* m_line;

	int getW() { return width; }
	int getH() { return height; }
	void mouse_button_click(double xpos, double ypos);
	glm::vec3 mouse_ray;
	std::vector<GLfloat> initPos;
private:
	int time = 0;
	bool loaded = false;
	ShaderProgram* shaderProgram;
	ShaderProgram* shaderProgram_plane;
	ShaderProgram* shaderProgram_gizmo;


	GLuint computeShaderProgram_test;

	int width;
	int height;
	void setupBuffer();

	std::vector<glm::mat4> Transforms;
	std::vector<glm::fdualquat> dualQuaternions;
	std::vector<glm::fdualquat> dualQuaternions2;
	std::vector<glm::mat2x4> DQs;
	glm::mat4* modelMatrices;
	glm::mat2x4* dqsMatrices;


	GLuint ssboHandle_t = 0;
	GLuint ssboHandle_ins = 1;

	GLuint ssboHandle_compute_test = 0;

	std::vector<Instance> Instances;
	std::vector<std::string> DQstrnames;
	std::vector<const char*> DQnames;

	// 함수
	glm::mat4 makeTransform();
};