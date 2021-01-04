#include "LightingCube.h"
const float vertices[] = {
	-0.5f, -0.5f, -0.5f, // back
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	 0.5f,  0.5f,  0.5f, // front
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f, // left
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f, // right
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f, // bottom
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f, // top
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
LightingCube::LightingCube() {
	LightingCube::setup();
}
void LightingCube::setup() {
	std::vector <glm::vec3> vbo_cube_vertices;
	std::vector <glm::vec3> vbo_cube_normals;
	glm::vec3 p1, p2, p3, n;

	int size = sizeof(vertices) / sizeof(float);
	std::cout << size << std::endl;
	for (int i = 0; i < size; i += 9) {
		p1 = glm::vec3(vertices[i], vertices[i+1], vertices[i+2]);
		p2 = glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]);
		p3 = glm::vec3(vertices[i+6], vertices[i+7], vertices[i+8]);
		vbo_cube_vertices.push_back(p1);
		vbo_cube_vertices.push_back(p2);
		vbo_cube_vertices.push_back(p3);
		std::cout << glm::to_string(p3) << std::endl;
		n = global::computeNormal(p1, p2, p3);
		vbo_cube_normals.push_back(n);
		vbo_cube_normals.push_back(n);
		vbo_cube_normals.push_back(n);
	}
	GLsize = vbo_cube_vertices.size() * 3;

	// 1. vao 생성
	glGenVertexArrays(1, &vaoHandle); // vao 1 개 생성
	glBindVertexArray(vaoHandle); // = 이 vao에 뭔가를  할거다..activate..picking..

	// 2. vbo 생성
	glGenBuffers(1, &vboVertex); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // 이 vbo를 선택한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vbo_cube_vertices.size() * 3, vbo_cube_vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // id 번호
		3, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		0, // 각 vertex 당 크기
		0 //  offset
	);
	glEnableVertexAttribArray(0); // id 0을 enable
	// 2. vbo 생성
	glGenBuffers(1, &vboNormal); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal); // 이 vbo를 선택한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vbo_cube_normals.size() * 3, vbo_cube_normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // id 번호
		3, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		0, // 각 vertex 당 크기
		0 //  offset
	);
	glEnableVertexAttribArray(1); // id 1을 enable

// 3. vao close
	glBindVertexArray(0); // 인자를 0 넣으면 VAO close
}

void LightingCube::draw() {
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, GLsize);
}