#include "Loader.h"
#include "Viewer.h"
#include "ColorCube.h"

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
	ColorCube* m_cube;
private:
	ShaderProgram* shaderProgram;
	int width;
	int height;

	void setupBuffer();

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 mvp;
};