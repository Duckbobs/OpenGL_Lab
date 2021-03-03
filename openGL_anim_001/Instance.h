#pragma once

#include <glm/gtc/matrix_transform.hpp> // translate()





struct Instance {
public:
	float getAnimationOffset();
	glm::vec3 getRotation();
	glm::vec3 getVelocity();
public:
	glm::mat4 getInstanceMatrix();
	void setAnimationOffset(float offset);
	void setPosition(glm::vec3 vec);
	void setRotation(glm::vec3 vec);
	void setScale(glm::vec3 vec);
	void setVelocity(glm::vec3 vec);

	void Update();
	bool updateMatrix();
private:
	float AnimationOffset;
	glm::mat4 aInstanceMatrix;
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::vec3 Velocity;

	bool hasVelocity = false;
	bool doUpdateMatrix = true;
	int delayUpdateMatrix = 0;
};