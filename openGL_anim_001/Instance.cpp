#include "Instance.h"

#define TODEG(x) (x * (180.f / PI))
#define TORAD(x) (x * (PI / 180.f))





void Instance::setAnimationOffset(float offset) {
	AnimationOffset = offset;
	doUpdateMatrix = true;
}
void Instance::setPosition(glm::vec3 vec) {
	Position = vec;
	doUpdateMatrix = true;
}
void Instance::setRotation(glm::vec3 vec) {
	Rotation = vec;
	doUpdateMatrix = true;
}
void Instance::setScale(glm::vec3 vec) {
	Scale = vec;
	doUpdateMatrix = true;
}
void Instance::setVelocity(glm::vec3 vec) {
	Velocity = vec;
	doUpdateMatrix = true;
	hasVelocity = true;
}


glm::vec3 Instance::getRotation() {
	return Rotation;
}
float Instance::getAnimationOffset() {
	return AnimationOffset;
}
glm::mat4 Instance::getInstanceMatrix() {
	return aInstanceMatrix;
}

void Instance::Update() {
	if (hasVelocity) {
		// Velocity 가 있을 경우 위치 이동
		Position = Position + Velocity;
		doUpdateMatrix = true;
	}
}
bool Instance::updateMatrix() {
	// doUpdateMatrix 를 통해 매트릭스를 업데이트할 필요가 있는지 확인
	if (doUpdateMatrix) {
		if (delayUpdateMatrix-- == 0) {
			delayUpdateMatrix = 1; // 매트릭스 업데이트 딜레이 ( 0 = 딜레이 없음 )
			doUpdateMatrix = false;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, Position);
			model = glm::scale(model, Scale);
			model = glm::rotate(model, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			aInstanceMatrix = model;
		}
		return true;
	}
	return false;
}