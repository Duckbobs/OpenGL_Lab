#pragma once

//#include "stb_image.h"
//#define STB_IMAGE_IMPLEMENTATION

#include "Loader.h"
#include <glm/glm.hpp> // 순서 상관 없음
#include <vector>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Mesh.h"


struct ModelData {
    /*  Model 데이터  */
    std::vector<Mesh> meshes;
    std::vector<aiAnimation*> animations;
};

class AssimpLoader
{
public:
    AssimpLoader(ModelData* modelData, std::string path);
private:
    //std::string directory;
    /*  Scene 데이터  */
    std::vector<Mesh>* meshes;

    /*  함수  */
    void loadModel(std::vector<Mesh>* meshes, std::vector<aiAnimation*>* animations, std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    //unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
    //std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};