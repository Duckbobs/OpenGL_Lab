#pragma once
#include <string>
#include "glm/glm.hpp"

#include "texUtils.h"


struct Texture {
	std::string path;
	GLuint texId;
	GLenum textType;

	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;

	void bind(GLenum unit) {
		glActiveTexture(unit);
		glBindTexture(textType, texId);
	};
	void load(std::string path) {
		texId = loadImage(path.c_str());
	};

	Texture(std::string path, GLenum textType) {
		path = path;
		textType = textType;
		load(path);
	};

	GLuint
		Texture::getTexture() {
		return texId;
	}

	~Texture();
};

