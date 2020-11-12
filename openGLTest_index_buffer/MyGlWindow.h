#include "Loader.h"
#include <glm/glm.hpp>


class ColorCube
{
private:
	GLuint vaoHandle;
	GLuint vboVertex;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	GLuint ibo_cube_elements;
	void setup();
public:
	void draw();
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
	//Viewer* m_viewer;
private:
	ShaderProgram* shaderProgram;
	int width;
	int height;

	void setupBuffer();
};