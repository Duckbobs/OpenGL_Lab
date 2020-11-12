#include "Loader.h"
#include <glm/glm.hpp>

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void resize(int w, int h);
	void setSize(int w, int h);
	void draw();
	glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up);
	glm::mat4 perspective(float fovy, float aspect, float near, float far);

	//ColorCube* m_cube;
	//Viewer * m_viewer;

private:
	ShaderProgram* shaderProgram;
	int width;
	int height;

	void setupBuffer();
};