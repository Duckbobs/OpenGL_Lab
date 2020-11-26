#include "Loader.h"


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

class CheckeredFloor
{
private:
	GLuint vaoHandle;
	GLuint vboVertex;
	GLuint vboColor;
	
	GLuint vbo_board_vertices, vbo_board_colors;
	GLuint ibo_board_elements;
	float floorColor1[3];
	float floorColor2[3];
	void setup(float size, int nSquares);
public:
	CheckeredFloor(float size, int nSquares);
	void draw();
};