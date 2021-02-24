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

struct Instance {
	float AnimationOffset;
	glm::mat4 aInstanceMatrix;
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

};

class MyGlWindow {
public:
	int max_amount = 50000; // 최대 인스턴스 개수
	int amount = 10000; // 인스턴스 개수

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

private:
	int time = 0;
	bool loaded = false;
	ShaderProgram* shaderProgram;
	ShaderProgram* shaderProgram_plane;
	int width;
	int height;
	void setupBuffer();

	std::vector<glm::mat4> Transforms;
	std::vector<glm::fdualquat> dualQuaternions;
	std::vector<glm::mat2x4> DQs;
	glm::mat4* modelMatrices;
	glm::mat2x4* dqsMatrices;


	GLuint ssboHandle_t = 0;
	GLuint ssboHandle_ins = 1;
	GLuint ssboHandle_offset = 2;

	std::vector<Instance> Instances;
	std::vector<std::string> DQstrnames;
	std::vector<const char*> DQnames;

	// 함수
	glm::mat4 makeTransform();
};