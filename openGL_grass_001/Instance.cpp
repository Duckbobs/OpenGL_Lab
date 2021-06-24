#include "Instance.h"


void Instance::setInstanceMatrix(glm::mat4 mat) {
	data.aInstanceMatrix = mat;
}
void Instance::setAnimationOffset(float offset) {
	AnimationOffset = offset;
	doUpdateMatrix = true;
}
void Instance::setPosition(glm::vec3 vec) {
	data.Position = vec;
	doUpdateMatrix = true;
}
void Instance::setRotation(glm::vec3 vec) {
	data.Rotation = vec;
	doUpdateMatrix = true;
}
void Instance::setScale(glm::vec3 vec) {
	data.Scale = vec;
	doUpdateMatrix = true;
}
void Instance::setVelocity(glm::vec3 vec) {
	Velocity = vec;
	doUpdateMatrix = true;
	hasVelocity = true;
}


InsData Instance::getData() {
	return data;
}
glm::vec3 Instance::getPosition() {
	return data.Position;
}
glm::vec3 Instance::getRotation() {
	return data.Rotation;
}
glm::vec3 Instance::getVelocity() {
	return Velocity;
}
float Instance::getAnimationOffset() {
	return AnimationOffset;
}
glm::mat4 Instance::getInstanceMatrix() {
	return data.aInstanceMatrix;
}

void Instance::Update() {
	if (hasVelocity) {
		// Velocity �� ���� ��� ��ġ �̵�
		data.Position = data.Position + Velocity;
		doUpdateMatrix = true;
	}
}

bool Instance::updateMatrix() {
	// doUpdateMatrix �� ���� ��Ʈ������ ������Ʈ�� �ʿ䰡 �ִ��� Ȯ��
	if (doUpdateMatrix) {
		doUpdateMatrix = false;
		if (init) {
			init = false;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, data.Position);
			model = glm::scale(model, data.Scale);
			model = glm::rotate(model, data.Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, data.Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, data.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			data.aInstanceMatrix = model;
		}
		return true;
	}
	return false;
}
