/*#include "AnimationComponent.h"

// 특정 시간(animationTime)과 특정 애니메이션 인덱스(animationIndex)를 넘겨주면
// 각 Bone의 형태를 Matrix 형식으로 리턴하는 함수
// 매 프레임마다 호출됨
std::vector<glm::mat4>* AnimationComponent::ExtractBoneTransforms(float animationTime, const int animationIndex)
{
    // 애니메이션이 계속 반복되도록 fmod 연산을 취함
    animationTime = fmod(animationTime, scene->mAnimations[animationIndex]->mDuration);
    // root node와 단위 행렬을 인자로 넘겨주면 재귀 호출을 통하여 boneInfos에 데이터를 저장하는 함수
    ReadNodeHierarchy(animationTime, scene->mRootNode, glm::mat4(1.0f));

    for (int i = 0; i < scene->mMeshes[0]->mNumBones; i++)
    {
        // boneTransforms는 vector<mat4> 타입으로 크기는 Bone의 개수와 같음
        boneTransforms[i] = boneInfos[i].finalTransform;
    }
    return &boneTransforms;
}

// 재귀 호출을 통해 자식 노드로 내려가면서 각각 매칭된 boneTransformation을 저장하는 함수
void AnimationComponent::ReadNodeHierarchy(float animationTime, const aiNode* node, const glm::mat4& parentTransform)
{

    std::string nodeName(node->mName.data);

    const aiAnimation* animation = scene->mAnimations[0];
    glm::mat4 nodeTransform = ToMat4(node->mTransformation);

    const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);

    // 애니메이션 정보가 있는 node라면
    if (nodeAnim)
    {
        // 주어진 key frame의 정보와 animationTime 정보를 이용해 interpolation을 하고 값을 저장
        const aiVector3D& scaling = CalcInterpolatedValueFromKey(animationTime, nodeAnim->mNumScalingKeys, nodeAnim->mScalingKeys);
        glm::mat4 scalingM = scale(mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z));

        const aiQuaternion& rotationQ = CalcInterpolatedValueFromKey(animationTime, nodeAnim->mNumRotationKeys, nodeAnim->mRotationKeys);
        glm::mat4 rotationM = glm::mat4(quat(rotationQ.w, rotationQ.x, rotationQ.y, rotationQ.z));

        const aiVector3D& translation = CalcInterpolatedValueFromKey(animationTime, nodeAnim->mNumPositionKeys, nodeAnim->mPositionKeys);
        glm::mat4 translationM = translate(mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

        nodeTransform = translationM * rotationM * scalingM;
    }

    // globalTransform은 bone space에서 정의되었던 정점들을 model space에서 정의되도록 함
    // parentTransform은 부모 bone space에서 정의되었던 정점들을 model space에서 정의되도록 함
    // nodeTransform은 bone space에서 정의되었던 정점들을 부모 bone space에서 정의되도록 함, 
    // 혹은 부모 bone space를 기준으로 한 일종의 변환
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    // bone이 있는 노드에 대해서만 bone Transform을 저장
    // boneMap은 map<string, int> 타입으로 bone의 이름과 index 저장
    if (boneMap.find(nodeName) != boneMap.end())
    {
        unsigned int boneIndex = boneMap[nodeName];
        boneInfos[boneIndex].finalTransform =
            ToMat4(scene->mRootNode->mTransformation) *
            globalTransform *
            // boneOffset은 model space에서 정의되었던 정점들을 bone space에서 정의되도록 만드는 것
            boneInfos[boneIndex].boneOffset;
    }

    // 모든 자식 노드에 대해 재귀 호출
    for (int i = 0; i < node->mNumChildren; i++)
        ReadNodeHierarchy(animationTime, node->mChildren[i], globalTransform);
}

aiNodeAnim* AnimationComponent::FindNodeAnim(const aiAnimation* animation, const std::string nodeName)
{
    for (int i = 0; i < animation->mNumChannels; i++)
        if (animation->mChannels[i]->mNodeName.data == nodeName)
            return animation->mChannels[i];
    return nullptr;
}

aiVector3D AnimationComponent::CalcInterpolatedValueFromKey(float animationTime, const int numKeys, const aiVectorKey* const vectorKey) const
{
    aiVector3D ret;
    if (numKeys == 1)
    {
        ret = vectorKey[0].mValue;
        return ret;
    }

    unsigned int keyIndex = FindKeyIndex(animationTime, numKeys, vectorKey);
    unsigned int nextKeyIndex = keyIndex + 1;

    assert(nextKeyIndex < numKeys);

    float deltaTime = vectorKey[nextKeyIndex].mTime - vectorKey[keyIndex].mTime;
    float factor = (animationTime - (float)vectorKey[keyIndex].mTime) / deltaTime;

    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D& startValue = vectorKey[keyIndex].mValue;
    const aiVector3D& endValue = vectorKey[nextKeyIndex].mValue;

    ret.x = startValue.x + (endValue.x - startValue.x) * factor;
    ret.y = startValue.y + (endValue.y - startValue.y) * factor;
    ret.z = startValue.z + (endValue.z - startValue.z) * factor;

    return ret;
}

aiQuaternion AnimationComponent::CalcInterpolatedValueFromKey(float animationTime, const int numKeys, const aiQuatKey* const quatKey) const
{
    aiQuaternion ret;
    if (numKeys == 1)
    {
        ret = quatKey[0].mValue;
        return ret;
    }

    unsigned int keyIndex = FindKeyIndex(animationTime, numKeys, quatKey);
    unsigned int nextKeyIndex = keyIndex + 1;

    assert(nextKeyIndex < numKeys);

    float deltaTime = quatKey[nextKeyIndex].mTime - quatKey[keyIndex].mTime;
    float factor = (animationTime - (float)quatKey[keyIndex].mTime) / deltaTime;

    assert(factor >= 0.0f && factor <= 1.0f);

    const aiQuaternion& startValue = quatKey[keyIndex].mValue;
    const aiQuaternion& endValue = quatKey[nextKeyIndex].mValue;
    aiQuaternion::Interpolate(ret, startValue, endValue, factor);
    ret = ret.Normalize();

    return ret;
}

unsigned int AnimationComponent::FindKeyIndex(const float animationTime, const int numKeys, const aiVectorKey* const vectorKey) const
{
    assert(numKeys > 0);
    for (int i = 0; i < numKeys - 1; i++)
        if (animationTime < (float)vectorKey[i + 1].mTime)
            return i;
    assert(0);
}

unsigned int AnimationComponent::FindKeyIndex(const float animationTime, const int numKeys, const aiQuatKey* const quatKey) const
{
    assert(numKeys > 0);
    for (int i = 0; i < numKeys - 1; i++)
        if (animationTime < (float)quatKey[i + 1].mTime)
            return i;
    assert(0);
}*/