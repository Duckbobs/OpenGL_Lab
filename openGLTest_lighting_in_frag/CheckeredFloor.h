#include "Loader.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include <glm/glm.hpp>

class CheckeredFloor
{
private:
	GLuint vaoHandle;
	GLuint vboVertex;
	GLuint vboColor;
	float floorColor1[3];
	float floorColor2[3];
	void setup(float size, int nSquares);
	int GLsize;
public:
	CheckeredFloor(float size, int nSquares);
	void draw();
};