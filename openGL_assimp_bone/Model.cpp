#include "Model.h"
#include <glm/gtc/type_ptr.hpp> // value_ptr()

Model::Model(char* path)
{
    AssimpLoader* assimpLoader = new AssimpLoader(&modelData, path);

}
void Model::Draw(ShaderProgram* shader)
{
    glm::mat4 boneTransform = CalcBoneTransform(modelData.animations[0]);
    for (unsigned int i = 0; i < modelData.meshes.size(); i++) {
        glUniformMatrix4fv(shader->uniform("boneTransform"), 1, GL_FALSE, glm::value_ptr(boneTransform));

        modelData.meshes[i].Draw(shader);
    }
}

glm::mat4 Model::CalcBoneTransform(aiAnimation* anim) {
    glm::mat4 matrix;
    
    //matrix = glm::translate(glm::mat4(1.0f), glm::vec3(rand() % 100, 0.0f, 0.0f));
    matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rand() % 100 * 0.5f), glm::vec3(1.0f, 0, 0));
    return matrix;
}
/*void Model::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw();
}*/