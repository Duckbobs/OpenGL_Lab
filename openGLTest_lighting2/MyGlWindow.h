#include "Loader.h"
#include "Viewer.h"
#include "LightingCube.h"
#include "CheckeredFloor.h"
#include "Sphere.h"
#include "VBOTeapot.h"

#include <vector>

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void resize(int w, int h);
	void setSize(int w, int h);
	void draw();
	glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up);
	glm::mat4 perspective(float fovy, float aspect, float near, float far);

	void setAspect(float r) { m_viewer->setAspectRatio(r); }
	void initialize();

	Viewer* m_viewer;
	LightingCube* m_cube;
	CheckeredFloor* m_board;
	Sphere* m_sphere;
	VBOTeapot* m_teapot;
private:
	ShaderProgram* shaderProgram;
	ShaderProgram* shaderProgram_Floor;
	int width;
	int height;

	void setupBuffer();
};