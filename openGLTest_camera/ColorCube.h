#include "Loader.h"

class ColorCube
{
private:
	GLuint vaoHandle;
	GLuint vboVertex;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	GLuint ibo_cube_elements;
	void setup();
public:
	ColorCube();
	void draw();
};