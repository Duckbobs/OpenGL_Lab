#pragma once

#include "Loader.h"
#include <glm/glm.hpp> // 순서 상관 없음
#include <vector>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
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
    /*  함수         */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderProgram shaderProgram);
private:
    /*  렌더 데이터  */
    unsigned int VAO, VBO, EBO;
    /*  함수         */
    void setupMesh();
};

class Model
{
public:
    /*  함수   */
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(ShaderProgram shader);
private:
    /*  Model 데이터  */
    std::vector<Mesh> meshes;
    std::string directory;
    /*  함수   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};