#define GLM_FORCE_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>
int main() {
    glm::vec3 A(1,2,-4);
    glm::vec3 B(2,3,5);
    std::cout << "" << glm::dot(A, B) << std::endl;
    return 0;
}