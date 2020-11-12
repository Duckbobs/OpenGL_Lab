#include "Loader.h"
#include <glm/glm.hpp>
#include "Viewer.h"

class ColorCube
{
private:
	GLuint vaoHandle;
	GLuint vboVertex;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	GLuint ibo_cube_elements;
public:
	void draw();
	void setup();
};
class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void resize(int w, int h);
	void setSize(int w, int h);
	void draw();
	glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up);
	glm::mat4 perspective(float fovy, float aspect, float near, float far);

	ColorCube* m_cube;
	void setAspect(float r) { m_viewer->setAspectRatio(r); }
	void initialize();

	Viewer* m_viewer;
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