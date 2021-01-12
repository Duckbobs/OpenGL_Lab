#pragma once

//#include "stb_image.h"
//#define STB_IMAGE_IMPLEMENTATION

#include "Loader.h"
#include <glm/glm.hpp> // 순서 상관 없음
#include <vector>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class AssimpLoader
{
public:
    void loadModel(std::vector<Mesh>* meshes, std::string path);
private:
    std::vector<Mesh>* meshes;
    std::string directory;
    /*  함수   */
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    //unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
    //std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};