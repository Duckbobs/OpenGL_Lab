#include "Sphere.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <cstddef>

template<typename T, size_t SIZE>
size_t getSize(T(&)[SIZE]) {
	return SIZE;
}
Sphere::Sphere()
{

}


Sphere::~Sphere()
{
}



Sphere::Sphere(float rad, GLuint sl, GLuint st) :
radius(rad), slices(sl), stacks(st)
{


	

	nVerts = (slices + 1) * (stacks + 1);
	elements = (slices * 2 * (stacks - 1)) * 3;

	// Verts
	float * v = new float[3 * nVerts];
	// Normals
	float * n = new float[3 * nVerts];  
	// Tex coords
	float * tex = new float[2 * nVerts];    //we don't use it now
	// Index
	unsigned int * el = new unsigned int[elements];  //index

	// Generate the vertex data
	generateVerts(v, n, tex, el);
	

	//create vao, vbo and ibo here... (We didn't use std::vector here...)

	// 1. vao ����
	glGenVertexArrays(1, &VAO); // vao 1 �� ����
	glBindVertexArray(VAO); // = �� vao�� ������  �ҰŴ�..activate..picking..
	// 2. vbo ����
	glGenBuffers(1, &VBO_position); // vbo 1�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position); // �� vbo�� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nVerts, v, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // id ��ȣ
		3, // �� vertex �� ����Ÿ ��
		GL_FLOAT, // ����Ÿ Ÿ��
		GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
		0, // �� vertex �� ũ��
		0 //  offset
	);
	glEnableVertexAttribArray(0); // id 0�� enable
	// 2. vbo ����
	glGenBuffers(1, &VBO_normal); // vbo 1�� ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal); // �� vbo�� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nVerts, n, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // id ��ȣ
		3, // �� vertex �� ����Ÿ ��
		GL_FLOAT, // ����Ÿ Ÿ��
		GL_FALSE, // ����ȭ(normalize) �Ǿ��ֳ�? GL_FALSE = �ȵ��ִ�
		0, // �� vertex �� ũ��
		0 //  offset
	);
	glEnableVertexAttribArray(1); // id 1�� enable
	// 2. ibo ����
	glGenBuffers(1, &IBO); // ibo 1�� ����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // �� vbo�� �����Ѵ�.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6 * nVerts, el, GL_STATIC_DRAW);

// 3. vao close
	glBindVertexArray(0); // ���ڸ� 0 ������ VAO close


	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tex;
}

void Sphere::draw() 
{
	glBindVertexArray(VAO);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

void Sphere::generateVerts(float * verts, float * norms, float * tex,
	unsigned int * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}

int Sphere::getVertexArrayHandle() 
{
	return this->VAO;
}
