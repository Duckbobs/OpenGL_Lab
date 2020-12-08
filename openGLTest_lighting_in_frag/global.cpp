#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace global
{
	glm::vec3 lightPos = glm::vec3(10, 10, 10);
	glm::vec3 backGround = glm::vec3(0.2, 0.2, 0.2);
	glm::ivec2 floorSize = glm::vec2(10, 5);

	glm::vec3 computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		glm::vec3 v1 = p2 - p1;
		glm::vec3 v2 = p3 - p1;
		glm::vec3 n = glm::cross(v1, v2);
		return glm::normalize(n);
	}
}