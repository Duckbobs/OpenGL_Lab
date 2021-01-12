#include <glm/glm.hpp> // 순서 상관 없음
#include <vector>

#include "AssimpLoader.h"

class Model {
public:
    /*  함수   */
    Model(char* path);
    void Draw(ShaderProgram* shader);
    void Draw();
private:
    /*  Model 데이터  */
    std::vector<Mesh> meshes;
};