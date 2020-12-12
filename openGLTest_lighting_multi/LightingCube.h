#include "Loader.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include <glm/glm.hpp>
#include "global.h"
#include <glm/gtx/string_cast.hpp> // to_string()

class LightingCube
{
private:
	GLuint vaoHandle;
	GLuint vboVertex;
	GLuint vboNormal;
	void setup();
	int GLsize;
public:
	LightingCube();
	void draw();
};