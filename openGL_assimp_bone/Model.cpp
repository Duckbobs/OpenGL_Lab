#include "Model.h"

Model::Model(char* path)
{
    AssimpLoader * assimpLoader = new AssimpLoader();
    assimpLoader->loadModel(&meshes, path);
}
void Model::Draw(ShaderProgram* shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}
void Model::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw();
}