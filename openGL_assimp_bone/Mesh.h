#include <glm/glm.hpp> // 순서 상관 없음
#include <vector>
#include "Loader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
    glm::vec3 Bone;
};
struct Bone {
    unsigned int ID;
    float Weights[];
};
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    /*  Mesh 데이터  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    std::vector<Bone> bones;

    /*  함수         */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderProgram* shaderProgram);
    void Draw();
private:

private:
    /*  렌더 데이터  */
    unsigned int VAO, VBO, EBO, BONE_VBO;
    /*  함수         */
    void setupMesh();
};