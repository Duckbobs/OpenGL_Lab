#pragma once

#include "Loader.h"
#include "Viewer.h"
#include <vector>
#include "global.h"

#include "Model.h"

#include <glm/gtc/matrix_transform.hpp> // translate()
#include <glm/gtc/type_ptr.hpp> // value_ptr()

#include <glm/gtx/string_cast.hpp> // to_string()

struct Instance {
	float AnimationOffset;
	glm::mat4 aInstanceMatrix;
};

class MyGlWindow {
public:
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

private:
	int time = 0;
	bool loaded = false;
	unsigned int amount = 10; // 인스턴스 개수
	ShaderProgram* shaderProgram;
	int width;
	int height;
	void setupBuffer();

	std::vector<glm::mat4> Transforms;
	std::vector<glm::fdualquat> dualQuaternions;
	std::vector<glm::mat2x4> DQs;
	glm::mat4* modelMatrices;
	glm::mat2x4* dqsMatrices;
	GLuint ssboHandle_t = 0;

	std::vector<Instance> Instances;
	std::vector<std::string> DQstrnames;
	std::vector<const char*> DQnames;
};