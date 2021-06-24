#pragma once

#include <glm/glm.hpp> // ���� ��� ����
#include "Loader.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include <vector>



class Line {
    unsigned int VBO, VAO;
    std::vector<glm::vec3> vertices;
public:
    Line(glm::vec3 start, glm::vec3 end);
    void draw();
};