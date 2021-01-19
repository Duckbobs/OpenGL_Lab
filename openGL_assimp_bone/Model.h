#pragma once

#include <glm/glm.hpp> // 순서 상관 없음
#include <vector>

#include "AssimpLoader.h"


class Model {
public:
    /*  함수   */
    Model(char* path);
    void Draw(ShaderProgram* shader);
    //void Draw();
private:
    ModelData modelData;
    glm::mat4 CalcBoneTransform(aiAnimation* anim);
};