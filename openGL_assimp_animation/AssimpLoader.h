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
    unsigned int vertexID;
    float weight;
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
    void Draw(ShaderProgram *shaderProgram);
    void Draw();
private:

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
    void Draw(ShaderProgram *shader);
    void Draw();

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



/*
for (unsigned int i = 0; i < mesh->mNumBones; i++) {
    std::cout << mesh->mName.C_Str() << std::endl;
}

int numBones = 0;
for (int i = 0; i < mesh->mNumBones; i++)
{
    unsigned int boneIndex = numBones++;

    for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
    {
        unsigned int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
        float weight = mesh->mBones[i]->mWeights[j].mWeight;

        // 정점은 최대 8개의 Bone의 영향을 받게 됨
        // 2개의 4차원 벡터를 이용하여 값을 저장
        for (int k = 0; k < 8; k++)
        {
            // 벡터의 인덱스
            unsigned int vectorId = k / 4;
            // 각 벡터의 원소 인덱스
            unsigned int elementId = k % 4;
            // push_back 효과를 구현
            if (vertices[vertexId].boneWeights[vectorId][elementId] == 0.0f)
            {
                vertices[vertexId].boneIds[vectorId][elementId] = boneIndex;
                vertices[vertexId].boneWeights[vectorId][elementId] = weight;
                break;
            }
            vertices.push_back(vertex);
        }
    }
}*/