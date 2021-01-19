#pragma once

#include <glm/glm.hpp> // 순서 상관 없음
#include <vector>
#include "Loader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    //glm::vec2 TexCoords;
    float boneWeights[2][4] = { 0.0f };
    float boneIds[2][4] = { 0.0f };
};
/*struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};*/
struct Bone
{
    glm::mat4 boneOffset;
    glm::mat4 finalTransform;
};

class Mesh {
public:
    /*  Mesh 데이터  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture> textures;
    /*  Bone 데이터  */
    std::vector<Bone> bones;

    /*  함수         */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices/*, std::vector<Texture> textures*/);
    void Draw(ShaderProgram* shaderProgram);
    //void Draw();
private:

private:
    /*  렌더 데이터  */
    unsigned int VAO, VBO, EBO;
    /*  함수         */
    void setupMesh();
};