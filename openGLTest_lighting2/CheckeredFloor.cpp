#include "CheckeredFloor.h"

CheckeredFloor::CheckeredFloor(float size, int nSquares) {
	CheckeredFloor::setup(size, nSquares);
}
void CheckeredFloor::setup(float size, int nSquares) {

	floorColor1[0] = 200;
	floorColor1[1] = 200;
	floorColor1[2] = 200;

	floorColor2[0] = 0;
	floorColor2[1] = 0;
	floorColor2[2] = 0;


	std::vector <glm::vec3> vlists;
	std::vector <glm::vec3> clists;
	std::vector <glm::vec3> nlists;

	// parameters:
	float maxX = size / 2, maxY = size / 2;
	float minX = -size / 2, minY = -size / 2;

	int x, y, v[3], i;
	float xp, yp, xd, yd;
	v[2] = 0;
	xd = (maxX - minX) / ((float)nSquares);
	yd = (maxY - minY) / ((float)nSquares);


	for (x = 0, xp = minX; x < nSquares; x++, xp += xd) {
		for (y = 0, yp = minY, i = x; y < nSquares; y++, i++, yp += yd) {
			if (i % 2 == 1) {
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
			}
			else {
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));

			}
			glm::vec3 p1, p2, p3, v1, v2, n;
			vlists.push_back(glm::vec3(xp, -0.001f, yp));
			vlists.push_back(glm::vec3(xp, -0.001f, yp + yd));
			vlists.push_back(glm::vec3(xp + xd, -0.001f, yp + yd));

			p1 = glm::vec3(xp, -0.001f, yp);
			p2 = glm::vec3(xp, -0.001f, yp + yd);
			p3 = glm::vec3(xp + xd, -0.001f, yp + yd);
			v1 = p2 - p1;
			v2 = p3 - p1;
			n = glm::cross(v1, v2);

			nlists.push_back(n);
			nlists.push_back(n);
			nlists.push_back(n);

			vlists.push_back(glm::vec3(xp, -0.001f, yp));
			vlists.push_back(glm::vec3(xp + xd, -0.001f, yp + yd));
			vlists.push_back(glm::vec3(xp + xd, -0.001f, yp));

			p1 = glm::vec3(xp, -0.001f, yp);
			p2 = glm::vec3(xp + xd, -0.001f, yp + yd);
			p3 = glm::vec3(xp + xd, -0.001f, yp);
			v1 = p2 - p1;
			v2 = p3 - p1;
			n = glm::cross(v1, v2);

			nlists.push_back(n);
			nlists.push_back(n);
			nlists.push_back(n);
		}
	}

	// 1. vao 생성
	glGenVertexArrays(1, &vaoHandle); // vao 1 개 생성
	glBindVertexArray(vaoHandle); // = 이 vao에 뭔가를  할거다..activate..picking..

	GLsize = vlists.size() * 3;
	// 2. vbo 생성
	glGenBuffers(1, &vboVertex); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex); // 이 vbo를 선택한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vlists.size() * 3, vlists.data(), GL_STATIC_DRAW);
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
	glGenBuffers(1, &vboColor); // vbo 1개 생성
	glBindBuffer(GL_ARRAY_BUFFER, vboColor); // 이 vbo를 선택한다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nlists.size() * 3, nlists.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // id 번호
		3, // 각 vertex 당 데이타 수
		GL_FLOAT, // 데이타 타입
		GL_FALSE, // 정규화(normalize) 되어있나? GL_FALSE = 안돼있다
		0, // 각 vertex 당 크기
		0 // offset
	);
	glEnableVertexAttribArray(1); // id 1을 enable

// 3. vao close
	glBindVertexArray(0); // 인자를 0 넣으면 VAO close
}

void CheckeredFloor::draw() {
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, GLsize);
}
